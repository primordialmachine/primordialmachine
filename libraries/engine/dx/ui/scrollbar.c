#include "dx/ui/scrollbar.h"

#include "dx/rectangle_presenter.h"
#include "dx/ui/manager.h"
#include "dx/val/cbinding.h"

Core_defineObjectType("dx.ui.scrollbar",
                       dx_ui_scrollbar,
                       dx_ui_widget);

static Core_Result set_relative_position(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_position);

static Core_Result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result set_relative_size(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_size);

static Core_Result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_scrollbar* SELF, Core_String* name);

static Core_Result render(dx_ui_scrollbar* SELF, Core_Real32 canvas_horizontal_size, Core_Real32 canvas_vertical_size, Core_Real32 dpi_horizontal, Core_Real32 dpi_vertical);

static void dx_ui_scrollbar_destruct(dx_ui_scrollbar* SELF)
{/*Intentionally empty.*/}

static void dx_ui_scrollbar_constructDispatch(dx_ui_scrollbar_Dispatch* SELF) {
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_position = (Core_Result(*)(DX_VEC2_F32*,dx_ui_widget*)) & get_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_size = (Core_Result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->render = (Core_Result(*)(dx_ui_widget*,Core_Real32,Core_Real32,Core_Real32,Core_Real32)) & render;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_position = (Core_Result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_size = (Core_Result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_position = (Core_Result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_size = (Core_Result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_child_by_name = (Core_Result(*)(dx_ui_widget**, dx_ui_widget*, Core_String*)) & get_child_by_name;
}

Core_Result dx_ui_scrollbar_construct(dx_ui_scrollbar* SELF, dx_ui_manager* manager) {
  DX_CONSTRUCT_PREFIX(dx_ui_scrollbar);
  if (dx_ui_widget_construct(DX_UI_WIDGET(SELF), manager)) {
    return Core_Failure;
  }
  dx_vec2_f32_set(&SELF->relative_position, 0.f, 0.f);
  dx_vec2_f32_set(&SELF->relative_size, 0.f, 0.f);
  dx_rgba_f32_set(&SELF->background_color, 1.f, 1.f, 1.f, 1.f);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ui_scrollbar_create(dx_ui_scrollbar** RETURN, dx_ui_manager* manager) {
  DX_CREATE_PREFIX(dx_ui_scrollbar);
  if (dx_ui_scrollbar_construct(SELF, manager)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result set_relative_position(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_position) {
  if (!SELF || !relative_position) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_position = *relative_position;
  return Core_Success;
}

static Core_Result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_position;
  return Core_Success;
}

static Core_Result set_relative_size(dx_ui_scrollbar* SELF, DX_VEC2_F32 const* relative_size) {
  if (!SELF || !relative_size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_size = *relative_size;
  return Core_Success;
}

static Core_Result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_size;
  return Core_Success;
}

static Core_Result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  DX_VEC2_F32 a;
  if (dx_ui_widget_get_relative_position(&a, DX_UI_WIDGET(SELF))) {
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF)->parent) {
    DX_VEC2_F32 b;
    if (dx_ui_widget_get_absolute_position(&b, DX_UI_WIDGET(SELF)->parent)) {
      return Core_Failure;
    }
    dx_vec2_f32_add3(&a, &a, &b);
  }
  *RETURN = a;
  return Core_Success;
}

static Core_Result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  DX_VEC2_F32 a;
  if (dx_ui_widget_get_relative_size(&a, DX_UI_WIDGET(SELF))) {
    return Core_Failure;
  }
  *RETURN = a;
  return Core_Success;
}

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_scrollbar* SELF, Core_String* name) {
  // Scrollbar has no publicly available children!
  Core_setError(Core_Error_NotFound);
  return Core_Failure;
}

Core_Result dx_ui_scrollbar_set_background_color(dx_ui_scrollbar* SELF, Core_InlineRgbaR32 const* background_color) {
  if (!SELF || !background_color) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->background_color = *background_color;
  return Core_Success;
}

Core_Result dx_ui_scrollbar_get_background_color(Core_InlineRgbaR32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->background_color;
  return Core_Success;
}

static Core_Result render(dx_ui_scrollbar* SELF, Core_Real32 canvas_horizontal_size, Core_Real32 canvas_vertical_size, Core_Real32 dpi_horizontal, Core_Real32 dpi_vertical) {
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
    return Core_Failure;
  }
  
  return Core_Success;
}

#if 0
Core_Result dx_ui_scrollbar_append_child(dx_ui_scrollbar* SELF, dx_ui_widget* child) {
  if (!SELF || !child) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_object_array_append(SELF->children, DX_OBJECT(child));
}
#endif

#if 0
Core_Result dx_ui_scrollbar_prepend_child(dx_ui_scrollbar* SELF, dx_ui_widget* child) {
  if (!SELF || !child) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_object_array_prepend(SELF->children, DX_OBJECT(child));
}
#endif

#if 0
Core_Result dx_ui_scrollbar_insert_child(dx_ui_scrollbar* SELF, dx_ui_widget* child, Core_Size index) {
  if (!SELF || !child) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (DX_UI_WIDGET(SELF) == child || NULL != child->parent) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_object_array_insert(SELF->children, DX_OBJECT(child), index);
}
#endif
