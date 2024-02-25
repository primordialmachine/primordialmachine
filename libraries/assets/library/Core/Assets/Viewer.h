// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_ASSETS_VIEWER_H_INCLUDED)
#define CORE_ASSETS_VIEWER_H_INCLUDED

#include "Core/Assets/Def.h"
typedef struct Core_Assets_Optics Core_Assets_Optics;
typedef struct dx_assets_viewer_controller dx_assets_viewer_controller;

/* http://localhost/assets#core-assets-viewer */
Core_declareObjectType("Core.Assets.Viewer",
                       dx_assets_viewer,
                       Core_Assets_Def);

static inline dx_assets_viewer* CORE_ASSETS_VIEWER(void* p) {
  return (dx_assets_viewer*)p;
}

struct dx_assets_viewer {
  Core_Assets_Def _parent;
  /// @brief A pointer to the optics of this viewer.
  /// @default A Core_Assets_PerspectiveOptics objects with default values.
  Core_Assets_Optics* optics;
  /// @brief A pointer to the ADL name of this viewer.
  Core_String* name;
  /// @brief The up vector.
  DX_VEC3 up;
  /// @brief The source position.
  DX_VEC3 source;
  /// @brief The target position.
  DX_VEC3 target;
  /// @brief A pointer to the viewer controller or a null pointer.
  dx_assets_viewer_controller* controller;
};

static inline dx_assets_viewer_Dispatch* CORE_ASSETS_VIEWER_DISPATCH(void* p) {
  return (dx_assets_viewer_Dispatch*)p;
}

struct dx_assets_viewer_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @default
/// The object is constructed with default values.
/// The default values are up <code>(0,1,0)</code>, source <code>(0,0,0)</code>, and target <code>(0,0,-1)</code>.
/// @param name A pointer to the "Asset Definition Language" name of this viewer object.
/// @constructor{dx_assets_viewer}
Core_Result dx_assets_viewer_construct(dx_assets_viewer* SELF, Core_String* name);

/// @create-operator{dx_assets_viewer}
Core_Result dx_assets_viewer_create(dx_assets_viewer** RETURN, Core_String* name);

#endif // CORE_ASSETS_VIEWER_H_INCLUDED
