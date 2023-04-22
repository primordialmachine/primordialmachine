#if !defined(DX_ADL_TYPE_HANDLERS_TEXTURE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_TEXTURE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.texture",
                       dx_adl_type_handlers_texture,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_texture* DX_ADL_TYPE_HANDLERS_TEXTURE(void* p) {
  return (dx_adl_type_handlers_texture*)p;
}

struct dx_adl_type_handlers_texture {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_texture_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_texture_construct(dx_adl_type_handlers_texture* self);

dx_adl_type_handlers_texture* dx_adl_type_handlers_texture_create();

#endif // DX_ADL_TYPE_HANDLERS_TEXTURE_H_INCLUDED
