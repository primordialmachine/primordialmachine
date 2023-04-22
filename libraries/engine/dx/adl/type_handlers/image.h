#if !defined(DX_ADL_TYPE_HANDLERS_IMAGE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_IMAGE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.image_reader",
                       dx_adl_type_handlers_image,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_image* DX_ADL_SEMANTICAL_IMAGE_READER(void* p) {
  return (dx_adl_type_handlers_image*)p;
}

struct dx_adl_type_handlers_image {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_image_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_image_construct(dx_adl_type_handlers_image* self);

dx_adl_type_handlers_image* dx_adl_type_handlers_image_create();

#endif // DX_ADL_TYPE_HANDLERS_IMAGE_H_INCLUDED
