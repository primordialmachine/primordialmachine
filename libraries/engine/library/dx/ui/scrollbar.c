#include "dx/ui/scrollbar.h"

#include "dx/rectangle_presenter.h"
#include "dx/ui/manager.h"
#include "dx/val/cbinding.h"

Core_defineObjectType("dx.ui.scrollbar",
                       dx_ui_scrollbar,
                       dx_ui_widget);

static Core_Result set_relative_position(dx_ui_scrollbar* SELF, Core_InlineVector2R32 const* relative_position);

static Core_Result get_relative_position(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result set_relative_size(dx_ui_scrollbar* SELF, Core_InlineVector2R32 const* relative_size);

static Core_Result get_relative_size(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result get_absolute_position(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result get_absolute_size(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF);

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_scrollbar* SELF, Core_String* name);

static Core_Result render(dx_ui_scrollbar* SELF);

static void dx_ui_scrollbar_destruct(dx_ui_scrollbar* SELF)
{/*Intentionally empty.*/}

static void dx_ui_scrollbar_constructDispatch(dx_ui_scrollbar_Dispatch* SELF) {
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_position = (Core_Result(*)(Core_InlineVector2R32*,dx_ui_widget*)) & get_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_size = (Core_Result(*)(Core_InlineVector2R32*, dx_ui_widget*)) & get_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->render = (Core_Result(*)(dx_ui_widget*)) & render;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_position = (Core_Result(*)(dx_ui_widget*, Core_InlineVector2R32 const*)) & set_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_size = (Core_Result(*)(dx_ui_widget*, Core_InlineVector2R32 const*)) & set_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_position = (Core_Result(*)(Core_InlineVector2R32*, dx_ui_widget*)) & get_absolute_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_size = (Core_Result(*)(Core_InlineVector2R32*, dx_ui_widget*)) & get_absolute_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_child_by_name = (Core_Result(*)(dx_ui_widget**, dx_ui_widget*, Core_String*)) & get_child_by_name;
}

Core_Result dx_ui_scrollbar_construct(dx_ui_scrollbar* SELF, dx_ui_manager* manager) {
  DX_CONSTRUCT_PREFIX(dx_ui_scrollbar);
  if (dx_ui_widget_construct(DX_UI_WIDGET(SELF), manager)) {
    return Core_Failure;
  }
  SELF->scrollbar_direction = dx_ui_scrollbar_direction_horizontal;
  dx_vec2_f32_set(&SELF->relative_position, 0.f, 0.f);
  dx_vec2_f32_set(&SELF->relative_size, 0.f, 0.f);
  Core_InlineRgbaR32_set(&SELF->background_color, 1.f, 1.f, 1.f, 1.f);
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

static Core_Result set_relative_position(dx_ui_scrollbar* SELF, Core_InlineVector2R32 const* relative_position) {
  if (!SELF || !relative_position) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_position = *relative_position;
  return Core_Success;
}

static Core_Result get_relative_position(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_position;
  return Core_Success;
}

static Core_Result set_relative_size(dx_ui_scrollbar* SELF, Core_InlineVector2R32 const* relative_size) {
  if (!SELF || !relative_size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_size = *relative_size;
  return Core_Success;
}

static Core_Result get_relative_size(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_size;
  return Core_Success;
}

static Core_Result get_absolute_position(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF) {
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

static Core_Result get_absolute_size(Core_InlineVector2R32* RETURN, dx_ui_scrollbar* SELF) {
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

static Core_Result render(dx_ui_scrollbar* SELF) {
  DX_RECT2_F32 absolute_rectangle;
  if (dx_ui_widget_get_absolute_rectangle(&absolute_rectangle, DX_UI_WIDGET(SELF))) {
    return Core_Failure;
  }

  //
  DX_RECT2_F32 r[2];
  //
  Core_InlineRgbaR32 colors[2];
  dx_rgb_n8_to_rgba_f32(&dx_colors_capri, 1.f, &colors[0]);
  dx_rgb_n8_to_rgba_f32(&dx_colors_amber, 1.f, &colors[1]);

  if (dx_rectangle_presenter_fill_rectangle(DX_UI_WIDGET(SELF)->manager->rectangle_presenter,
                                            &absolute_rectangle,
                                            0.f,
                                            &SELF->background_color)) {
    return Core_Failure;
  }
  if (SELF->scrollbar_direction == dx_ui_scrollbar_direction_vertical) {
    // bottom to top scrollbar
    Core_Real32 height;
    Core_minR32(&height, 16.f, absolute_rectangle.extend.y);
    // bottom button
    r[0].offset = absolute_rectangle.offset;
    r[0].extend.x = absolute_rectangle.extend.x;
    r[0].extend.y = height;
    // top button
    r[1].offset.x = absolute_rectangle.offset.x;
    r[1].offset.y = absolute_rectangle.offset.y + absolute_rectangle.extend.y - height;
    r[1].extend.x = absolute_rectangle.extend.x;
    r[1].extend.y = height;
  } else {
    // left to right scrollbar
    Core_Real32 width;
    Core_minR32(&width, 16.f, absolute_rectangle.extend.x);
    // left button
    r[0].offset = absolute_rectangle.offset;
    r[0].extend.x = width;
    r[0].extend.y = absolute_rectangle.extend.y;
    // right button
    r[1].offset.x = absolute_rectangle.offset.x + absolute_rectangle.extend.x - width;
    r[1].offset.y = absolute_rectangle.offset.y;
    r[1].extend.x = width;
    r[1].extend.y = absolute_rectangle.extend.y;
  }
 
  if (dx_rectangle_presenter_fill_rectangle(DX_UI_WIDGET(SELF)->manager->rectangle_presenter,
                                            &r[0],
                                            0.f,
                                            &colors[0])) {
    return Core_Failure;
  }
  if (dx_rectangle_presenter_fill_rectangle(DX_UI_WIDGET(SELF)->manager->rectangle_presenter,
                                            &r[1],
                                            0.f,
                                            &colors[1])) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_ui_scrollbar_get_scrollbar_direction(dx_ui_scrollbar_direction* RETURN, dx_ui_scrollbar* SELF) {
  *RETURN = SELF->scrollbar_direction;
  return Core_Success;
}

Core_Result dx_ui_scrollbar_set_scrollbar_direction(dx_ui_scrollbar* SELF, dx_ui_scrollbar_direction scrollbar_direction) {
  SELF->scrollbar_direction = scrollbar_direction;
  return Core_Success;
}
