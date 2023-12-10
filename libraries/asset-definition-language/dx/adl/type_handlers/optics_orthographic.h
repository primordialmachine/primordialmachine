#if !defined(DX_ADL_TYPE_HANDLERS_OPTICS_ORTHOGRAPHIC_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_OPTICS_ORTHOGRAPHIC_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.optics_orthographic",
                       dx_adl_type_handlers_optics_orthographic,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_optics_orthographic* DX_ADL_TYPE_HANDLERS_OPTICS_ORTHOGRAPHIC(void* p) {
  return (dx_adl_type_handlers_optics_orthographic*)p;
}

struct dx_adl_type_handlers_optics_orthographic {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  dx_inline_pointer_hashmap expected_keys;
};

static inline dx_adl_type_handlers_optics_orthographic_dispatch* DX_ADL_TYPE_HANDLERS_OPTICS_ORTHOGRAPHIC_DISPATCH(void* p) {
  return (dx_adl_type_handlers_optics_orthographic_dispatch*)p;
}

struct dx_adl_type_handlers_optics_orthographic_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

Core_Result dx_adl_type_handlers_optics_orthographic_construct(dx_adl_type_handlers_optics_orthographic* SELF);

Core_Result dx_adl_type_handlers_optics_orthographic_create(dx_adl_type_handlers_optics_orthographic** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_OPTICS_ORTHOGRAPHIC_H_INCLUDED
