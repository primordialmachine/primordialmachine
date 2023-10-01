#include "dx/ui/text.h"

#include "dx/rectangle_presenter.h"
#include "dx/ui/manager.h"
#include "dx/val/cbinding.h"
#include "dx/font_presenter.h"

// strlen
#include <string.h>

static dx_n8 const UNKNOWN_SYMBOL_POLICY = dx_font_presenter_unknown_symbol_policy_error;
//static dx_n8 const UNKNOWN_SYMBOL_POLICY = dx_font_presenter_unknown_symbol_policy_error;
static dx_n8 const PRESENTATION_POLICY = dx_font_presenter_presentation_policy_glyph;
//static dx_n8 const PRESENTATION_POLICY = dx_font_presenter_presentation_policy_ascender_descender
//                                       | dx_font_presenter_presentation_policy_glyph;

// The default text color.
static DX_RGBA_F32 const TEXT_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

// The default background color.
static DX_RGBA_F32 const BACKGROUND_COLOR = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

/// The font file.
static char* const FONT_FILE = "./assets/fonts/SourceCodePro-Regular.ttf";

/// The DPI along the horizontal axis.
/// As the base DPI is 96, a DPI of 216 is a DPI scaling of 225%.
static dx_f32 DPI_HORIZONTAL = 216.f;

/// The DPI scale along the vertical axis.
/// As the base DPI is 96, a DPI of 216 is a DPI scaling of 225%.
static dx_f32 DPI_VERTICAL = 216.f;

/// The font size in points.
static dx_size const FONT_SIZE = 12;

static inline dx_size get_font_size() {
  dx_f32 dpi = DPI_HORIZONTAL < DPI_VERTICAL ? DPI_HORIZONTAL : DPI_VERTICAL;
  dx_f32 dpi_scale = dpi / 96.f;
  return (dx_size)dx_floor_f32(((dx_f32)FONT_SIZE) * dpi_scale);
}

/// @Ensure that SELF->font is pointing to a suitable font.
/// @param SELF A pointer to this console.
/// @method-call
static inline dx_result create_font(dx_ui_text* SELF) {
  dx_string* font_file = NULL;
  if (dx_string_create(&font_file, FONT_FILE, strlen(FONT_FILE))) {
    return DX_FAILURE;
  }
  dx_font* font = NULL;
  dx_size font_size = get_font_size();
  dx_result result = dx_font_manager_get_or_create_font(&font, DX_UI_WIDGET(SELF)->manager->font_presenter->font_manager, font_file, font_size);
  DX_UNREFERENCE(font_file);
  font_file = NULL;
  if (result) {
    return DX_FAILURE;
  }
  SELF->font = font;
  return DX_SUCCESS;
}

DX_DEFINE_OBJECT_TYPE("dx.ui.text",
                       dx_ui_text,
                       dx_ui_widget);

static dx_result set_relative_position(dx_ui_text* SELF, DX_VEC2_F32 const* relative_position);

static dx_result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_text* SELF);

static dx_result set_relative_size(dx_ui_text* SELF, DX_VEC2_F32 const* relative_size);

static dx_result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_text* SELF);

static dx_result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_text* SELF);

static dx_result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_text* SELF);

static dx_result render(dx_ui_text* SELF, dx_f32 canvas_horizontal_size, dx_f32 canvas_vertical_size, dx_f32 dpi_horizontal, dx_f32 dpi_vertical);

static void dx_ui_text_destruct(dx_ui_text* SELF) {
  DX_UNREFERENCE(SELF->font);
  SELF->font = NULL;

  DX_UNREFERENCE(SELF->text);
  SELF->text = NULL;
}

static void dx_ui_text_dispatch_construct(dx_ui_text_dispatch* SELF) {
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_position = (dx_result(*)(DX_VEC2_F32*,dx_ui_widget*)) & get_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_relative_size = (dx_result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->render = (dx_result(*)(dx_ui_widget*,dx_f32,dx_f32,dx_f32,dx_f32)) & render;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_position = (dx_result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_position;
  DX_UI_WIDGET_DISPATCH(SELF)->set_relative_size = (dx_result(*)(dx_ui_widget*,DX_VEC2_F32 const*)) & set_relative_size;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_position = (dx_result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_position;
  DX_UI_WIDGET_DISPATCH(SELF)->get_absolute_size = (dx_result(*)(DX_VEC2_F32*, dx_ui_widget*)) & get_absolute_size;
}

dx_result dx_ui_text_construct(dx_ui_text* SELF, dx_ui_manager* manager) {
  dx_rti_type* TYPE = dx_ui_text_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_ui_widget_construct(DX_UI_WIDGET(SELF), manager)) {
    return DX_FAILURE;
  }
  dx_vec2_f32_set(&SELF->relative_position, 0.f, 0.f);
  dx_vec2_f32_set(&SELF->relative_size, 0.f, 0.f);
  SELF->background_color = BACKGROUND_COLOR;
  SELF->text_color = TEXT_COLOR;
  if (dx_text_document_create(&SELF->text)) {
    return DX_FAILURE;
  }
  if (create_font(SELF)) {
    DX_UNREFERENCE(SELF->text);
    SELF->text = NULL;
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ui_text_create(dx_ui_text** RETURN, dx_ui_manager* manager) {
  DX_CREATE_PREFIX(dx_ui_text)
  if (dx_ui_text_construct(SELF, manager)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result set_relative_position(dx_ui_text* SELF, DX_VEC2_F32 const* relative_position) {
  if (!SELF || !relative_position) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->relative_position = *relative_position;
  return DX_SUCCESS;
}

static dx_result get_relative_position(DX_VEC2_F32* RETURN, dx_ui_text* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->relative_position;
  return DX_SUCCESS;
}

static dx_result set_relative_size(dx_ui_text* SELF, DX_VEC2_F32 const* relative_size) {
  if (!SELF || !relative_size) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->relative_size = *relative_size;
  return DX_SUCCESS;
}

static dx_result get_relative_size(DX_VEC2_F32* RETURN, dx_ui_text* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->relative_size;
  return DX_SUCCESS;
}

static dx_result get_absolute_position(DX_VEC2_F32* RETURN, dx_ui_text* SELF) {
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

static dx_result get_absolute_size(DX_VEC2_F32* RETURN, dx_ui_text* SELF) {
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

dx_result dx_ui_text_set_background_color(dx_ui_text* SELF, DX_RGBA_F32 const* background_color) {
  if (!SELF || !background_color) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->background_color = *background_color;
  return DX_SUCCESS;
}

dx_result dx_ui_text_get_background_color(DX_RGBA_F32* RETURN, dx_ui_text* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->background_color;
  return DX_SUCCESS;
}

dx_result dx_ui_text_set_text_color(dx_ui_text* SELF, DX_RGBA_F32 const* text_color) {
  if (!SELF || !text_color) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->text_color = *text_color;
  return DX_SUCCESS;
}

dx_result dx_ui_text_get_text_color(DX_RGBA_F32* RETURN, dx_ui_text* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->text_color;
  return DX_SUCCESS;
}

dx_result dx_ui_text_set_text(dx_ui_text* SELF, dx_string* text) {
  return dx_text_document_set_text(SELF->text, text);
}

dx_result dx_ui_text_append_text(dx_ui_text* SELF, dx_string* text) {
  return dx_text_document_append_text(SELF->text, text);
}

static dx_result render(dx_ui_text* SELF, dx_f32 canvas_horizontal_size, dx_f32 canvas_vertical_size, dx_f32 dpi_horizontal, dx_f32 dpi_vertical) {
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

  // The distance from one baseline to the previous/next baseline multiplied by 1.5.
  dx_f32 baseline_distance;
  dx_font_get_baseline_distance(&baseline_distance, SELF->font);
  baseline_distance = baseline_distance * 1.5f;

  dx_size n;
  if (dx_object_array_get_size(&n, SELF->text->lines)) {
    return DX_FAILURE;
  }

  DX_VEC2_F32 p;
  if (get_absolute_position(&p, SELF)) {
    return DX_FAILURE;
  }

  for (dx_size i = n; i > 0; --i) {
    dx_text_range* line = NULL;
    if (dx_object_array_get_at((dx_object**)&line, SELF->text->lines, i - 1)) {
      return DX_FAILURE;
    }
    void const* bytes = NULL;
    if (dx_string_buffer_get_bytes(&bytes, SELF->text->text)) {
      return DX_FAILURE;
    }
    dx_size number_of_bytes;
    if (dx_string_buffer_get_number_of_bytes(&number_of_bytes, SELF->text->text)) {
      return DX_FAILURE;
    }
    dx_string* string;
    if (dx_string_create(&string, ((char const*)bytes) + line->start, line->length)) {
      return DX_FAILURE;
    }
    dx_font_presenter_render_line_string(DX_UI_WIDGET(SELF)->manager->font_presenter, 
                                         p.e[0], p.e[1], string, &SELF->text_color, SELF->font, PRESENTATION_POLICY, UNKNOWN_SYMBOL_POLICY);
    DX_UNREFERENCE(string);
    string = NULL;
    p.e[1] += baseline_distance;
  }
  return DX_SUCCESS;
}

dx_result dx_ui_text_get_font(dx_font** RETURN, dx_ui_text* SELF) {
  return DX_SUCCESS;
}