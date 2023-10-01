#if !defined(DX_ADL_TYPE_HANDLERS_IMAGE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_IMAGE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.image_reader",
                       dx_adl_type_handlers_image,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_image* DX_ADL_TYPE_HANDLERS_IMAGE(void* p) {
  return (dx_adl_type_handlers_image*)p;
}

struct dx_adl_type_handlers_image {
  dx_adl_type_handler _parent;
};

static inline dx_adl_type_handlers_image_dispatch* DX_ADL_TYPE_HANDLERS_DISPATCH(void* p) {
  return (dx_adl_type_handlers_image_dispatch*)p;
}

struct dx_adl_type_handlers_image_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_image_construct(dx_adl_type_handlers_image* SELF);

dx_result dx_adl_type_handlers_image_create(dx_adl_type_handlers_image** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_IMAGE_H_INCLUDED
