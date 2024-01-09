#if !defined(DX_ASSETS_IMAGE_OPERATION_H_INCLUDED)
#define DX_ASSETS_IMAGE_OPERATION_H_INCLUDED

#include "dx/assets/color_rgb_n8.h"

/// @brief An image operation asset.
Core_declareObjectType("dx.assets.image_operation",
                       dx_assets_image_operation,
                       Core_Object);

static inline dx_assets_image_operation* DX_ASSETS_IMAGE_OPERATION(void* p) {
  return (dx_assets_image_operation*)p;
}

struct dx_assets_image_operation {
  Core_Object _parent;
};

static inline dx_assets_image_operation_Dispatch* DX_ASSETS_IMAGE_OPERATION_DISPATCH(void* p) {
  return (dx_assets_image_operation_Dispatch*)p;
}

struct dx_assets_image_operation_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_assets_image_operation_construct(dx_assets_image_operation* SELF);

#endif // DX_ASSETS_IMAGE_OPERATION_H_INCLUDED
