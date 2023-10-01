#include "dx/ui/widget.h"

DX_DEFINE_OBJECT_TYPE("dx.ui.widget",
                      dx_ui_widget,
                      dx_object);

static dx_result get_relative_rectangle(DX_RECT2_F32* RETURN, dx_ui_widget* SELF) {
  DX_VEC2_F32 position;
  if (dx_ui_widget_get_relative_position(&position, SELF)) {
    return DX_FAILURE;
  }
  DX_VEC2_F32 size;
  if (dx_ui_widget_get_relative_size(&size, SELF)) {
    return DX_FAILURE;
  }
  DX_RECT2_F32 rectangle;
  dx_rect2_f32_set(&rectangle, position.e[0],
                               position.e[1],
                               position.e[0] + size.e[0],
                               position.e[1] + size.e[1]);
  *RETURN = rectangle;
  return DX_SUCCESS;
}

static dx_result get_absolute_rectangle(DX_RECT2_F32* RETURN, dx_ui_widget* SELF) {
  DX_RECT2_F32 rectangle;
  if (dx_ui_widget_get_relative_rectangle(&rectangle, SELF)) {
    return DX_FAILURE;
  }
  if (DX_UI_WIDGET(SELF)->parent) {
    DX_VEC2_F32 t;
    if (dx_ui_widget_get_absolute_position(&t, DX_UI_WIDGET(SELF)->parent)) {
      return DX_FAILURE;
    }
    dx_rect2_f32_translate(&rectangle, &t);
  }
  *RETURN = rectangle;
  return DX_SUCCESS;
}


static dx_result get_name(dx_string** RETURN, dx_ui_widget* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (SELF->name) {
    DX_REFERENCE(SELF->name);
  }
  *RETURN = SELF->name;
  return DX_SUCCESS;
}

static dx_result set_name(dx_ui_widget* SELF, dx_string* name) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (name) {
    DX_REFERENCE(name);
  }
  if (SELF->name) {
    DX_UNREFERENCE(SELF->name);
  }
  SELF->name = name;
  return DX_SUCCESS;
}

static void dx_ui_widget_destruct(dx_ui_widget* SELF) {
  if (SELF->name) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  DX_UNREFERENCE(SELF->manager);
  SELF->manager = NULL;
  SELF->parent = NULL;
}

static void dx_ui_widget_dispatch_construct(dx_ui_widget_dispatch* SELF) {
  SELF->set_name = &set_name;
  SELF->get_name = &get_name;
  SELF->get_relative_rectangle = &get_relative_rectangle;
  SELF->get_absolute_rectangle = &get_absolute_rectangle;
}

dx_result dx_ui_widget_construct(dx_ui_widget* SELF, dx_ui_manager* manager) {
  dx_rti_type* TYPE = dx_ui_widget_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->parent = NULL;
  if (!manager) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->manager = manager;
  DX_REFERENCE(manager);
  SELF->name = NULL;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
