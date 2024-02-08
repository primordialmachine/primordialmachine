#include "dx/ui/group.h"

#include "dx/rectangle_presenter.h"
#include "dx/ui/manager.h"
#include "dx/val/cbinding.h"

Core_defineObjectType("dx.ui.group",
                       dx_ui_group,
                       dx_ui_widget);

static Core_Result set_relative_position(dx_ui_group* SELF, Core_InlineVector2R32 const* relative_position);

static Core_Result get_relative_position(Core_InlineVector2R32* RETURN, dx_ui_group* SELF);

static Core_Result set_relative_size(dx_ui_group* SELF, Core_InlineVector2R32 const* relative_size);

static Core_Result get_relative_size(Core_InlineVector2R32* RETURN, dx_ui_group* SELF);

static Core_Result get_absolute_position(Core_InlineVector2R32* RETURN, dx_ui_group* SELF);

static Core_Result get_absolute_size(Core_InlineVector2R32* RETURN, dx_ui_group* SELF);

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_group* SELF, Core_String* name);

static Core_Result render(dx_ui_group* SELF);

static void dx_ui_group_destruct(dx_ui_group* SELF) {
  CORE_UNREFERENCE(SELF->children);
  SELF->children = NULL;
}

static void dx_ui_group_constructDispatch(dx_ui_group_Dispatch* SELF) {
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_position = (Core_Result(*)(Core_InlineVector2R32*,dx_ui_widget*)) & get_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_size = (Core_Result(*)(Core_InlineVector2R32*, dx_ui_widget*)) & get_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->render = (Core_Result(*)(dx_ui_widget*)) & render;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_position = (Core_Result(*)(dx_ui_widget*, Core_InlineVector2R32 const*)) & set_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_size = (Core_Result(*)(dx_ui_widget*, Core_InlineVector2R32 const*)) & set_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_position = (Core_Result(*)(Core_InlineVector2R32*, dx_ui_widget*)) & get_absolute_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_size = (Core_Result(*)(Core_InlineVector2R32*, dx_ui_widget*)) & get_absolute_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_child_by_name = (Core_Result(*)(dx_ui_widget**, dx_ui_widget*, Core_String*)) & get_child_by_name;
}

Core_Result dx_ui_group_construct(dx_ui_group* SELF, dx_ui_manager* manager) {
  DX_CONSTRUCT_PREFIX(dx_ui_group);
  if (dx_ui_widget_construct(DX_UI_WIDGET(SELF), manager)) {
    return Core_Failure;
  }
  if (dx_object_array_create(&SELF->children, 0)) {
    return Core_Failure;
  }
  dx_vec2_f32_set(&SELF->relative_position, 0.f, 0.f);
  dx_vec2_f32_set(&SELF->relative_size, 0.f, 0.f);
  Core_InlineRgbaR32_set(&SELF->background_color, 1.f, 1.f, 1.f, 1.f);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ui_group_create(dx_ui_group** RETURN, dx_ui_manager* manager) {
  DX_CREATE_PREFIX(dx_ui_group);
  if (dx_ui_group_construct(SELF, manager)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result set_relative_position(dx_ui_group* SELF, Core_InlineVector2R32 const* relative_position) {
  if (!SELF || !relative_position) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_position = *relative_position;
  return Core_Success;
}

static Core_Result get_relative_position(Core_InlineVector2R32* RETURN, dx_ui_group* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_position;
  return Core_Success;
}

static Core_Result set_relative_size(dx_ui_group* SELF, Core_InlineVector2R32 const* relative_size) {
  if (!SELF || !relative_size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_size = *relative_size;
  return Core_Success;
}

static Core_Result get_relative_size(Core_InlineVector2R32* RETURN, dx_ui_group* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_size;
  return Core_Success;
}

static Core_Result get_absolute_position(Core_InlineVector2R32* RETURN, dx_ui_group* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_InlineVector2R32 a;
  if (dx_ui_widget_get_relative_position(&a, DX_UI_WIDGET(SELF))) {
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF)->parent) {
    Core_InlineVector2R32 b;
    if (dx_ui_widget_get_absolute_position(&b, DX_UI_WIDGET(SELF)->parent)) {
      return Core_Failure;
    }
    Core_InlineVector2R32_add_vv(&a, &a, &b);
  }
  *RETURN = a;
  return Core_Success;
}

static Core_Result get_absolute_size(Core_InlineVector2R32* RETURN, dx_ui_group* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_InlineVector2R32 a;
  if (dx_ui_widget_get_relative_size(&a, DX_UI_WIDGET(SELF))) {
    return Core_Failure;
  }
  *RETURN = a;
  return Core_Success;
}

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_group* SELF, Core_String* name) {
  Core_Size n;
  if (dx_object_array_get_size(&n, SELF->children)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_ui_widget* child = NULL;
    if (dx_object_array_get_at((Core_Object**)&child, SELF->children, i)) {
      return Core_Failure;
    }
    if (child->name) {
      Core_Boolean isEqualTo = Core_False;
      if (Core_String_isEqualTo(&isEqualTo, child->name, name)) {
        return Core_Failure;
      }
      if (isEqualTo) {
        CORE_REFERENCE(child);
        *RETURN = child;
        return Core_Success;
      }
    }
  }
  Core_setError(Core_Error_NotFound);
  return Core_Failure;
}

Core_Result dx_ui_group_set_background_color(dx_ui_group* SELF, Core_InlineRgbaR32 const* background_color) {
  if (!SELF || !background_color) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->background_color = *background_color;
  return Core_Success;
}

Core_Result dx_ui_group_get_background_color(Core_InlineRgbaR32* RETURN, dx_ui_group* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->background_color;
  return Core_Success;
}

static Core_Result render(dx_ui_group* SELF) {
  DX_RECT2_F32 target_rectangle;
  if (dx_ui_widget_get_absolute_rectangle(&target_rectangle, DX_UI_WIDGET(SELF))) {
    return Core_Failure;
  }
  if (dx_rectangle_presenter_fill_rectangle(DX_UI_WIDGET(SELF)->manager->rectangle_presenter,
                                            &target_rectangle,
                                            0.f,
                                            &SELF->background_color)) {
    return Core_Failure;
  }
  
  return Core_Success;
}

Core_Result dx_ui_group_append_child(dx_ui_group* SELF, dx_ui_widget* child) {
  if (!SELF || !child) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_object_array_append(SELF->children, CORE_OBJECT(child));
}

Core_Result dx_ui_group_prepend_child(dx_ui_group* SELF, dx_ui_widget* child) {
  if (!SELF || !child) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_object_array_prepend(SELF->children, CORE_OBJECT(child));
}

Core_Result dx_ui_group_insert_child(dx_ui_group* SELF, dx_ui_widget* child, Core_Size index) {
  if (!SELF || !child) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_object_array_insert(SELF->children, CORE_OBJECT(child), index);
}
