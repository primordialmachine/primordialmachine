#if !defined(CORE_ASSETS_IMAGEOPERATION_H_INCLUDED)
#define CORE_ASSETS_IMAGEOPERATION_H_INCLUDED

#include "Core/Assets/Def.h"

/// @brief The base of Core.Assets.ImageOperation derived type objects.
Core_declareObjectType("Core.Assets.ImageOperation",
                       Core_Assets_ImageOperation,
                       Core_Object);

static inline Core_Assets_ImageOperation* CORE_ASSETS_IMAGEOPERATION(void* p) {
  return (Core_Assets_ImageOperation*)p;
}

struct Core_Assets_ImageOperation {
  Core_Object _parent;
};

static inline Core_Assets_ImageOperation_Dispatch* CORE_ASSETS_IMAGEOPERATION_DISPATCH(void* p) {
  return (Core_Assets_ImageOperation_Dispatch*)p;
}

struct Core_Assets_ImageOperation_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_Assets_ImageOperation_construct(Core_Assets_ImageOperation* SELF);

#endif // CORE_ASSETS_IMAGEOPERATION_H_INCLUDED
