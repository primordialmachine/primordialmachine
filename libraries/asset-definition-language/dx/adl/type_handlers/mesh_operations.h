#if !defined(DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.mesh_operations",
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
  Core_InlineHashMapPP expected_keys;
};

static inline dx_adl_type_handlers_mesh_operations_Dispatch* DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_DISPATCH(void* p) {
  return (dx_adl_type_handlers_mesh_operations_Dispatch*)p;
}

struct dx_adl_type_handlers_mesh_operations_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_mesh_operations_construct(dx_adl_type_handlers_mesh_operations* SELF);

Core_Result dx_adl_type_handlers_mesh_operations_create(dx_adl_type_handlers_mesh_operations** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED
