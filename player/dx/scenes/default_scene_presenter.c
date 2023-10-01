#include "dx/scenes/default_scene_presenter.h"

// _strdup
#include <string.h>
// sinf
#include <math.h>
// snprintf
#include <stdio.h>
#include "dx/val/cbinding.h"
#include "dx/scenes/create_assets.h"
#include "dx/val/viewer.h"
#include "dx/data_definition_language.h"
#include "dx/assets.h"

DX_DEFINE_OBJECT_TYPE("dx.default_scene_presenter",
                      dx_default_scene_presenter,
                      dx_scene_presenter);

static dx_result on_scene_asset_object(dx_default_scene_presenter* SELF, dx_val_context* context, dx_object* asset_object) {
  // mesh instance
  if (dx_rti_type_is_leq(asset_object->type, dx_asset_mesh_instance_get_type())) {
    dx_asset_mesh_instance* asset_mesh_instance = DX_ASSET_MESH_INSTANCE(asset_object);
    dx_val_mesh* mesh = NULL;
    if (dx_val_mesh_create(&mesh, context, DX_ASSET_MESH(asset_mesh_instance->mesh_reference->object))) {
      return DX_FAILURE;
    }
    dx_val_mesh_instance* mesh_instance = NULL;
    if (dx_val_mesh_instance_create(&mesh_instance, asset_mesh_instance->world_matrix->value, mesh)) {
      DX_UNREFERENCE(mesh);
      mesh = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(mesh);
    mesh = NULL;
    if (dx_inline_object_array_append(&SELF->mesh_instances, DX_OBJECT(mesh_instance))) {
      DX_UNREFERENCE(mesh_instance);
      mesh_instance = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(mesh_instance);
    mesh_instance = NULL;
    return DX_NO_ERROR != dx_get_error() ? DX_FAILURE : DX_SUCCESS;
  }
  if (dx_get_error()) {
    return DX_FAILURE;
  }
  if (dx_rti_type_is_leq(asset_object->type, dx_asset_material_get_type())) {
    dx_asset_material* asset_material = DX_ASSET_MATERIAL(asset_object);
    if (dx_inline_object_array_append(&SELF->asset_material_objects, DX_OBJECT(asset_material))) {
      DX_UNREFERENCE(asset_material);
      asset_material = NULL;
      return DX_FAILURE;
    }
    return DX_NO_ERROR != dx_get_error() ? DX_FAILURE : DX_SUCCESS;
  }
  // viewer instance
  if (dx_rti_type_is_leq(asset_object->type, dx_asset_viewer_instance_get_type())) {
    dx_asset_viewer_instance* asset_viewer_instance = DX_ASSET_VIEWER_INSTANCE(asset_object);
    dx_val_viewer* viewer;
    if (dx_val_viewer_create(&viewer, asset_viewer_instance)) {
      return DX_FAILURE;
    }
    if (dx_inline_object_array_append(&SELF->viewers, DX_OBJECT(viewer))) {
      DX_UNREFERENCE(viewer);
      viewer = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(viewer);
    viewer = NULL;
    return DX_NO_ERROR != dx_get_error() ? DX_FAILURE : DX_SUCCESS;
  }
  if (dx_get_error()) {
    return DX_FAILURE;
  }
  dx_log("ignoring unknown/unsupported scene object\n", sizeof("ignoring unknown/unsupported scene object\n") - 1);
  return DX_SUCCESS;
}

static dx_result mesh_instance_on_startup(dx_default_scene_presenter* SELF, dx_val_context* context, dx_asset_scene* asset_scene) {
  if (dx_inline_object_array_initialize(&SELF->mesh_instances, 0)) {
    return DX_FAILURE;
  }
  if (dx_inline_object_array_initialize(&SELF->viewers, 0)) {
    dx_inline_object_array_uninitialize(&SELF->mesh_instances);
    return DX_FAILURE;
  }
  if (dx_inline_object_array_initialize(&SELF->asset_material_objects, 0)) {
    dx_inline_object_array_uninitialize(&SELF->viewers);
    dx_inline_object_array_uninitialize(&SELF->mesh_instances);
    return DX_FAILURE;
  }
  dx_size n;
  if (dx_inline_object_array_get_size(&n, &asset_scene->assets)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_object* asset_object = NULL;
    if(dx_inline_object_array_get_at(&asset_object, &asset_scene->assets, i)) {
      dx_inline_object_array_uninitialize(&SELF->asset_material_objects);
      dx_inline_object_array_uninitialize(&SELF->viewers);
      dx_inline_object_array_uninitialize(&SELF->mesh_instances);
      return DX_FAILURE;
    }
    if (on_scene_asset_object(SELF, context, asset_object)) {
      dx_inline_object_array_uninitialize(&SELF->asset_material_objects);
      dx_inline_object_array_uninitialize(&SELF->viewers);
      dx_inline_object_array_uninitialize(&SELF->mesh_instances);
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static void mesh_instance_on_shutdown(dx_default_scene_presenter* SELF) {
  dx_inline_object_array_uninitialize(&SELF->asset_material_objects);
  dx_inline_object_array_uninitialize(&SELF->viewers);
  dx_inline_object_array_uninitialize(&SELF->mesh_instances);
}

static dx_result viewer_push_constants(dx_default_scene_presenter* SELF, dx_val_cbinding* cbinding, dx_i32 canvas_width, dx_i32 canvas_height) {
  dx_size n;
  if (dx_inline_object_array_get_size(&n, &SELF->viewers)) {
    return DX_FAILURE;
  }
  if (n == 0) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return DX_FAILURE;
  }
  dx_val_viewer* viewer = NULL;
  if (dx_inline_object_array_get_at((dx_object**)&viewer, &SELF->viewers, n - 1)) {
    return DX_FAILURE;
  }
  DX_MAT4 a;
  if (dx_val_viewer_get_projection_matrix(&a, viewer, canvas_width, canvas_height)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &a);
  if (dx_val_viewer_get_view_matrix(&a, viewer, canvas_width, canvas_height)) {
    return DX_FAILURE;
  }
  dx_val_cbinding_set_mat4(cbinding, "matrices.view_matrix", &a);
  return DX_SUCCESS;
}

// We need to update all materials We need to make sure no material is updated twice.
static dx_result tick2(dx_default_scene_presenter* SELF, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  //
  {
    dx_size n;
    if (dx_inline_object_array_get_size(&n, &SELF->asset_material_objects)) {
      return DX_FAILURE;
    }
    for (dx_size i = 0; i < n; ++i) {
      dx_asset_material* asset_material = NULL;
      if (dx_inline_object_array_get_at((dx_object**)&asset_material, &SELF->asset_material_objects, i)) {
        return DX_FAILURE;
      }
      if (asset_material->controller) {
        if (dx_asset_material_controller_update(asset_material->controller, asset_material, delta_seconds)) {
          return DX_FAILURE;
        }
      }
    }
  }
  //
  dx_size n;
  if (dx_inline_object_array_get_size(&n, &SELF->mesh_instances)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_val_mesh_instance* mesh_instance = NULL;
    if (dx_inline_object_array_get_at((dx_object**)&mesh_instance, &SELF->mesh_instances, i)) {
      return DX_FAILURE;
    }
    DX_RGB_N8 a = DX_ASSETS_COLOR_RGB_N8(mesh_instance->mesh->material->asset_material->ambient_color->object)->value;
    dx_rgb_n8_to_rgba_f32(&a, 1.f, &mesh_instance->mesh->material->ambient_color);
  }
  return DX_SUCCESS;
}

static dx_result tick(dx_default_scene_presenter* SELF, dx_f32 delta_seconds) {
  dx_size n;
  if (dx_inline_object_array_get_size(&n, &SELF->viewers)) {
    return DX_FAILURE;
  }
  if (n == 0) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_val_viewer* val_viewer = NULL;
  if (dx_inline_object_array_get_at((dx_object**) &val_viewer, & SELF->viewers, n - 1)) {
    return DX_FAILURE;
  }
  dx_asset_viewer* asset_viewer = DX_ASSET_VIEWER(val_viewer->asset_viewer_instance->viewer_reference->object);
  if (asset_viewer->controller) {
    dx_asset_viewer_controller* asset_viewer_controller = asset_viewer->controller;
    if (dx_asset_viewer_controller_update(asset_viewer_controller, asset_viewer, delta_seconds)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static dx_result update_viewer(dx_default_scene_presenter* SELF, dx_i32 canvas_width, dx_i32 canvas_height) {
  dx_size n;
  if (dx_inline_object_array_get_size(&n, &SELF->viewers)) {
    return DX_FAILURE;
  }
  dx_val_viewer* val_viewer = NULL;
  if (dx_inline_object_array_get_at((dx_object**)&val_viewer, &SELF->viewers, n - 1)) {
    return DX_FAILURE;
  }
  dx_asset_viewer* asset_viewer = DX_ASSET_VIEWER(val_viewer->asset_viewer_instance->viewer_reference->object);
  if (asset_viewer) {
    val_viewer->up = asset_viewer->up;
    val_viewer->target = asset_viewer->target;
    val_viewer->source = asset_viewer->source;
  }
  return DX_SUCCESS;
}

static dx_result make_commands_1(dx_val_command_list* commands) {
  dx_val_command* command = NULL;

  // clear color command
  DX_RGBA_F32 clear_color;
  dx_rgb_n8_to_rgba_f32(&dx_colors_capri, 0.f, &clear_color);
  if (dx_val_command_create_clear_color(&command, 0, 0, 640, 480, &clear_color)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // clear depth command
  static dx_f32 const clear_depth = 1.f;
  if (dx_val_command_create_clear_depth(&command, 0, 0, 640, 480, clear_depth)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // set viewport command
  if (dx_val_command_create_viewport(&command, 0, 0, 640, 480)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, dx_cull_mode_back, dx_depth_test_function_less_than, DX_TRUE)) {
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(commands, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  return DX_SUCCESS;
}

static dx_result dx_default_scene_presenter_startup(dx_default_scene_presenter* SELF, dx_val_context* context) {
  {
    SELF->asset_scene = _create_scene_from_file(SELF->path);
  }
  if (!SELF->asset_scene) {
    return DX_FAILURE;
  }
  //
  if (mesh_instance_on_startup(SELF, context, SELF->asset_scene)) {
    DX_UNREFERENCE(SELF->asset_scene);
    SELF->asset_scene = NULL;
    return DX_FAILURE;
  }
  //
  update_viewer(SELF, 640, 480);
  //
  {
    dx_val_command_list* commands = NULL;
    if (dx_val_command_list_create(&commands)) {
      mesh_instance_on_shutdown(SELF);
      DX_UNREFERENCE(SELF->asset_scene);
      SELF->asset_scene = NULL;
      return DX_FAILURE;
    }
    if (make_commands_1(commands)) {
      DX_UNREFERENCE(commands);
      commands = NULL;
      mesh_instance_on_shutdown(SELF);
      DX_UNREFERENCE(SELF->asset_scene);
      SELF->asset_scene = NULL;
      return DX_FAILURE;
    }
    SELF->commands = commands;
  }
  //
  return DX_SUCCESS;
}

static dx_result dx_default_scene_presenter_render(dx_default_scene_presenter* SELF, dx_val_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height) {
  update_viewer(SELF, canvas_width, canvas_height);
  tick2(SELF, delta_seconds, canvas_width, canvas_height);
  tick(SELF, delta_seconds);
  {
    dx_val_command* command;

    // clear color command
    command = dx_val_command_list_get_at(SELF->commands, 0);
    if (command->kind != DX_VAL_COMMAND_KIND_CLEAR_COLOR) {
      return DX_FAILURE;
    }
    command->clear_color_command.rectangle.w = (dx_f32)canvas_width;
    command->clear_color_command.rectangle.h = (dx_f32)canvas_height;

    // clear depth command
    command = dx_val_command_list_get_at(SELF->commands, 1);
    if (command->kind != DX_VAL_COMMAND_KIND_CLEAR_DEPTH) {
      return DX_FAILURE;
    }
    command->clear_depth_command.rectangle.w = (dx_f32)canvas_width;
    command->clear_depth_command.rectangle.h = (dx_f32)canvas_height;

    // viewport command
    command = dx_val_command_list_get_at(SELF->commands, 2);
    if (command->kind != DX_VAL_COMMAND_KIND_VIEWPORT) {
      return DX_FAILURE;
    }
    command->viewport_command.w = (dx_f32)canvas_width;
    command->viewport_command.h = (dx_f32)canvas_height;
  }
  // the "on enter frame" commands.
  if (dx_val_context_execute_commands(context, SELF->commands)) {
    return DX_FAILURE;
  }
  // the "per mesh instance" commands.
  {
    dx_size n;
    if (dx_inline_object_array_get_size(&n, &SELF->mesh_instances)) {
      return DX_FAILURE;
    }
    for (dx_size i = 0; i < n; ++i) {
      dx_val_mesh_instance* mesh_instance = NULL;
      if (dx_inline_object_array_get_at((dx_object**) & mesh_instance, &SELF->mesh_instances, i)) {
        return DX_FAILURE;
      }
      // update the constant binding
      dx_val_cbinding* cbinding = dx_val_mesh_instance_get_cbinding(mesh_instance);
      dx_val_mesh_instance_update_cbinding(mesh_instance, cbinding);
      viewer_push_constants(SELF, cbinding, canvas_width, canvas_height);
      if (dx_val_context_execute_commands(context, mesh_instance->commands)) {
        return DX_FAILURE;
      }
    }
  }
  return DX_SUCCESS;
}

static dx_result dx_default_scene_presenter_shutdown(dx_default_scene_presenter* SELF, dx_val_context* context) {
  mesh_instance_on_shutdown(SELF);
  if (SELF->asset_scene) {
    DX_UNREFERENCE(SELF->asset_scene);
    SELF->asset_scene = NULL;
  }
  if (SELF->commands) {
    DX_UNREFERENCE(SELF->commands);
    SELF->commands = NULL;
  }
  return DX_SUCCESS;
}

static void dx_default_scene_presenter_destruct(dx_default_scene_presenter* SELF) {
  if (SELF->path) {
    DX_UNREFERENCE(SELF->path);
    SELF->path = NULL;
  }
}

static void dx_default_scene_presenter_dispatch_construct(dx_default_scene_presenter_dispatch* SELF) {
  DX_SCENE_PRESENTER_DISPATCH(SELF)->startup = (dx_result (*)(dx_scene_presenter*, dx_val_context*)) & dx_default_scene_presenter_startup;
  DX_SCENE_PRESENTER_DISPATCH(SELF)->render = (dx_result(*)(dx_scene_presenter*, dx_val_context*, dx_f32, dx_i32, dx_i32)) & dx_default_scene_presenter_render;
  DX_SCENE_PRESENTER_DISPATCH(SELF)->shutdown = (dx_result(*)(dx_scene_presenter*, dx_val_context*)) dx_default_scene_presenter_shutdown;
}

dx_result dx_default_scene_presenter_construct(dx_default_scene_presenter* SELF, dx_string* path) {
  dx_rti_type* TYPE = dx_default_scene_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (dx_scene_presenter_construct(DX_SCENE_PRESENTER(SELF))) {
    return DX_FAILURE;
  }
  
  if (!path) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_REFERENCE(path);
  SELF->path = path;

  SELF->asset_scene = NULL;
  SELF->commands = NULL;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_default_scene_presenter_create(dx_default_scene_presenter** RETURN, dx_string* path) {
  DX_CREATE_PREFIX(dx_default_scene_presenter)
  if (dx_default_scene_presenter_construct(SELF, path)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
