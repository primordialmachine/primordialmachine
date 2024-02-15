#if !defined(CORE_ASSETS_IMAGEOPERATIONS_MIRRORHORIZONTAL_H_INCLUDED)
#define CORE_ASSETS_IMAGEOPERATIONS_MIRRORHORIZONTAL_H_INCLUDED

#include "Core/Assets/ImageOperation.h"
#include "Core/Assets/Ref.h"

/// @extends Core_Assets_ImageOperation
/// @brief An image operation.
/// Mirrors an image horizontally.
/// Let \f$w geq 0\f$ and \f$h \geq 0\f$ be the width of the height of the image, respectively,
/// then the pixel at \f$(x,y)\f$ with \f$x \in [1,w]\f$ and \f$y \in [1,h]\f$ is swapped with the pixel at \f$(w-x,y)\f$. 
Core_declareObjectType("Core.Assets.ImageOperations.MirrorHorizontal",
                       Core_Assets_ImageOperations_MirrorHorizontal,
                       Core_Assets_ImageOperation);

static inline Core_Assets_ImageOperations_MirrorHorizontal* CORE_ASSETS_IMAGEOPERATIONS_MIRRORHORIZONTAL(void* p) {
  return (Core_Assets_ImageOperations_MirrorHorizontal*)p;
}

struct Core_Assets_ImageOperations_MirrorHorizontal {
  Core_Assets_ImageOperation _parent;
};

static inline Core_Assets_ImageOperations_MirrorHorizontal_Dispatch* CORE_ASSET_IMAGEOPERATIONS_MIRRORHORIZONTAL_DISPATCH(void* p) {
  return (Core_Assets_ImageOperations_MirrorHorizontal_Dispatch*)p;
}

struct Core_Assets_ImageOperations_MirrorHorizontal_Dispatch {
  Core_Assets_ImageOperation_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{Core_Assets_ImageOperations_MirrorHorizontal}
Core_Result Core_Assets_ImageOperations_MirrorHorizontal_construct(Core_Assets_ImageOperations_MirrorHorizontal * SELF);

/// @create-operator{Core_Assets_ImageOperations_MirrorHorizontal}
Core_Result Core_Assets_ImageOperations_MirrorHorizontal_create(Core_Assets_ImageOperations_MirrorHorizontal** RETURN);

#endif // CORE_ASSETS_IMAGEOPERATIONS_MIRRORHORIZONTAL_H_INCLUDED
