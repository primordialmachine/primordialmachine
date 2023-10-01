#include "dx/ui/scrollbar.h"

#include "dx/rectangle_presenter.h"
#include "dx/ui/manager.h"
#include "dx/val/cbinding.h"

DX_DEFINE_OBJECT_TYPE("dx.ui.scrollbar",
                       dx_ui_scrollbar,
                       dx_ui_widget);

static dx_result set_relative_position(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_position);

static dx_result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static dx_result set_relative_size(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_size);

static dx_result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static dx_result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static dx_result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static dx_result get_child_by_name(dx_ui_widget** RETURN, dx_ui_scrollbar* SELF, dx_string* name);

static dx_result render(dx_ui_scrollbar* SELF, dx_f32 canvas_horizontal_size, dx_f32 canvas_vertical_size, dx_f32 dpi_horizontal, dx_f32 dpi_vertical);

static void dx_ui_scrollbar_destruct(dx_ui_scrollbar* SELF)
{/*Intentionally empty.*/}

static void dx_ui_scrollbar_dispatch_construct(dx_ui_scrollbar_dispatch* SELF) {
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_position = (dx_result(*)(DX_VEC2_F32*,dx_ui_widget*)) & get_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_size = (dx_result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->render = (dx_result(*)(dx_ui_widget*,dx_f32,dx_f32,dx_f32,dx_f32)) & render;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_position = (dx_result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_size = (dx_result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_position = (dx_result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_size = (dx_result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_child_by_name = (dx_result(*)(dx_ui_widget**, dx_ui_widget*, dx_string*)) & get_child_by_name;
}

dx_result dx_ui_scrollbar_construct(dx_ui_scrollbar* SELF, dx_ui_manager* manager) {
  dx_rti_type* TYPE = dx_ui_scrollbar_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_ui_widget_construct(DX_UI_WIDGET(SELF), manager)) {
    return DX_FAILURE;
  }
  dx_vec2_f32_set(&SELF->relative_position, 0.f, 0.f);
  dx_vec2_f32_set(&SELF->relative_size, 0.f, 0.f);
  dx_rgba_f32_set(&SELF->background_color, 1.f, 1.f, 1.f, 1.f);
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ui_scrollbar_create(dx_ui_scrollbar** RETURN, dx_ui_manager* manager) {
  DX_CREATE_PREFIX(dx_ui_scrollbar);
  if (dx_ui_scrollbar_construct(SELF, manager)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result set_relative_position(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_position) {
  if (!SELF || !relative_position) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->relative_position = *relative_position;
  return DX_SUCCESS;
}

static dx_result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->relative_position;
  return DX_SUCCESS;
}

static dx_result set_relative_size(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_size) {
  if (!SELF || !relative_size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->relative_size = *relative_size;
  return DX_SUCCESS;
}

static dx_result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->relative_size;
  return DX_SUCCESS;
}

static dx_result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_VEC2_F32 a;
  if (dx_ui_widget_get_relative_position(&a, DX_UI_WIDGET(SELF))) {
    return DX_FAILURE;
  }
  if (DX_UI_WIDGET(SELF)->parent) {
    DX_VEC2_F32 b;
    if (dx_ui_widget_get_absolute_position(&b, DX_UI_WIDGET(SELF)->parent)) {
      return DX_FAILURE;
    }
    dx_vec2_f32_add3(&a, &a, &b);
  }
  *RETURN = a;
  return DX_SUCCESS;
}

static dx_result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_VEC2_F32 a;
  if (dx_ui_widget_get_relative_size(&a, DX_UI_WIDGET(SELF))) {
    return DX_FAILURE;
  }
  *RETURN = a;
  return DX_SUCCESS;
}

static dx_result get_child_by_name(dx_ui_widget** RETURN, dx_ui_scrollbar* SELF, dx_string* name) {
  // Scrollbar has no publicly available children!
  dx_set_error(DX_ERROR_NOT_FOUND);
  return DX_FAILURE;
}

dx_result dx_ui_scrollbar_set_background_color(dx_ui_scrollbar* SELF, DX_RGBA_F32 const* background_color) {
  if (!SELF || !background_color) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->background_color = *background_color;
  return DX_SUCCESS;
}

dx_result dx_ui_scrollbar_get_background_color(DX_RGBA_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->background_color;
  return DX_SUCCESS;
}

static dx_result render(dx_ui_scrollbar* SELF, dx_f32 canvas_horizontal_size, dx_f32 canvas_vertical_size, dx_f32 dpi_horizontal, dx_f32 dpi_vertical) {
  DX_MAT4 world_matrix;
  dx_mat4_set_identity(&world_matrix);
  dx_val_cbinding_set_mat4(DX_UI_WIDGET(SELF)->manager->rectangle_presenter->val_cbinding, "vs_matrices.world_matrix", &world_matrix);
  
  DX_MAT4 view_matrix;
  dx_mat4_set_identity(&view_matrix);
  dx_val_cbinding_set_mat4(DX_UI_WIDGET(SELF)->manager->rectangle_presenter->val_cbinding, "vs_matrices.view_matrix", &view_matrix);
  
  DX_MAT4 projection_matrix;
  dx_mat4_set_ortho(&projection_matrix, 0, canvas_horizontal_size, 0, canvas_vertical_size, -1, +1);
  dx_val_cbinding_set_mat4(DX_UI_WIDGET(SELF)->manager->rectangle_presenter->val_cbinding, "vs_matrices.projection_matrix", &projection_matrix);

  DX_RECT2_F32 target_rectangle;
  dx_rect2_f32_set(&target_rectangle, SELF->relative_position.e[0],
                                      SELF->relative_position.e[1],
                                      SELF->relative_position.e[0] + SELF->relative_size.e[0],
                                      SELF->relative_position.e[1] + SELF->relative_size.e[1]);
  if (dx_rectangle_presenter_fill_rectangle(DX_UI_WIDGET(SELF)->manager->rectangle_presenter,
                                            &target_rectangle,
                                            0.f,
                                            &SELF->background_color)) {
    return DX_FAILURE;
  }
  
  return DX_SUCCESS;
}

#if 0
dx_result dx_ui_scrollbar_append_child(dx_ui_scrollbar* SELF, dx_ui_widget* child) {
  if (!SELF || !child) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_object_array_append(SELF->children, DX_OBJECT(child));
}
#endif

#if 0
dx_result dx_ui_scrollbar_prepend_child(dx_ui_scrollbar* SELF, dx_ui_widget* child) {
  if (!SELF || !child) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_object_array_prepend(SELF->children, DX_OBJECT(child));
}
#endif

#if 0
dx_result dx_ui_scrollbar_insert_child(dx_ui_scrollbar* SELF, dx_ui_widget* child, dx_size index) {
  if (!SELF || !child) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_object_array_insert(SELF->children, DX_OBJECT(child), index);
}
#endif
