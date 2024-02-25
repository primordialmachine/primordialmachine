// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_ORTHOGRAPHICOPTICS_H_INCLUDED)
#define CORE_ASSETS_ORTHOGRAPHICOPTICS_H_INCLUDED

#include "Core/Assets/Optics.h"

/* http://localhost/assets#core-assets-orthographicoptics */
Core_declareObjectType("Core.Assets.OrthographicOptics",
                       Core_Assets_OrthographicOptics,
                       Core_Assets_Optics);

static inline Core_Assets_OrthographicOptics* CORE_ASSETS_ORTHOGRAPHICOPTICS(void* p) {
  return (Core_Assets_OrthographicOptics*)p;
}

#if Core_OperatingSystem_Windows == Core_OperatingSystem
#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far
#endif

struct Core_Assets_OrthographicOptics {
  Core_Assets_Optics _parent;
  /// @brief The aspect ratio.
  /// The default value is NULL.
  Core_Real32* aspectRatio;
  /// @brief The scaling along the x-axis applied to the rectangle the points are projected into.
  /// The default value is 1.0.
  /// If not specified, scaleX is assigned the actual width of the viewport.
  Core_Real32 *scaleX;
  /// @brief The scaling along the y-axis applied to the rectangle the points are projected into.
  /// The default value is 1.0.
  /// If not specified, scaleY is assigned the actual height of the viewport.
  Core_Real32 *scaleY;
  /// @brief The default values are 0.1.
  Core_Real32 near;
  /// @brief The default value is 1000.0.
  Core_Real32 far;
};

#if Core_OperatingSystem_Windows == Core_OperatingSystem
#pragma pop_macro("near")
#pragma pop_macro("far")
#endif

static inline Core_Assets_OrthographicOptics_Dispatch* CORE_ASSETS_ORTHOGRAPHICOPTICS_DISPATCH(void* p) {
  return (Core_Assets_OrthographicOptics_Dispatch*)p;
}

struct Core_Assets_OrthographicOptics_Dispatch {
  Core_Assets_Optics_Dispatch _parent;
};

/// @constructor{Core_Assets_OrthographicOptics}
Core_Result Core_Assets_OrthographicOptics_construct(Core_Assets_OrthographicOptics* SELF);

/// @create-operator{Core_Assets_OrthographicOptics}
Core_Result Core_Assets_OrthographicOptics_create(Core_Assets_OrthographicOptics** RETURN);

#endif // CORE_ASSETS_ORTHOGRAPHICOPTICS_H_INCLUDED
