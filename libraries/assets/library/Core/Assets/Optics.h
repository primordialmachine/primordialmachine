// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_OPTICS_H_INCLUDED)
#define CORE_ASSETS_OPTICS_H_INCLUDED

#include "Core/Assets/Def.h"

/* http://localhost/assets#core-assets-optics */
Core_declareObjectType("Core.Assets.Optics",
                       Core_Assets_Optics,
                       Core_Object);

static inline Core_Assets_Optics* CORE_ASSETS_OPTICS(void* p) {
  return (Core_Assets_Optics*)p;
}

struct Core_Assets_Optics {
  Core_Assets_Def _parent;
};

static inline Core_Assets_Optics_Dispatch* CORE_ASSETS_OPTICS_DISPATCH(void* p) {
  return (Core_Assets_Optics_Dispatch*)p;
}

struct Core_Assets_Optics_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @constructor{Core_Assets_Optics}
Core_Result Core_Assets_Optics_construct(Core_Assets_Optics* SELF);

#endif // CORE_ASSETS_OPTICS_H_INCLUDED
