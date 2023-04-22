#if !defined(DX_ADL_TYPE_HANDLERS_MATERIAL_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MATERIAL_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.material",
                       dx_adl_type_handlers_material,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_material* DX_ADL_TYPE_HANDLERS_MATERIAL(void* p) {
  return (dx_adl_type_handlers_material*)p;
}

struct dx_adl_type_handlers_material {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_material_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* self);

dx_adl_type_handlers_material* dx_adl_type_handlers_material_create();

#endif // DX_ADL_TYPE_HANDLERS_MATERIAL_H_INCLUDED
