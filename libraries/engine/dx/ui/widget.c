#include "dx/ui/widget.h"

DX_DEFINE_OBJECT_TYPE("dx.ui.widget",
                      dx_ui_widget,
                      dx_object);

static void dx_ui_widget_destruct(dx_ui_widget* SELF) {
  DX_UNREFERENCE(SELF->manager);
  SELF->manager = NULL;
  SELF->parent = NULL;
}

static void dx_ui_widget_dispatch_construct(dx_ui_widget_dispatch* SELF)
{/*Intentionally empty.*/}

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
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
