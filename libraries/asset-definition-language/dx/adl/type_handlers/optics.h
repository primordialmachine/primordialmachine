#if !defined(DX_ADL_TYPE_HANDLERS_OPTICS_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_OPTICS_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.optics",
                       dx_adl_type_handlers_optics,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_optics* DX_ADL_TYPE_HANDLERS_OPTICS(void* p) {
  return (dx_adl_type_handlers_optics*)p;
}

struct dx_adl_type_handlers_optics {
  dx_adl_type_handler _parent;
};

static inline dx_adl_type_handlers_optics_dispatch* DX_ADL_TYPE_HANDLERS_OPTICS_DISPATCH(void* p) {
  return (dx_adl_type_handlers_optics_dispatch*)p;
}

struct dx_adl_type_handlers_optics_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_optics_construct(dx_adl_type_handlers_optics* SELF);

dx_result dx_adl_type_handlers_optics_create(dx_adl_type_handlers_optics** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_OPTICS_H_INCLUDED