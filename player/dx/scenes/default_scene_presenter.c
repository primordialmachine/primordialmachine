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

Core_defineObjectType("dx.default_scene_presenter",
                      dx_default_scene_presenter,
                      dx_scene_presenter);

static Core_Result on_scene_asset_object(dx_default_scene_presenter* SELF, dx_val_context* context, Core_Object* asset_object) {
  Core_Type* type = NULL;
  Core_Boolean result = Core_False;
  // mesh instance
  if (dx_asset_mesh_instance_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, asset_object->type, type)) {
    return Core_Failure;
  }
  if (result) {
    dx_asset_mesh_instance* asset_mesh_instance = DX_ASSET_MESH_INSTANCE(asset_object);
    dx_val_mesh* mesh = NULL;
    if (dx_val_mesh_create(&mesh, context, DX_ASSETS_MESH(asset_mesh_instance->mesh_reference->object))) {
      return Core_Failure;
    }
    dx_val_mesh_instance* mesh_instance = NULL;
    if (dx_val_mesh_instance_create(&mesh_instance, asset_mesh_instance->world_matrix->value, mesh)) {
      CORE_UNREFERENCE(mesh);
      mesh = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(mesh);
    mesh = NULL;
    if (dx_inline_object_array_append(&SELF->mesh_instances, CORE_OBJECT(mesh_instance))) {
      CORE_UNREFERENCE(mesh_instance);
      mesh_instance = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(mesh_instance);
    mesh_instance = NULL;
    return Core_Error_NoError != Core_getError() ? Core_Failure : Core_Success;
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  // material
  if (dx_assets_material_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, asset_object->type, type)) {
    return Core_Failure;
  }
  if (result) {
    dx_assets_material* material_asset = DX_ASSETS_MATERIAL(asset_object);
    if (dx_inline_object_array_append(&SELF->material_assets, CORE_OBJECT(material_asset))) {
      CORE_UNREFERENCE(material_asset);
      material_asset = NULL;
      return Core_Failure;
    }
    return Core_Error_NoError != Core_getError() ? Core_Failure : Core_Success;
  }
  // viewer instance
  if (dx_assets_viewer_instance_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, asset_object->type, type)) {
    return Core_Failure;
  }
  if (result) {
    dx_assets_viewer_instance* viewer_instance_asset = DX_ASSETS_VIEWER_INSTANCE(asset_object);
    dx_val_viewer* viewer;
    if (dx_val_viewer_create(&viewer, viewer_instance_asset)) {
      return Core_Failure;
    }
    if (dx_inline_object_array_append(&SELF->viewers, CORE_OBJECT(viewer))) {
      CORE_UNREFERENCE(viewer);
      viewer = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(viewer);
    viewer = NULL;
    return Core_Error_NoError != Core_getError() ? Core_Failure : Core_Success;
  }
  if (Core_getError()) {
    return Core_Failure;
  }
  dx_log("ignoring unknown/unsupported scene object\n", sizeof("ignoring unknown/unsupported scene object\n") - 1);
  return Core_Success;
}

static Core_Result mesh_instance_on_startup(dx_default_scene_presenter* SELF, dx_val_context* context, dx_assets_scene* asset_scene) {
  if (dx_inline_object_array_initialize(&SELF->mesh_instances, 0)) {
    return Core_Failure;
  }
  if (dx_inline_object_array_initialize(&SELF->viewers, 0)) {
    dx_inline_object_array_uninitialize(&SELF->mesh_instances);
    return Core_Failure;
  }
  if (dx_inline_object_array_initialize(&SELF->material_assets, 0)) {
    dx_inline_object_array_uninitialize(&SELF->viewers);
    dx_inline_object_array_uninitialize(&SELF->mesh_instances);
    return Core_Failure;
  }
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, &asset_scene->assets)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    Core_Object* asset_object = NULL;
    if(dx_inline_object_array_get_at(&asset_object, &asset_scene->assets, i)) {
      dx_inline_object_array_uninitialize(&SELF->material_assets);
      dx_inline_object_array_uninitialize(&SELF->viewers);
      dx_inline_object_array_uninitialize(&SELF->mesh_instances);
      return Core_Failure;
    }
    if (on_scene_asset_object(SELF, context, asset_object)) {
      dx_inline_object_array_uninitialize(&SELF->material_assets);
      dx_inline_object_array_uninitialize(&SELF->viewers);
      dx_inline_object_array_uninitialize(&SELF->mesh_instances);
      return Core_Failure;
    }
  }
  return Core_Success;
}

static void mesh_instance_on_shutdown(dx_default_scene_presenter* SELF) {
  dx_inline_object_array_uninitialize(&SELF->material_assets);
  dx_inline_object_array_uninitialize(&SELF->viewers);
  dx_inline_object_array_uninitialize(&SELF->mesh_instances);
}

static Core_Result viewer_push_constants(dx_default_scene_presenter* SELF, dx_val_cbinding* cbinding, Core_Integer32 canvas_width, Core_Integer32 canvas_height) {
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, &SELF->viewers)) {
    return Core_Failure;
  }
  if (n == 0) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  dx_val_viewer* viewer = NULL;
  if (dx_inline_object_array_get_at((Core_Object**)&viewer, &SELF->viewers, n - 1)) {
    return Core_Failure;
  }
  DX_MAT4 a;
  if (dx_val_viewer_get_projection_matrix(&a, viewer, canvas_width, canvas_height)) {
    return Core_Failure;
  }
  dx_val_cbinding_set_mat4(cbinding, "matrices.projection_matrix", &a);
  if (dx_val_viewer_get_view_matrix(&a, viewer, canvas_width, canvas_height)) {
    return Core_Failure;
  }
  dx_val_cbinding_set_mat4(cbinding, "matrices.view_matrix", &a);
  return Core_Success;
}

// We need to update all materials We need to make sure no material is updated twice.
static Core_Result tick2(dx_default_scene_presenter* SELF, Core_Real32 delta_seconds, Core_Integer32 canvas_width, Core_Integer32 canvas_height) {
  //
  {
    Core_Size n;
    if (dx_inline_object_array_get_size(&n, &SELF->material_assets)) {
      return Core_Failure;
    }
    for (Core_Size i = 0; i < n; ++i) {
      dx_assets_material* material_asset = NULL;
      if (dx_inline_object_array_get_at((Core_Object**)&material_asset, &SELF->material_assets, i)) {
        return Core_Failure;
      }
      if (material_asset->controller) {
        if (dx_assets_material_controller_update(material_asset->controller, material_asset, delta_seconds)) {
          return Core_Failure;
        }
      }
    }
  }
  //
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, &SELF->mesh_instances)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_val_mesh_instance* mesh_instance = NULL;
    if (dx_inline_object_array_get_at((Core_Object**)&mesh_instance, &SELF->mesh_instances, i)) {
      return Core_Failure;
    }
    DX_RGB_N8 a = DX_ASSETS_COLOR_RGB_N8(mesh_instance->mesh->material->material_asset->ambient_color->object)->value;
    dx_rgb_n8_to_rgba_f32(&a, 1.f, &mesh_instance->mesh->material->ambient_color);
  }
  return Core_Success;
}

static Core_Result tick(dx_default_scene_presenter* SELF, Core_Real32 delta_seconds) {
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, &SELF->viewers)) {
    return Core_Failure;
  }
  if (n == 0) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  dx_val_viewer* val_viewer = NULL;
  if (dx_inline_object_array_get_at((Core_Object**) &val_viewer, & SELF->viewers, n - 1)) {
    return Core_Failure;
  }
  dx_assets_viewer* viewer_asset = DX_ASSETS_VIEWER(val_viewer->asset_viewer_instance->viewer_reference->object);
  if (viewer_asset->controller) {
    dx_assets_viewer_controller* viewer_controller_asset = viewer_asset->controller;
    if (dx_assets_viewer_controller_update(viewer_controller_asset, viewer_asset, delta_seconds)) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

static Core_Result update_viewer(dx_default_scene_presenter* SELF, Core_Integer32 canvas_width, Core_Integer32 canvas_height) {
  Core_Size n;
  if (dx_inline_object_array_get_size(&n, &SELF->viewers)) {
    return Core_Failure;
  }
  dx_val_viewer* val_viewer = NULL;
  if (dx_inline_object_array_get_at((Core_Object**)&val_viewer, &SELF->viewers, n - 1)) {
    return Core_Failure;
  }
  dx_assets_viewer* viewer_asset = DX_ASSETS_VIEWER(val_viewer->asset_viewer_instance->viewer_reference->object);
  if (viewer_asset) {
    val_viewer->up = viewer_asset->up;
    val_viewer->target = viewer_asset->target;
    val_viewer->source = viewer_asset->source;
  }
  return Core_Success;
}

static Core_Result make_commands_1(dx_val_command_list* commands) {
  dx_val_command* command = NULL;

  // clear color command
  DX_RGBA_F32 clear_color;
  dx_rgb_n8_to_rgba_f32(&dx_colors_capri, 0.f, &clear_color);
  if (dx_val_command_create_clear_color(&command, 0, 0, 640, 480, &clear_color)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(commands, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // clear depth command
  static Core_Real32 const clear_depth = 1.f;
  if (dx_val_command_create_clear_depth(&command, 0, 0, 640, 480, clear_depth)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(commands, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // set viewport command
  if (dx_val_command_create_viewport(&command, 0, 0, 640, 480)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(commands, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  // pipeline state command
  if (dx_val_command_create_pipeline_state(&command, Core_CullMode_Back, Core_DepthCompareFunction_LessThan, Core_True)) {
    return Core_Failure;
  }
  if (dx_val_command_list_append(commands, command)) {
    CORE_UNREFERENCE(command);
    command = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(command);
  command = NULL;

  return Core_Success;
}

static Core_Result dx_default_scene_presenter_startup(dx_default_scene_presenter* SELF, dx_val_context* context) {
  if (_create_scene_from_file(&SELF->asset_scene, SELF->path)) {
    return Core_Failure;
  }
  //
  if (mesh_instance_on_startup(SELF, context, SELF->asset_scene)) {
    CORE_UNREFERENCE(SELF->asset_scene);
    SELF->asset_scene = NULL;
    return Core_Failure;
  }
  //
  update_viewer(SELF, 640, 480);
  //
  {
    dx_val_command_list* commands = NULL;
    if (dx_val_command_list_create(&commands)) {
      mesh_instance_on_shutdown(SELF);
      CORE_UNREFERENCE(SELF->asset_scene);
      SELF->asset_scene = NULL;
      return Core_Failure;
    }
    if (make_commands_1(commands)) {
      CORE_UNREFERENCE(commands);
      commands = NULL;
      mesh_instance_on_shutdown(SELF);
      CORE_UNREFERENCE(SELF->asset_scene);
      SELF->asset_scene = NULL;
      return Core_Failure;
    }
    SELF->commands = commands;
  }
  //
  return Core_Success;
}

static Core_Result dx_default_scene_presenter_render(dx_default_scene_presenter* SELF, dx_val_context* context, Core_Real32 delta_seconds, Core_Integer32 canvas_width, Core_Integer32 canvas_height) {
  update_viewer(SELF, canvas_width, canvas_height);
  tick2(SELF, delta_seconds, canvas_width, canvas_height);
  tick(SELF, delta_seconds);
  {
    dx_val_command* command;

    // clear color command
    command = dx_val_command_list_get_at(SELF->commands, 0);
    if (command->kind != DX_VAL_COMMAND_KIND_CLEAR_COLOR) {
      return Core_Failure;
    }
    command->clear_color_command.rectangle.w = (Core_Real32)canvas_width;
    command->clear_color_command.rectangle.h = (Core_Real32)canvas_height;

    // clear depth command
    command = dx_val_command_list_get_at(SELF->commands, 1);
    if (command->kind != DX_VAL_COMMAND_KIND_CLEAR_DEPTH) {
      return Core_Failure;
    }
    command->clear_depth_command.rectangle.w = (Core_Real32)canvas_width;
    command->clear_depth_command.rectangle.h = (Core_Real32)canvas_height;

    // viewport command
    command = dx_val_command_list_get_at(SELF->commands, 2);
    if (command->kind != DX_VAL_COMMAND_KIND_VIEWPORT) {
      return Core_Failure;
    }
    command->viewport_command.w = (Core_Real32)canvas_width;
    command->viewport_command.h = (Core_Real32)canvas_height;
  }
  // the "on enter frame" commands.
  if (dx_val_context_execute_commands(context, SELF->commands)) {
    return Core_Failure;
  }
  // the "per mesh instance" commands.
  {
    Core_Size n;
    if (dx_inline_object_array_get_size(&n, &SELF->mesh_instances)) {
      return Core_Failure;
    }
    for (Core_Size i = 0; i < n; ++i) {
      dx_val_mesh_instance* mesh_instance = NULL;
      if (dx_inline_object_array_get_at((Core_Object**) & mesh_instance, &SELF->mesh_instances, i)) {
        return Core_Failure;
      }
      // update the constant binding
      dx_val_cbinding* cbinding = dx_val_mesh_instance_get_cbinding(mesh_instance);
      dx_val_mesh_instance_update_cbinding(mesh_instance, cbinding);
      viewer_push_constants(SELF, cbinding, canvas_width, canvas_height);
      if (dx_val_context_execute_commands(context, mesh_instance->commands)) {
        return Core_Failure;
      }
    }
  }
  return Core_Success;
}

static Core_Result dx_default_scene_presenter_shutdown(dx_default_scene_presenter* SELF, dx_val_context* context) {
  mesh_instance_on_shutdown(SELF);
  if (SELF->asset_scene) {
    CORE_UNREFERENCE(SELF->asset_scene);
    SELF->asset_scene = NULL;
  }
  if (SELF->commands) {
    CORE_UNREFERENCE(SELF->commands);
    SELF->commands = NULL;
  }
  return Core_Success;
}

static void dx_default_scene_presenter_destruct(dx_default_scene_presenter* SELF) {
  if (SELF->path) {
    CORE_UNREFERENCE(SELF->path);
    SELF->path = NULL;
  }
}

static void dx_default_scene_presenter_constructDispatch(dx_default_scene_presenter_Dispatch* SELF) {
  DX_SCENE_PRESENTER_DISPATCH(SELF)->startup = (Core_Result (*)(dx_scene_presenter*, dx_val_context*)) & dx_default_scene_presenter_startup;
  DX_SCENE_PRESENTER_DISPATCH(SELF)->render = (Core_Result(*)(dx_scene_presenter*, dx_val_context*, Core_Real32, Core_Integer32, Core_Integer32)) & dx_default_scene_presenter_render;
  DX_SCENE_PRESENTER_DISPATCH(SELF)->shutdown = (Core_Result(*)(dx_scene_presenter*, dx_val_context*)) dx_default_scene_presenter_shutdown;
}

Core_Result dx_default_scene_presenter_construct(dx_default_scene_presenter* SELF, Core_String* path) {
  DX_CONSTRUCT_PREFIX(dx_default_scene_presenter);
  if (dx_scene_presenter_construct(DX_SCENE_PRESENTER(SELF))) {
    return Core_Failure;
  }
  
  if (!path) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  CORE_REFERENCE(path);
  SELF->path = path;

  SELF->asset_scene = NULL;
  SELF->commands = NULL;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_default_scene_presenter_create(dx_default_scene_presenter** RETURN, Core_String* path) {
  DX_CREATE_PREFIX(dx_default_scene_presenter);
  if (dx_default_scene_presenter_construct(SELF, path)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
