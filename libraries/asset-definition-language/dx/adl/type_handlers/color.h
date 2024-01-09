#if !defined(DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.color",
                       dx_adl_type_handlers_color,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_color* DX_ADL_TYPE_HANDLERS_COLOR(void* p) {
  return (dx_adl_type_handlers_color*)p;
}

struct dx_adl_type_handlers_color {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlinePointerHashmap expected_keys;
};

static inline dx_adl_type_handlers_color_Dispatch* DX_ADL_TYPE_HANDLERS_COLOR_DISPATCH(void* p) {
  return (dx_adl_type_handlers_color_Dispatch*)p;
}

struct dx_adl_type_handlers_color_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_color_construct(dx_adl_type_handlers_color* SELF);

Core_Result dx_adl_type_handlers_color_create(dx_adl_type_handlers_color** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED
