// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_ASSETS_PERSPECTIVEOPTICS_H_INCLUDED)
#define CORE_ASSETS_PERSPECTIVEOPTICS_H_INCLUDED

#include "Core/Assets/Optics.h"

/* http://localhost/assets#core-assets-perspectiveoptics */
Core_declareObjectType("Core.Assets.PerspectiveOptics",
                       Core_Assets_PerspectiveOptics,
                       Core_Assets_Optics);

static inline Core_Assets_PerspectiveOptics* CORE_ASSETS_PERSPECTIVEOPTICS(void* p) {
  return (Core_Assets_PerspectiveOptics*)p;
}

#if Core_OperatingSystem_Windows == Core_OperatingSystem
#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far
#endif

struct Core_Assets_PerspectiveOptics {
  Core_Assets_Optics _parent;
  /// @brief The aspect ratio (the quotient of the width and the height of near plane).
  /// The default is 4:3 = 1.33... .
  /// If not provided, it is computed from the actual width and the actual height of the viewport.
  Core_Real32* aspect_ratio;
  /// @brief The field of yiew angle in degrees.
  /// The angle between
  /// - a plane through the origin and the bottom of the near plane
  /// - a plane through the origin and the top of the near plane.
  /// The default is 60.0.
  Core_Real32 field_of_view_y;
  /// @brief The default value is 0.1.
  Core_Real32 near;
  /// @brief The default value is 1000.0.
  Core_Real32 far;
};

#if Core_OperatingSystem_Windows == Core_OperatingSystem
#pragma pop_macro("near")
#pragma pop_macro("far")
#endif

static inline Core_Assets_PerspectiveOptics_Dispatch* CORE_ASSETS_PERSPECTIVEOPTICS_DISPATCH(void* p) {
  return (Core_Assets_PerspectiveOptics_Dispatch*)p;
}

struct Core_Assets_PerspectiveOptics_Dispatch {
  Core_Assets_Optics_Dispatch _parent;
};

/// @constructor{Core_Assets_PerspectiveOptics}
Core_Result Core_Assets_PerspectiveOptics_construct(Core_Assets_PerspectiveOptics* SELF);

/// @create-operator{Core_Assets_PerspectiveOptics}
Core_Result Core_Assets_PerspectiveOptics_create(Core_Assets_PerspectiveOptics** RETURN);

#endif // CORE_ASSETS_PERSPECTIVEOPTICS_H_INCLUDED
