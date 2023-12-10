#if !defined(DX_ASSETS_IMAGE_OPERATIONS_MIRROR_VERTICAL_H_INCLUDED)
#define DX_ASSETS_IMAGE_OPERATIONS_MIRROR_VERTICAL_H_INCLUDED

#include "dx/assets/image_operation.h"
#include "dx/assets/reference.h"

/// @extends dx_assets_image_operation
/// @brief An image operation.
/// Mirrors an image vertically.
/// Let \f$w geq 0\f$ and \f$h \geq 0\f$ be the width of the height of the image, respectively,
/// then the pixel at \f$(x,y)\f$ with \f$x \in [1,w]\f$ and \f$y \in [1,h]\f$ is swapped with the pixel at \f$(x,h-y)\f$. 
DX_DECLARE_OBJECT_TYPE("dx.assets.image_operations.mirror_vertical",
                       dx_assets_image_operations_mirror_vertical,
                       dx_assets_image_operation)

static inline dx_assets_image_operations_mirror_vertical* DX_ASSETS_IMAGE_OPERATIONS_MIRROR_VERTICAL(void* p) {
  return (dx_assets_image_operations_mirror_vertical*)p;
}

struct dx_assets_image_operations_mirror_vertical {
  dx_assets_image_operation _parent;
};

static inline dx_assets_image_operations_mirror_vertical_dispatch* DX_ASSETS_IMAGE_OPERATIONS_MIRROR_VERTICAL_DISPATCH(void* p) {
  return (dx_assets_image_operations_mirror_vertical_dispatch*)p;
}

struct dx_assets_image_operations_mirror_vertical_dispatch {
  dx_assets_image_operation_dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{dx_assets_image_operations_mirror_vertical}
Core_Result dx_assets_image_operations_mirror_vertical_construct(dx_assets_image_operations_mirror_vertical* SELF);

/// @create-operator{dx_assets_image_operations_mirror_vertical}
Core_Result dx_assets_image_operations_mirror_vertical_create(dx_assets_image_operations_mirror_vertical** RETURN);

#endif // DX_ASSETS_IMAGE_OPERATIONS_MIRROR_VERTICAL_H_INCLUDED
