#if !defined(DX_ADL_TYPE_HANDLERS_VIEWER_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_VIEWER_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.viewer_reader",
                       dx_adl_type_handlers_viewer,
                       dx_adl_type_handler);
                       
static inline dx_adl_type_handlers_viewer* DX_ADL_TYPE_HANDLERS_VIEWER(void* p) {
  return (dx_adl_type_handlers_viewer*)p;
}

struct dx_adl_type_handlers_viewer {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlinePointerHashmap expected_keys;
};

static inline dx_adl_type_handlers_viewer_Dispatch* DX_ADL_TYPE_HANDLERS_VIEWER_DISPATCH(void* p) {
  return (dx_adl_type_handlers_viewer_Dispatch*)p;
}

struct dx_adl_type_handlers_viewer_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_viewer_construct(dx_adl_type_handlers_viewer* SELF);

Core_Result dx_adl_type_handlers_viewer_create(dx_adl_type_handlers_viewer** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_VIEWER_H_INCLUDED
