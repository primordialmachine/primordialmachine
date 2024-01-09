#if !defined(DX_ASSETS_OPTICS_H_INCLUDED)
#define DX_ASSETS_OPTICS_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.assets.optics",
                       dx_assets_optics,
                       Core_Object);

static inline dx_assets_optics* DX_ASSETS_OPTICS(void* p) {
  return (dx_assets_optics*)p;
}

struct dx_assets_optics {
  Core_Object _parent;
};

static inline dx_assets_optics_Dispatch* DX_ASSETS_OPTICS_DISPATCH(void* p) {
  return (dx_assets_optics_Dispatch*)p;
}

struct dx_assets_optics_Dispatch {
  Core_Object _parent;
};

/// @constructor{dx_assets_optics}
Core_Result dx_assets_optics_construct(dx_assets_optics* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.asset.optics_orthographic",
                       dx_asset_optics_orthographic,
                       dx_asset_optics);

static inline dx_asset_optics_orthographic* DX_ASSET_OPTICS_ORTHOGRAPHIC(void* p) {
  return (dx_asset_optics_orthographic*)p;
}

#if DX_CONFIGURATION_SYSTEM == DX_CONFIGURATION_SYSTEM_WINDOWS
#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far
#endif

struct dx_asset_optics_orthographic {
  dx_assets_optics _parent;
  /// @brief The aspect ratio.
  /// The default value is NULL.
  Core_Real32* aspect_ratio;
  /// @brief The scaling along the x-axis applied to the rectangle the points are projected into.
  /// The default value is 1.0.
  /// If not specified, scale_y is assigned the actual width of the viewport.
  Core_Real32 *scale_x;
  /// @brief The scaling along the y-axis applied to the rectangle the points are projected into.
  /// The default value is 1.0.
  /// If not specified, scale_y is assigned the actual height of the viewport.
  Core_Real32 *scale_y;
  /// @brief The default values are 0.1.
  Core_Real32 near;
  /// @brief The default value is 1000.0.
  Core_Real32 far;
};

#if DX_CONFIGURATION_SYSTEM == DX_CONFIGURATION_SYSTEM_WINDOWS
#pragma pop_macro("near")
#pragma pop_macro("far")
#endif

static inline dx_asset_optics_orthographic_Dispatch* DX_ASSET_OPTICS_ORTHOGRAPHIC_DISPATCH(void* p) {
  return (dx_asset_optics_orthographic_Dispatch*)p;
}

struct dx_asset_optics_orthographic_Dispatch {
  dx_assets_optics_Dispatch _parent;
};

/// @constructor{dx_asset_optics_orthographic}
Core_Result dx_asset_optics_orthographic_construct(dx_asset_optics_orthographic* SELF);

/// @create-operator{dx_asset_optics_orthographic}
Core_Result dx_asset_optics_orthographic_create(dx_asset_optics_orthographic** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.asset.optics_perspective",
                       dx_asset_optics_perspective,
                       dx_asset_optics);

static inline dx_asset_optics_perspective* DX_ASSET_OPTICS_PERSPECTIVE(void* p) {
  return (dx_asset_optics_perspective*)p;
}

#if DX_CONFIGURATION_SYSTEM == DX_CONFIGURATION_SYSTEM_WINDOWS
#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far
#endif

struct dx_asset_optics_perspective {
  dx_assets_optics _parent;
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

#if DX_CONFIGURATION_SYSTEM == DX_CONFIGURATION_SYSTEM_WINDOWS
#pragma pop_macro("near")
#pragma pop_macro("far")
#endif

static inline dx_asset_optics_perspective_Dispatch* DX_ASSET_OPTICS_PERSPECTIVE_DISPATCH(void* p) {
  return (dx_asset_optics_perspective_Dispatch*)p;
}

struct dx_asset_optics_perspective_Dispatch {
  dx_assets_optics_Dispatch _parent;
};

/// @constructor{dx_asset_optics_perspective}
Core_Result dx_asset_optics_perspective_construct(dx_asset_optics_perspective* SELF);

/// @create-operator{dx_asset_optics_perspective}
Core_Result dx_asset_optics_perspective_create(dx_asset_optics_perspective** RETURN);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSETS_OPTICS_H_INCLUDED
