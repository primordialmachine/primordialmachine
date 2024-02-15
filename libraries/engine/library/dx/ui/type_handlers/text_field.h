#if !defined(DX_UI_TYPE_HANDLERS_TEXT_FIELD_H_INCLUDED)
#define DX_UI_TYPE_HANDLERS_TEXT_FIELD_H_INCLUDED

#include "dx/ui/type_handler.h"

Core_declareObjectType("dx.ui.type_handlers.text_field",
                       dx_ui_type_handlers_text_field,
                       dx_ui_type_handler);
                       
static inline dx_ui_type_handlers_text_field* DX_UI_TYPE_HANDLERS_TEXT_FIELD(void* p) {
  return (dx_ui_type_handlers_text_field*)p;
}

struct dx_ui_type_handlers_text_field {
  dx_ui_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlineHashMapPP expected_keys;
};

static inline dx_ui_type_handlers_text_field_Dispatch* DX_UI_TYPE_HANDLERS_TEXT_FIELD_DISPATCH(void* p) {
  return (dx_ui_type_handlers_text_field_Dispatch*)p;
}

struct dx_ui_type_handlers_text_field_Dispatch {
  dx_ui_type_handler_Dispatch _parent;
};

Core_Result dx_ui_type_handlers_text_field_construct(dx_ui_type_handlers_text_field* SELF);

Core_Result dx_ui_type_handlers_text_field_create(dx_ui_type_handlers_text_field** RETURN);

#endif // DX_UI_TYPE_HANDLERS_TEXT_FIELD_H_INCLUDED
