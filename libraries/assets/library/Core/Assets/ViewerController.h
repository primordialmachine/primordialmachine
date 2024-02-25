#if !defined(DX_ASSETS_VIEWERCONTROLLER_H_INCLUDED)
#define DX_ASSETS_VIEWERCONTROLLER_H_INCLUDED

#include "Core/Assets/Def.h"
typedef struct dx_assets_viewer dx_assets_viewer;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("Core.Assets.ViewerController",
                       dx_assets_viewer_controller,
                       Core_Assets_Def);

static inline dx_assets_viewer_controller* DX_ASSETS_VIEWER_CONTROLLER(void* p) {
  return (dx_assets_viewer_controller*)p;
}

struct dx_assets_viewer_controller {
  Core_Assets_Def _parent;
  Core_Result (*update)(dx_assets_viewer_controller* SELF, dx_assets_viewer* viewer, Core_Real32 delta_seconds);
};

static inline dx_assets_viewer_controller* DX_ASSETS_VIEWER_CONTROLLER_DISPATCH(void* p) {
  return (dx_assets_viewer_controller*)p;
}

struct dx_assets_viewer_controller_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

Core_Result dx_assets_viewer_controller_construct(dx_assets_viewer_controller* SELF);

Core_Result dx_assets_viewer_controller_update(dx_assets_viewer_controller* SELF, dx_assets_viewer* viewer, Core_Real32 delta_seconds);

#endif // DX_ASSETS_VIEWERCONTROLLER_H_INCLUDED
