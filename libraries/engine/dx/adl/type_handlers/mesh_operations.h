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
};

struct dx_adl_type_handlers_mesh_operations_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_mesh_operations_construct(dx_adl_type_handlers_mesh_operations* self);

dx_adl_type_handlers_mesh_operations* dx_adl_type_handlers_mesh_operations_create();

#endif // DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS_H_INCLUDED
