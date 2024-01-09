#if !defined(DX_ASSETS_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_H_INCLUDED)
#define DX_ASSETS_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Mirrors an image horizontally.
/// Let \f$w geq 0\f$ and \f$h \geq 0\f$ be the width of the height of the image, respectively,
/// then the pixel at \f$(x,y)\f$ with \f$x \in [1,w]\f$ and \f$y \in [1,h]\f$ is swapped with the pixel at \f$(w-x,y)\f$. 
Core_declareObjectType("dx.assets.image_operations.mirror_horizontal",
                       dx_asset_image_operations_mirror_horizontal,
                       dx_assets_image_operation)

static inline dx_asset_image_operations_mirror_horizontal* DX_ASSETS_IMAGE_OPERATIONS_MIRROR_HORIZONTAL(void* p) {
  return (dx_asset_image_operations_mirror_horizontal*)p;
}

struct dx_asset_image_operations_mirror_horizontal {
  dx_assets_image_operation _parent;
};

static inline dx_asset_image_operations_mirror_horizontal_Dispatch* DX_ASSETS_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_DISPATCH(void* p) {
  return (dx_asset_image_operations_mirror_horizontal_Dispatch*)p;
}

struct dx_asset_image_operations_mirror_horizontal_Dispatch {
  dx_assets_image_operation_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{dx_assets_image_operations_mirror_horizontal}
Core_Result dx_asset_image_operations_mirror_horizontal_construct(dx_asset_image_operations_mirror_horizontal * SELF);

/// @create-operator{dx_assets_image_operations_mirror_horizontal}
Core_Result dx_asset_image_operations_mirror_horizontal_create(dx_asset_image_operations_mirror_horizontal** RETURN);

#endif // DX_ASSETS_IMAGE_OPERATIONS_MIRROR_HORIZONTAL_H_INCLUDED
