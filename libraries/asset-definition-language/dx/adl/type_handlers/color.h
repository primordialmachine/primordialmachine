#if !defined(DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.color",
                       dx_adl_type_handlers_color,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_color* DX_ADL_TYPE_HANDLERS_COLOR(void* p) {
  return (dx_adl_type_handlers_color*)p;
}

struct dx_adl_type_handlers_color {
  dx_adl_type_handler _parent;
};

static inline dx_adl_type_handlers_color_dispatch* DX_ADL_TYPE_HANDLERS_COLOR_DISPATCH(void* p) {
  return (dx_adl_type_handlers_color_dispatch*)p;
}

struct dx_adl_type_handlers_color_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_color_construct(dx_adl_type_handlers_color* SELF);

dx_result dx_adl_type_handlers_color_create(dx_adl_type_handlers_color** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED
