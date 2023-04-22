#if !defined(DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.mesh_instance",
                       dx_adl_type_handlers_mesh_instance,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_mesh_instance* DX_ADL_TYPE_HANDLERS_MESH_INSTANCE(void* p) {
  return (dx_adl_type_handlers_mesh_instance*)p;
}

struct dx_adl_type_handlers_mesh_instance {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_mesh_instance_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_mesh_instance_construct(dx_adl_type_handlers_mesh_instance* self);

dx_adl_type_handlers_mesh_instance* dx_adl_type_handlers_mesh_instance_create();

#endif // DX_ADL_TYPE_HANDLERS_MESH_INSTANCE_H_INCLUDED
