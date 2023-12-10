#if !defined(DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.viewer_instance",
                       dx_adl_type_handlers_viewer_instance,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_viewer_instance* DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE(void* p) {
  return (dx_adl_type_handlers_viewer_instance*)p;
}

struct dx_adl_type_handlers_viewer_instance {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  dx_inline_pointer_hashmap expected_keys;
};

static inline dx_adl_type_handlers_viewer_instance_dispatch* DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_DISPATCH(void* p) {
  return (dx_adl_type_handlers_viewer_instance_dispatch*)p;
}

struct dx_adl_type_handlers_viewer_instance_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

Core_Result dx_adl_type_handlers_viewer_instance_construct(dx_adl_type_handlers_viewer_instance* SELF);

Core_Result dx_adl_type_handlers_viewer_instance_create(dx_adl_type_handlers_viewer_instance** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_H_INCLUDED
