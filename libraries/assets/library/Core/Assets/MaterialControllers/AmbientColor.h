// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_MATERIALCONTROLLERS_AMBIENTCOLOR_H_INCLUDED)
#define CORE_ASSETS_MATERIALCONTROLLERS_AMBIENTCOLOR_H_INCLUDED

#include "Core/Assets/MaterialController.h"

/// @brief Controller that interpolates the ambient color of a material.
Core_declareObjectType("Core.Assets.MaterialControllers.AmbientColor",
                       Core_Assets_MaterialControllers_AmbientColor,
                       Core_Assets_MaterialController);

static inline Core_Assets_MaterialControllers_AmbientColor* CORE_ASSETS_MATERIALCONTROLLERS_AMBIENTCOLOR(void* p) {
  return (Core_Assets_MaterialControllers_AmbientColor*)p;
}

struct Core_Assets_MaterialControllers_AmbientColor {
  Core_Assets_MaterialController _parent;
  /// @brief The current time.
  Core_Real32 time;
  /// @brief The current color.
  Core_InlineRgbN8 current;
};

static inline Core_Assets_MaterialControllers_AmbientColor_Dispatch* CORE_ASSETS_MATERIALCONTROLLERS_AMBIENTCOLOR_DISPATCH(void* p) {
  return (Core_Assets_MaterialControllers_AmbientColor_Dispatch*)p;
}

struct Core_Assets_MaterialControllers_AmbientColor_Dispatch {
  Core_Assets_MaterialController_Dispatch _parent;
};

/// @detail
/// The object is constructed with default values.
/// The default values are start <code>black</code>, end <code>white</code>, speed <code>0.1 per second</code>, and position <code>0</code>.
/// @constructor{Core_Assets_MaterialControllers_AmbientColor}
Core_Result Core_Assets_MaterialControllers_AmbientColor_construct(Core_Assets_MaterialControllers_AmbientColor* SELF);

/// @create-operator{Core_Assets_MaterialControllers_AmbientColor}
Core_Result Core_Assets_MaterialControllers_AmbientColor_create(Core_Assets_MaterialControllers_AmbientColor** RETURN);

#endif // CORE_ASSETS_MATERIALCONTROLLERS_AMBIENTCOLOR_H_INCLUDED
