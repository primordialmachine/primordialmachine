#include "dx/ui/widget.h"

Core_defineObjectType("dx.ui.widget",
                      dx_ui_widget,
                      Core_Object);

static Core_Result get_relative_rectangle(DX_RECT2_F32* RETURN, dx_ui_widget* SELF) {
  Core_InlineVector2R32 position;
  if (dx_ui_widget_get_relative_position(&position, SELF)) {
    return Core_Failure;
  }
  Core_InlineVector2R32 size;
  if (dx_ui_widget_get_relative_size(&size, SELF)) {
    return Core_Failure;
  }
  DX_RECT2_F32 rectangle;
  dx_rect2_f32_set(&rectangle, position.e[0],
                               position.e[1],
                               position.e[0] + size.e[0],
                               position.e[1] + size.e[1]);
  *RETURN = rectangle;
  return Core_Success;
}

static Core_Result get_absolute_rectangle(DX_RECT2_F32* RETURN, dx_ui_widget* SELF) {
  DX_RECT2_F32 rectangle;
  if (dx_ui_widget_get_relative_rectangle(&rectangle, SELF)) {
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF)->parent) {
    Core_InlineVector2R32 t;
    if (dx_ui_widget_get_absolute_position(&t, DX_UI_WIDGET(SELF)->parent)) {
      return Core_Failure;
    }
    dx_rect2_f32_translate(&rectangle, &t);
  }
  *RETURN = rectangle;
  return Core_Success;
}


static Core_Result get_name(Core_String** RETURN, dx_ui_widget* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->name) {
    CORE_REFERENCE(SELF->name);
  }
  *RETURN = SELF->name;
  return Core_Success;
}

static Core_Result set_name(dx_ui_widget* SELF, Core_String* name) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (name) {
    CORE_REFERENCE(name);
  }
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
  }
  SELF->name = name;
  return Core_Success;
}

static void dx_ui_widget_destruct(dx_ui_widget* SELF) {
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  CORE_UNREFERENCE(SELF->manager);
  SELF->manager = NULL;
  SELF->parent = NULL;
}

static void dx_ui_widget_constructDispatch(dx_ui_widget_Dispatch* SELF) {
  SELF->set_name = &set_name;
  SELF->get_name = &get_name;
  SELF->get_relative_rectangle = &get_relative_rectangle;
  SELF->get_absolute_rectangle = &get_absolute_rectangle;
}

Core_Result dx_ui_widget_construct(dx_ui_widget* SELF, dx_ui_manager* manager) {
  DX_CONSTRUCT_PREFIX(dx_ui_widget);
  SELF->parent = NULL;
  if (!manager) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->manager = manager;
  CORE_REFERENCE(manager);
  SELF->name = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
