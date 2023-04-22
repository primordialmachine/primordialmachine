#if !defined(DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.image_operations.color_fill_reader",
                       dx_adl_type_handlers_image_operations_color_fill,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_image_operations_color_fill* DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL(void* p) {
  return (dx_adl_type_handlers_image_operations_color_fill*)p;
}

struct dx_adl_type_handlers_image_operations_color_fill {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_image_operations_color_fill_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_image_operations_color_fill_construct(dx_adl_type_handlers_image_operations_color_fill* self);

dx_adl_type_handlers_image_operations_color_fill* dx_adl_type_handlers_image_operations_color_fill_create();

#endif // DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED
