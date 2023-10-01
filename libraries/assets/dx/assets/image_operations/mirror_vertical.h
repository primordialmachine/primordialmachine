#if !defined(DX_ASSET_IMAGE_OPERATIONS_MIRROR_VERTICAL_H_INCLUDED)
#define DX_ASSET_IMAGE_OPERATIONS_MIRROR_VERTICAL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_asset_image_operation
/// @brief An image operation.
/// Mirrors an image vertically.
/// Let \f$w geq 0\f$ and \f$h \geq 0\f$ be the width of the height of the image, respectively,
/// then the pixel at \f$(x,y)\f$ with \f$x \in [1,w]\f$ and \f$y \in [1,h]\f$ is swapped with the pixel at \f$(x,h-y)\f$. 
DX_DECLARE_OBJECT_TYPE("dx.asset.image_operations.mirror_vertical",
                       dx_asset_image_operations_mirror_vertical,
                       dx_asset_image_operation)

static inline dx_asset_image_operations_mirror_vertical* DX_ASSET_IMAGE_OPERATIONS_MIRROR_VERTICAL(void* p) {
  return (dx_asset_image_operations_mirror_vertical*)p;
}

struct dx_asset_image_operations_mirror_vertical {
  dx_asset_image_operation _parent;
};

struct dx_asset_image_operations_mirror_vertical_dispatch {
  dx_asset_image_operation_dispatch _parent;
};

/// @brief Construct this dx_asset_image_operations_mirror_vertical object with default values.
/// @param SELF A pointer to this dx_asset_image_operations_mirror_vertical object.
/// @method-call
dx_result dx_asset_image_operations_mirror_vertical_construct(dx_asset_image_operations_mirror_vertical* SELF);

/// @create-operator{dx_asset_image_operations_mirror_vertical}
dx_result dx_asset_image_operations_mirror_vertical_create(dx_asset_image_operations_mirror_vertical** RETURN);

#endif // DX_ASSET_IMAGE_OPERATIONS_MIRROR_VERTICAL_H_INCLUDED
