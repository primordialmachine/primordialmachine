#if !defined(DX_ADL_TYPE_HANDLERS_MESH_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MESH_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.mesh",
                       dx_adl_type_handlers_mesh,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_mesh* DX_ADL_TYPE_HANDLERS_MESH(void* p) {
  return (dx_adl_type_handlers_mesh*)p;
}

struct dx_adl_type_handlers_mesh {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_mesh_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_mesh_construct(dx_adl_type_handlers_mesh* self);

dx_adl_type_handlers_mesh* dx_adl_type_handlers_mesh_create();

#endif // DX_ADL_TYPE_HANDLERS_MESH_H_INCLUDED
