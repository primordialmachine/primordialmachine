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
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  dx_inline_pointer_hashmap expected_keys;
};

static inline dx_adl_type_handlers_image_operations_color_fill_dispatch* DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL_DISPATCH(void* p) {
  return (dx_adl_type_handlers_image_operations_color_fill_dispatch*)p;
}

struct dx_adl_type_handlers_image_operations_color_fill_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

Core_Result dx_adl_type_handlers_image_operations_color_fill_construct(dx_adl_type_handlers_image_operations_color_fill* SELF);

Core_Result dx_adl_type_handlers_image_operations_color_fill_create(dx_adl_type_handlers_image_operations_color_fill** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL_H_INCLUDED
