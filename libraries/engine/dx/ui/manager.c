#include "dx/ui/manager.h"

#include "dx/val/command.h"
#include "dx/ui/group.h"

DX_DEFINE_OBJECT_TYPE("dx.ui.manager",
                      dx_ui_manager,
                      dx_object);

static dx_result on_render_group_children(dx_ui_manager* SELF, dx_ui_group* group);

static dx_result on_render_widget(dx_ui_manager* SELF, dx_ui_widget* widget);

static void dx_ui_manager_destruct(dx_ui_manager* SELF) {
  DX_UNREFERENCE(SELF->command_list);
  SELF->command_list = NULL;
  DX_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;
  DX_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;
}

static void dx_ui_manager_dispatch_construct(dx_ui_manager_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_ui_manager_construct(dx_ui_manager* SELF, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  dx_rti_type* TYPE = dx_ui_manager_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (dx_val_command_list_create(&SELF->command_list)) {
    return DX_FAILURE;
  }

  // create the set viewport command
  dx_val_command* command = NULL;
  if (dx_val_command_create_viewport(&command, 0.f, 0.f, 640.f, 480.f)) {
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return DX_FAILURE;
  }
  if (dx_val_command_list_append(SELF->command_list, command)) {
    DX_UNREFERENCE(command);
    command = NULL;
    DX_UNREFERENCE(SELF->command_list);
    SELF->command_list = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(command);
  command = NULL;

  SELF->font_presenter = font_presenter;
  DX_REFERENCE(font_presenter);

  SELF->rectangle_presenter = rectangle_presenter;
  DX_REFERENCE(rectangle_presenter);
  
  dx_vec2_f32_set(&SELF->dpi, 96.f, 96.f);
  
  dx_vec2_f32_set(&SELF->resolution, 640, 480);
  
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_create(dx_ui_manager** RETURN, dx_font_presenter* font_presenter, dx_rectangle_presenter* rectangle_presenter) {
  DX_CREATE_PREFIX(dx_ui_manager)
  if (dx_ui_manager_construct(SELF, font_presenter, rectangle_presenter)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_set_canvas_dpi(dx_ui_manager* SELF, DX_VEC2_F32 const* dpi) {
  SELF->dpi = *dpi;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_set_canvas_resolution(dx_ui_manager* SELF, DX_VEC2_F32 const* resolution) {
  SELF->resolution = *resolution;
  return DX_SUCCESS;
}

dx_result dx_ui_manager_set_root(dx_ui_manager* SELF, dx_ui_widget* root) {
  SELF->root = root;
  return DX_SUCCESS;
}

static dx_result on_render_group_children(dx_ui_manager* SELF, dx_ui_group* group) {
  dx_size n;
  if (dx_object_array_get_size(&n, group->children)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ui_widget* widget = NULL;
    if (dx_object_array_get_at((dx_object**)&widget, group->children, i)) {
      return DX_FAILURE;
    }
    if (on_render_widget(SELF, widget)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static dx_result on_render_widget(dx_ui_manager* SELF, dx_ui_widget* widget) {
  if (dx_ui_widget_render(widget, SELF->resolution.e[0], SELF->resolution.e[1], SELF->dpi.e[0], SELF->dpi.e[1])) {
    return DX_FAILURE;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(widget)->type, dx_ui_group_get_type())) {
    if (on_render_group_children(SELF, DX_UI_GROUP(widget))) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

dx_result dx_ui_manager_enter_render(dx_ui_manager* SELF) {
  dx_val_command* command = NULL;

  // Update the viewport command.
  command = dx_val_command_list_get_at(SELF->command_list, 0);
  command->viewport_command.w = SELF->resolution.e[0];
  command->viewport_command.h = SELF->resolution.e[1];

  // Execute the commands.
  if (dx_val_context_execute_commands(DX_PRESENTER(SELF->font_presenter)->val_context, SELF->command_list)) {
    return DX_FAILURE;
  }

  // Update the world, view, and projection matrices.
  DX_MAT4 world_matrix;
  dx_mat4_set_identity(&world_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
  
  DX_MAT4 view_matrix;
  dx_mat4_set_identity(&view_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);

  DX_MAT4 projection_matrix;
  dx_mat4_set_ortho(&projection_matrix, 0.f, SELF->resolution.e[0], 0, SELF->resolution.e[1], -1, +1);
  dx_val_cbinding_set_mat4(SELF->font_presenter->rectangle_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);
  dx_val_cbinding_set_mat4(SELF->font_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);

  return DX_SUCCESS;
}

dx_result dx_ui_manager_leave_render(dx_ui_manager* SELF) {
  if (SELF->root) {
    return on_render_widget(SELF, SELF->root);
  }
  return DX_SUCCESS;
}
