#if !defined(DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.mesh_operations",
                       dx_adl_type_handlers_mesh_operations,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_mesh_operations* DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS(void* p) {
  return (dx_adl_type_handlers_mesh_operations*)p;
}

struct dx_adl_type_handlers_mesh_operations {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  dx_inline_pointer_hashmap expected_keys;
};

static inline dx_adl_type_handlers_mesh_operations_dispatch* DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_DISPATCH(void* p) {
  return (dx_adl_type_handlers_mesh_operations_dispatch*)p;
}

struct dx_adl_type_handlers_mesh_operations_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_mesh_operations_construct(dx_adl_type_handlers_mesh_operations* SELF);

dx_result dx_adl_type_handlers_mesh_operations_create(dx_adl_type_handlers_mesh_operations** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED
