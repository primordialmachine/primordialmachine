#if !defined(DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.color",
                       dx_adl_type_handlers_color,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_color* DX_ADL_SEMANTICAL_COLOR_READER(void* p) {
  return (dx_adl_type_handlers_color*)p;
}

struct dx_adl_type_handlers_color {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_color_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_color_construct(dx_adl_type_handlers_color* self);

dx_adl_type_handlers_color* dx_adl_type_handlers_color_create();

#endif // DX_ADL_TYPE_HANDLERS_COLOR_H_INCLUDED
