#if !defined(DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.mesh_instance",
                       dx_adl_type_handlers_mesh_instance,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_mesh_instance* DX_ADL_TYPE_HANDLERS_MESH_INSTANCE(void* p) {
  return (dx_adl_type_handlers_mesh_instance*)p;
}

struct dx_adl_type_handlers_mesh_instance {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlinePointerHashmap expected_keys;
};

static inline dx_adl_type_handlers_mesh_instance_Dispatch* DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_DISPATCH(void* p) {
  return (dx_adl_type_handlers_mesh_instance_Dispatch*)p;
}

struct dx_adl_type_handlers_mesh_instance_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_mesh_instance_construct(dx_adl_type_handlers_mesh_instance* SELF);

Core_Result dx_adl_type_handlers_mesh_instance_create(dx_adl_type_handlers_mesh_instance** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_H_INCLUDED
