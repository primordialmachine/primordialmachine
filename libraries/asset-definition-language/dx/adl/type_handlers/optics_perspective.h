#if !defined(DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.optics_perspective",
                       dx_adl_type_handlers_optics_perspective,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_optics_perspective* DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE(void* p) {
  return (dx_adl_type_handlers_optics_perspective*)p;
}

struct dx_adl_type_handlers_optics_perspective {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlinePointerHashmap expected_keys;
};

static inline dx_adl_type_handlers_optics_perspective_Dispatch* DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_DISPATCH(void* p) {
  return (dx_adl_type_handlers_optics_perspective_Dispatch*)p;
}

struct dx_adl_type_handlers_optics_perspective_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_optics_perspective_construct(dx_adl_type_handlers_optics_perspective* SELF);

Core_Result dx_adl_type_handlers_optics_perspective_create(dx_adl_type_handlers_optics_perspective** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_H_INCLUDED
