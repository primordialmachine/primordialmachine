#if !defined(DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.optics_perspective",
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
  dx_inline_pointer_hashmap expected_keys;
};

static inline dx_adl_type_handlers_optics_perspective_dispatch* DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_DISPATCH(void* p) {
  return (dx_adl_type_handlers_optics_perspective_dispatch*)p;
}

struct dx_adl_type_handlers_optics_perspective_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_optics_perspective_construct(dx_adl_type_handlers_optics_perspective* SELF);

dx_result dx_adl_type_handlers_optics_perspective_create(dx_adl_type_handlers_optics_perspective** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_OPTICS_PERSPECTIVE_H_INCLUDED