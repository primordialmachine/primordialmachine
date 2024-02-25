#include "Core/Assets/ViewerController.h"

#include "Core/Assets/Viewer.h"

Core_defineObjectType("Core.Assets.ViewerController",
                      dx_assets_viewer_controller,
                      Core_Assets_Def);

static void dx_assets_viewer_controller_destruct(dx_assets_viewer_controller* SELF)
{/*Intentionally empty.*/}

static void dx_assets_viewer_controller_constructDispatch(dx_assets_viewer_controller_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_viewer_controller_construct(dx_assets_viewer_controller* SELF) {
  Core_BeginConstructor(dx_assets_viewer_controller);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  SELF->update = NULL;
  Core_EndConstructor(dx_assets_viewer_controller);
}

Core_Result dx_assets_viewer_controller_update(dx_assets_viewer_controller* SELF, dx_assets_viewer* viewer, Core_Real32 delta_seconds) {
  return SELF->update(SELF, viewer, delta_seconds);
}
