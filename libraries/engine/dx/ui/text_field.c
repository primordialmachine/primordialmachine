#include "dx/ui/text_field.h"

#include "dx/rectangle_presenter.h"
#include "dx/ui/manager.h"
#include "dx/val/cbinding.h"
#include "dx/font_presenter.h"

// strlen
#include <string.h>

typedef struct DX_UI_RENDER_ARGS {
  Core_Real32 canvas_horizontal_size;
  Core_Real32 canvas_vertical_size;
  Core_Real32 dpi_horizontal;
  Core_Real32 dpi_vertical;
} DX_UI_RENDER_ARGS;

static DX_TEXT_PRESENTATION_OPTIONS const TEXT_PRESENTATION_OPTIONS = {
  .code_point_not_presentable_policy = DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP,
  .glyph_not_availabe_policy = DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP,
  .vertical_anchor = dx_text_anchor_vertical_center,
  .present_glyph_ascender = Core_False,
  .present_glyph_descender = Core_False,
  .present_glyph = Core_True,
};

// The default text color.
static Core_InlineRgbaR32 const TEXT_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

// The default background color.
static Core_InlineRgbaR32 const BACKGROUND_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

/// The font file.
static char* const FONT_FILE = "./assets/fonts/SourceCodePro-Regular.ttf";

/// The DPI along the horizontal axis.
/// As the base DPI is 96, a DPI of 216 is a DPI scaling of 225%.
static Core_Real32 DPI_HORIZONTAL = 216.f;

/// The DPI scale along the vertical axis.
/// As the base DPI is 96, a DPI of 216 is a DPI scaling of 225%.
static Core_Real32 DPI_VERTICAL = 216.f;

/// The font size in points.
static Core_Size const FONT_SIZE = 12;

static inline Core_Size get_font_size() {
  Core_Real32 dpi = DPI_HORIZONTAL < DPI_VERTICAL ? DPI_HORIZONTAL : DPI_VERTICAL;
  Core_Real32 dpi_scale = dpi / 96.f;
  Core_floorR32(&dpi_scale, (Core_Real32)FONT_SIZE * dpi_scale);
  return (Core_Size)dpi_scale;
}

/// @Ensure that SELF->font is pointing to a suitable font.
/// @param SELF A pointer to this console.
/// @method-call
static inline Core_Result create_font(dx_ui_text_field* SELF) {
  Core_String* font_file = NULL;
  if (Core_String_create(&font_file, FONT_FILE, strlen(FONT_FILE))) {
    return Core_Failure;
  }
  dx_font* font = NULL;
  Core_Size font_size = get_font_size();
  Core_Result result = dx_font_manager_get_or_create_font(&font, DX_UI_WIDGET(SELF)->manager->font_presenter->font_manager, font_file, font_size);
  CORE_UNREFERENCE(font_file);
  font_file = NULL;
  if (result) {
    return Core_Failure;
  }
  SELF->font = font;
  return Core_Success;
}

Core_defineObjectType("dx.ui.text_field",
                       dx_ui_text_field,
                       dx_ui_widget);

static Core_Result set_relative_position(dx_ui_text_field* SELF, DX_VEC2_F32 const* relative_position);

static Core_Result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF);

static Core_Result set_relative_size(dx_ui_text_field* SELF, DX_VEC2_F32 const* relative_size);

static Core_Result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF);

static Core_Result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF);

static Core_Result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF);

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_text_field* SELF, Core_String* name);

static Core_Result render(dx_ui_text_field* SELF, Core_Real32 canvas_horizontal_size, Core_Real32 canvas_vertical_size, Core_Real32 dpi_horizontal, Core_Real32 dpi_vertical);

static void dx_ui_text_field_destruct(dx_ui_text_field* SELF) {
  CORE_UNREFERENCE(SELF->font);
  SELF->font = NULL;

  CORE_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_ui_text_field_constructDispatch(dx_ui_text_field_Dispatch* SELF) {
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_position = (Core_Result(*)(DX_VEC2_F32*,dx_ui_widget*)) & get_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_size = (Core_Result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->render = (Core_Result(*)(dx_ui_widget*,Core_Real32,Core_Real32,Core_Real32,Core_Real32)) & render;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_position = (Core_Result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_size = (Core_Result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_position = (Core_Result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_size = (Core_Result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_child_by_name = (Core_Result(*)(dx_ui_widget**, dx_ui_widget*, Core_String*)) & get_child_by_name;
}

Core_Result dx_ui_text_field_construct(dx_ui_text_field* SELF, dx_ui_manager* manager) {
  DX_CONSTRUCT_PREFIX(dx_ui_text_field);
  if (dx_ui_widget_construct(DX_UI_WIDGET(SELF), manager)) {
    return Core_Failure;
  }

  dx_rect2_f32_set(&SELF->text_bounds, 0.f, 0.f, 0.f, 0.f);
  SELF->text_bounds_dirty = true;

  SELF->vertical_text_anchor = dx_text_anchor_vertical_baseline;
  SELF->horizontal_text_anchor = dx_text_anchor_horizontal_left;

  SELF->vertical_scrollbar_policy = dx_ui_scrollbar_policy_auto;
  SELF->horizontal_scrollbar_policy = dx_ui_scrollbar_policy_auto;

  dx_vec2_f32_set(&SELF->relative_position, 0.f, 0.f);
  dx_vec2_f32_set(&SELF->relative_size, 0.f, 0.f);
  SELF->background_color = BACKGROUND_COLOR;
  SELF->text_color = TEXT_COLOR;
  if (dx_text_document_create(&SELF->text)) {
    return Core_Failure;
  }
  if (create_font(SELF)) {
    CORE_UNREFERENCE(SELF->text);
    SELF->text = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ui_text_field_create(dx_ui_text_field** RETURN, dx_ui_manager* manager) {
  DX_CREATE_PREFIX(dx_ui_text_field)
  if (dx_ui_text_field_construct(SELF, manager)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result set_relative_position(dx_ui_text_field* SELF, DX_VEC2_F32 const* relative_position) {
  if (!SELF || !relative_position) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_position = *relative_position;
  return Core_Success;
}

static Core_Result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_position;
  return Core_Success;
}

static Core_Result set_relative_size(dx_ui_text_field* SELF, DX_VEC2_F32 const* relative_size) {
  if (!SELF || !relative_size) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->relative_size = *relative_size;
  return Core_Success;
}

static Core_Result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->relative_size;
  return Core_Success;
}

static Core_Result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF) {
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

static Core_Result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_text_field* SELF) {
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

static Core_Result get_child_by_name(dx_ui_widget** RETURN, dx_ui_text_field* SELF, Core_String* name) {
  // Text has no publicly available children!
  Core_setError(Core_Error_NotFound);
  return Core_Failure;
}

/// @{ 

Core_Result dx_ui_text_field_get_background_color(Core_InlineRgbaR32* RETURN, dx_ui_text_field* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->background_color;
  return Core_Success;
}

Core_Result dx_ui_text_field_set_background_color(dx_ui_text_field* SELF, Core_InlineRgbaR32 const* background_color) {
  if (!SELF || !background_color) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->background_color = *background_color;
  return Core_Success;
}

/// @}

/// @{ 

Core_Result dx_ui_text_field_get_text_color(Core_InlineRgbaR32* RETURN, dx_ui_text_field* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->text_color;
  return Core_Success;
}


Core_Result dx_ui_text_field_set_text_color(dx_ui_text_field* SELF, Core_InlineRgbaR32 const* text_color) {
  if (!SELF || !text_color) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->text_color = *text_color;
  return Core_Success;
}

/// @}

/// @{ 

Core_Result dx_ui_text_field_get_text(Core_String** RETURN, dx_ui_text_field* SELF) {
  if (dx_text_document_get_text(RETURN, SELF->text)) {
    return Core_Failure;
  }
  SELF->text_bounds_dirty = true;
  return Core_Success;
}

Core_Result dx_ui_text_field_set_text(dx_ui_text_field* SELF, Core_String* text) {
  if (dx_text_document_set_text(SELF->text, text)) {
    return Core_Failure;
  }
  SELF->text_bounds_dirty = true;
  return Core_Success;
}

/// @}

/// @{

Core_Result dx_ui_text_field_get_horizontal_scrollbar_policy(dx_ui_scrollbar_policy* RETURN, dx_ui_text_field* SELF) {
  *RETURN = SELF->horizontal_scrollbar_policy;
  return Core_Success;
}

Core_Result dx_ui_text_field_set_horizontal_scrollbar_policy(dx_ui_text_field* SELF, dx_ui_scrollbar_policy scrollbar_policy) {
  dx_ui_scrollbar_policy new_value = scrollbar_policy;
  dx_ui_scrollbar_policy old_value = SELF->horizontal_scrollbar_policy;
  SELF->horizontal_scrollbar_policy = new_value;
  if (old_value != new_value) {
    if (new_value) {
      dx_log("horizontal scrollbar policy changed\n", sizeof("horizontal scrollbar policy changed\n") - 1);
    } else {
      dx_log("horizontal scrollbar policy changed\n", sizeof("horizontal scrollbar policy changed\n") - 1);
    }
  }
  return Core_Success;
}

/// @}

/// @{ 

Core_Result dx_ui_text_field_get_vertical_scrollbar_policy(dx_ui_scrollbar_policy* RETURN, dx_ui_text_field* SELF) {
  *RETURN = SELF->vertical_scrollbar_policy;
  return Core_Success;
}

Core_Result dx_ui_text_field_set_vertical_scrollbar_policy(dx_ui_text_field* SELF, dx_ui_scrollbar_policy scrollbar_policy) {
  dx_ui_scrollbar_policy new_value = scrollbar_policy;
  dx_ui_scrollbar_policy old_value = SELF->vertical_scrollbar_policy;
  SELF->vertical_scrollbar_policy = new_value;
  if (old_value != new_value) {
    if (new_value) {
      dx_log("vertical scrollbar policy changed\n", sizeof("vertical scrollbar policy changed\n") - 1);
    } else {
      dx_log("vertical scrollbar policy changed\n", sizeof("vertical scrollbar policy changed\n") - 1);
    }
  }
  return Core_Success;
}

/// @} 


Core_Result dx_ui_text_field_append_text(dx_ui_text_field* SELF, Core_String* text) {
  if (dx_text_document_append_text(SELF->text, text)) {
    return Core_Failure;
  }
  SELF->text_bounds_dirty = true;
  return Core_Success;
}

static Core_Result update_text_bounds(dx_ui_text_field* SELF) {
  // The distance from one baseline to the previous/next baseline multiplied by 1.5.
  // @todo The baseline distance multiplier should be part of the properties of the ui widget.
  Core_Real32 baseline_distance;
  if (dx_font_get_baseline_distance(&baseline_distance, SELF->font)) {
    return Core_Failure;
  }
  baseline_distance = baseline_distance * 1.5f;

  static DX_TEXT_MEASUREMENT_OPTIONS const TEXT_MEASUREMENT_OPTIONS = {
    .code_point_not_presentable_policy = DX_CODE_POINT_NOT_PRESENTABLE_POLICY_SKIP,
    .glyph_not_availabe_policy = DX_GLYPH_NOT_AVAILABLE_POLICY_SKIP,
    .text_bounds_type = dx_text_bounds_type_logical,
    .vertical_anchor = dx_text_anchor_vertical_center,
  };

  Core_Size n;
  if (dx_object_array_get_size(&n, SELF->text->lines)) {
    return Core_Failure;
  }

  DX_VEC2_F32 p;
  if (get_relative_position(&p, SELF)) {
    return Core_Failure;
  }

  DX_RECT2_F32 text_bounds1;
  dx_rect2_f32_set2(&text_bounds1, p.e[0], p.e[1], 0.f, 0.f);

  for (Core_Size i = n; i > 0; --i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((Core_Object**)&line, SELF->text->lines, i - 1)) {
      return Core_Failure;
    }
    // @todo Do NOT create a string each time.
    // Use dx_string_buffer's string iterator.
    void const* bytes = NULL;
    if (dx_string_buffer_get_bytes(&bytes, SELF->text->text)) {
      return Core_Failure;
    }
    Core_Size number_of_bytes;
    if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->text->text)) {
      return Core_Failure;
    }
    Core_String* string;
    if (Core_String_create(&string, ((char const*)bytes) + line->start, line->length)) {
      return Core_Failure;
    }
    DX_RECT2_F32 line_bounds;
    if (dx_font_presenter_measure_line_string(DX_UI_WIDGET(SELF)->manager->font_presenter,
                                              &p, string, SELF->font, &TEXT_MEASUREMENT_OPTIONS, &line_bounds)) {
      return Core_Failure;
    }
    dx_rect2_f32_union(&text_bounds1, &text_bounds1, &line_bounds);
    CORE_UNREFERENCE(string);
    string = NULL;
    p.e[1] += baseline_distance;
  }
  SELF->text_bounds = text_bounds1;
  return Core_Success;
}

static Core_Result on_update(dx_ui_text_field* SELF, DX_UI_RENDER_ARGS const* args) {
  if (SELF->text_bounds_dirty) {
    if (update_text_bounds(SELF)) {
      return Core_Failure;
    }
    SELF->text_bounds_dirty = false;
  }
  return Core_Success;
}

static Core_Result on_render_scrollbars(dx_ui_text_field* SELF, DX_UI_RENDER_ARGS const* args, bool display_vertical_scrollbar, bool display_horizontal_scrollbar) {
  return Core_Success;
}

static Core_Result on_render(dx_ui_text_field* SELF, DX_UI_RENDER_ARGS const* args) {
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

  Core_Real32 y_offset = 0.f;
  switch (SELF->vertical_text_anchor) {
    case dx_text_anchor_vertical_baseline: {
      y_offset = 0.f;
    } break;
    case dx_text_anchor_vertical_center: {
      DX_RECT2_F32 wr;
      if (dx_ui_widget_get_relative_rectangle(&wr, DX_UI_WIDGET(SELF))) {
        return Core_Failure;
      }
      DX_RECT2_F32 tr;
      tr = SELF->text_bounds;
      Core_Real32 ctr = tr.offset.y + tr.extend.y / 2.f;
      Core_Real32 cwr = wr.offset.y + wr.extend.y / 2.f;
      Core_Real32 d = cwr - ctr;
      y_offset = d;
    } break;
  }

  // The distance from one baseline to the previous/next baseline multiplied by 1.5.
  // @todo The baseline distance multiplier should be part of the properties of the ui widget.
  Core_Real32 baseline_distance;
  dx_font_get_baseline_distance(&baseline_distance, SELF->font);
  baseline_distance = baseline_distance * 1.5f;

  Core_Size n;
  if (dx_object_array_get_size(&n, SELF->text->lines)) {
    return Core_Failure;
  }

  DX_VEC2_F32 p;
  if (get_absolute_position(&p, SELF)) {
    return Core_Failure;
  }
  p.e[1] += y_offset;

  for (Core_Size i = n; i > 0; --i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((Core_Object**)&line, SELF->text->lines, i - 1)) {
      return Core_Failure;
    }
    // @todo Do NOT create a string each time.
    // Use dx_string_buffer's string iterator.
    void const* bytes = NULL;
    if (dx_string_buffer_get_bytes(&bytes, SELF->text->text)) {
      return Core_Failure;
    }
    Core_Size number_of_bytes;
    if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->text->text)) {
      return Core_Failure;
    }
    Core_String* string;
    if (Core_String_create(&string, ((char const*)bytes) + line->start, line->length)) {
      return Core_Failure;
    }
    dx_font_presenter_render_line_string(DX_UI_WIDGET(SELF)->manager->font_presenter,
                                         &p, string, &SELF->text_color, SELF->font, &TEXT_PRESENTATION_OPTIONS);
    CORE_UNREFERENCE(string);
    string = NULL;
    p.e[1] += baseline_distance;
  }
  return Core_Success;
}

static Core_Result render(dx_ui_text_field* SELF, Core_Real32 canvas_horizontal_size, Core_Real32 canvas_vertical_size, Core_Real32 dpi_horizontal, Core_Real32 dpi_vertical) {
  DX_UI_RENDER_ARGS args = {
    .canvas_horizontal_size = canvas_horizontal_size,
    .canvas_vertical_size = canvas_vertical_size,
    .dpi_horizontal = dpi_horizontal,
    .dpi_vertical = dpi_vertical,
  };

  if (on_update(SELF, &args)) {
    return Core_Failure;
  }

  static const Core_Real32 scrollbar_size = 64.f;
  bool display_vertical_scrollbar = false,
       display_horizontal_scrollbar = false;
  bool vertical_overflow = false,
       horizontal_overflow = false;
  bool changed = false;

  do {
    // when this loop exits, then display_vertical_scrollbar and display_horizontal_scrollbar indicate what scrollbars to render.
    changed = false;

    // display the horizontal scrollbar because of "always" policy?
    if (SELF->horizontal_scrollbar_policy == dx_ui_scrollbar_policy_always) {
      if (!display_horizontal_scrollbar) {
        display_horizontal_scrollbar = true;
        changed = true;
        //dx_log("display horizontal scrollbar = true\n", sizeof("display horizontal scrollbar = true") - 1);
      }
    }
    // display the horizontal scrollbar because of "auto" policy and the existence of a vertical overflow.
    if (SELF->horizontal_scrollbar_policy == dx_ui_scrollbar_policy_auto && vertical_overflow) {
      if (!display_horizontal_scrollbar) {
        display_horizontal_scrollbar = true;
        changed = true;
        //dx_log("display horizontal scrollbar = true\n", sizeof("display horizontal scrollbar = true\n") - 1);
      }
    }

    // display the vertical scrollbar because of "always" policy?
    if (SELF->vertical_scrollbar_policy == dx_ui_scrollbar_policy_always) {
      if (!display_vertical_scrollbar) {
        display_vertical_scrollbar = true;
        changed = true;
        //dx_log("display vertical scrollbar = true\n", sizeof("display vertical scrollbar = true\n") - 1);
      }
    }
    // display the vertical scrollbar because of "auto" policy and the existence of a horizontal overflow.
    if (SELF->vertical_scrollbar_policy == dx_ui_scrollbar_policy_auto && horizontal_overflow) {
      if (!display_vertical_scrollbar) {
        display_vertical_scrollbar = true;
        changed = true;
        //dx_log("display vertical scrollbar = true\n", sizeof("display vertical scrollbar = true\n") - 1);
      }
    }

    // do we have a horizontal overflow?
    Core_Real32 available_size_x = SELF->relative_size.e[0];
    if (display_vertical_scrollbar) {
      available_size_x -= scrollbar_size;
    }
    if (SELF->text_bounds.extend.x > available_size_x) {
      if (!horizontal_overflow) {
        horizontal_overflow = true;
        changed = true;
        //dx_log("horizontal overflow = true\n", sizeof("horizontal overflow = true\n") - 1);
      }
    }
    // do we have a vertical overflow?
    Core_Real32 available_size_y = SELF->relative_size.e[1];
    if (display_horizontal_scrollbar) {
      available_size_y -= scrollbar_size;
    }
    if (SELF->text_bounds.extend.y > available_size_y) {
      if (!vertical_overflow) {
        vertical_overflow = true;
        changed = true;
        //dx_log("vertical overflow = true\n", sizeof("vertical overflow = true\n") - 1);
      }
    }
  } while (changed);

  if (on_render(SELF, &args)) {
    return Core_Failure;
  }
  if (on_render_scrollbars(SELF, &args, display_vertical_scrollbar, display_horizontal_scrollbar)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_ui_text_get_font(dx_font** RETURN, dx_ui_text_field* SELF) {
  CORE_REFERENCE(SELF->font);
  *RETURN = SELF->font;
  return Core_Success;
}

Core_Result dx_ui_text_set_font(dx_ui_text_field* SELF, dx_font* font) {
  CORE_REFERENCE(font);
  CORE_UNREFERENCE(SELF->font);
  SELF->font = font;
  return Core_Success;
}
