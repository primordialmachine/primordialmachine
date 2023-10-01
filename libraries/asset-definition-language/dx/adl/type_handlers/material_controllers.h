#if !defined(DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.material_controllers",
                       dx_adl_type_handlers_material_controllers,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_material_controllers* DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS(void* p) {
  return (dx_adl_type_handlers_material_controllers*)p;
}

struct dx_adl_type_handlers_material_controllers {
  dx_adl_type_handler _parent;
};

static inline dx_adl_type_handlers_material_controllers_dispatch* DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_DISPATCH(void* p) {
  return (dx_adl_type_handlers_material_controllers_dispatch*)p;
}

struct dx_adl_type_handlers_material_controllers_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_material_controllers_construct(dx_adl_type_handlers_material_controllers* SELF);

dx_result dx_adl_type_handlers_material_controllers_create(dx_adl_type_handlers_material_controllers** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_H_INCLUDED
