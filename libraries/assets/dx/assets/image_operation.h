#if !defined(DX_ASSET_IMAGE_OPERATION_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATION_H_INCLUDED

#include "dx/assets/color_rgb_n8.h"

DX_DECLARE_OBJECT_TYPE("dx.asset.image_operation",
                       dx_asset_image_operation,
                       dx_object);

static inline dx_asset_image_operation* DX_ASSET_IMAGE_OPERATION(void* p) {
  return (dx_asset_image_operation*)p;
}

struct dx_asset_image_operation {
  dx_object _parent;
};

static inline dx_asset_image_operation_dispatch* DX_ASSET_IMAGE_OPERATION_DISPATCH(void* p) {
  return (dx_asset_image_operation_dispatch*)p;
}

struct dx_asset_image_operation_dispatch {
  dx_object_dispatch _parent;
};

int dx_asset_image_operation_construct(dx_asset_image_operation* self);

#endif // DX_ASSET_IMAGE_OPERATION_H_INCLUDED
