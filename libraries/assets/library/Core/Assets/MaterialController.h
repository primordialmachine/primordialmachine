// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_MATERIALCONTROLLER_H_INCLUDED)
#define CORE_ASSETS_MATERIALCONTROLLER_H_INCLUDED

#include "Core/Assets/Def.h"
typedef struct Core_Assets_Material Core_Assets_Material;

Core_declareObjectType("Core.Assets.MaterialController",
                       Core_Assets_MaterialController,
                       Core_Assets_Def);

static inline Core_Assets_MaterialController* CORE_ASSETS_MATERIAL_CONTROLLER(void* p) {
  return (Core_Assets_MaterialController*)p;
}

struct Core_Assets_MaterialController {
  Core_Assets_Def _parent;
};

static inline Core_Assets_MaterialController_Dispatch* CORE_ASSETS_MATERIAL_CONTROLLER_DISPATCH(void* p) {
  return (Core_Assets_MaterialController_Dispatch*)p;
}

struct Core_Assets_MaterialController_Dispatch {
  Core_Assets_Def_Dispatch parent;
  Core_Result (*update)(Core_Assets_MaterialController* SELF, Core_Assets_Material* material, Core_Real32 delta_seconds);
};

/// @constructor{Core_Assets_MaterialController}
Core_Result Core_Assets_MaterialController_construct(Core_Assets_MaterialController* SELF);

static inline Core_Result Core_Assets_MaterialController_update(Core_Assets_MaterialController* SELF, Core_Assets_Material* material, Core_Real32 delta_seconds) {
  DX_OBJECT_VIRTUALCALL(Core_Assets_MaterialController, update, SELF, material, delta_seconds);
}

#endif // CORE_ASSETS_MATERIALCONTROLLER_H_INCLUDED
