#if !defined(CORE_ASSETS_IMAGEOPERATIONS_MIRRORVERTICAL_H_INCLUDED)
#define CORE_ASSETS_IMAGEOPERATIONS_MIRRORVERTICAL_H_INCLUDED

#include "Core/Assets/ImageOperation.h"
#include "Core/Assets/Ref.h"

/// @extends Core_Assets_ImageOperation
/// @brief An image operation.
/// Mirrors an image vertically.
/// Let \f$w geq 0\f$ and \f$h \geq 0\f$ be the width of the height of the image, respectively,
/// then the pixel at \f$(x,y)\f$ with \f$x \in [1,w]\f$ and \f$y \in [1,h]\f$ is swapped with the pixel at \f$(x,h-y)\f$. 
Core_declareObjectType("Core.Assets.ImageOperations.MirrorVertical",
                       Core_Assets_ImageOperations_MirrorVertical,
                       Core_Assets_ImageOperation);

static inline Core_Assets_ImageOperations_MirrorVertical* CORE_ASSETS_IMAGEOPERATIONS_MIRRORVERTICAL(void* p) {
  return (Core_Assets_ImageOperations_MirrorVertical*)p;
}

struct Core_Assets_ImageOperations_MirrorVertical {
  Core_Assets_ImageOperation _parent;
};

static inline Core_Assets_ImageOperations_MirrorVertical_Dispatch* CORE_ASSETS_IMAGEOPERATIONS_MIRRORVERTICAL_DISPATCH(void* p) {
  return (Core_Assets_ImageOperations_MirrorVertical_Dispatch*)p;
}

struct Core_Assets_ImageOperations_MirrorVertical_Dispatch {
  Core_Assets_ImageOperation_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{Core_Assets_ImageOperations_MirrorVertical}
Core_Result Core_Assets_ImageOperations_MirrorVertical_construct(Core_Assets_ImageOperations_MirrorVertical* SELF);

/// @create-operator{Core_Assets_ImageOperations_MirrorVertical}
Core_Result Core_Assets_ImageOperations_MirrorVertical_create(Core_Assets_ImageOperations_MirrorVertical** RETURN);

#endif // CORE_ASSETS_IMAGEOPERATIONS_MIRRORVERTICAL_H_INCLUDED
