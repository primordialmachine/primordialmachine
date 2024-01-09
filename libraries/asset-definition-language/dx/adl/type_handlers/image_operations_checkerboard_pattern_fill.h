#if !defined(DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.image_operations.checkerboard_pattern_fill",
                       dx_adl_type_handlers_image_operations_checkerboard_pattern_fill,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(void* p) {
  return (dx_adl_type_handlers_image_operations_checkerboard_pattern_fill*)p;
}

struct dx_adl_type_handlers_image_operations_checkerboard_pattern_fill {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlinePointerHashmap expected_keys;
};

static inline dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_Dispatch* DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_DISPATCH(void* p) {
  return (dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_Dispatch*)p;
}

struct dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF);

Core_Result dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_create(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL_H_INCLUDED
