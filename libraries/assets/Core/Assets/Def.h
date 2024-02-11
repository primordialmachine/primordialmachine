#if !defined(CORE_ASSETS_DEF_H_INCLUDED)
#define CORE_ASSETS_DEF_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("Core.Assets.Def",
                       Core_Assets_Def,
                       Core_Object);

static inline Core_Assets_Def* CORE_ASSETS_DEF(void* p) {
  return (Core_Assets_Def*)p;
}

struct Core_Assets_Def {
  Core_Object _parent;
};

static inline Core_Assets_Def_Dispatch* CORE_ASSETS_DEF_DISPATCH(void* p) {
  return (Core_Assets_Def_Dispatch*)p;
}

struct Core_Assets_Def_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @detail This Core_Asset_Def object was initialized with default values.
/// @constructor{Core_Assets_Def} 
Core_Result Core_Assets_Def_construct(Core_Assets_Def* SELF);

#endif // CORE_ASSETS_DEF_H_INCLUDED
