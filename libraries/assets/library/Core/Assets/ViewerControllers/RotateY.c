#include "Core/Assets/ViewerControllers/RotateY.h"

#include <math.h>
#include "Core/Assets/Viewer.h"

Core_defineObjectType("Core.Assets.ViewerControllers.RotateY",
                      dx_asset_viewer_controllers_rotate_y,
                      dx_assets_viewer_controller);

static Core_Result dx_asset_viewer_controllers_rotate_y_update(dx_asset_viewer_controllers_rotate_y* SELF, dx_assets_viewer* viewer, Core_Real32 delta_seconds) {
  Core_Real32 degrees = SELF->degrees_per_second * delta_seconds;
  SELF->degrees = fmodf(SELF->degrees + degrees, 360.f);
  DX_MAT4 a;
  dx_mat4_set_rotate_y(&a, SELF->degrees);
  dx_transform_point(&viewer->source, &SELF->source, &a);
  return Core_Success;
}

static void dx_asset_viewer_controllers_rotate_y_destruct(dx_asset_viewer_controllers_rotate_y* SELF) {/*Intentionally empty.*/
}

static void dx_asset_viewer_controllers_rotate_y_constructDispatch(dx_asset_viewer_controllers_rotate_y_Dispatch* self) {/*Intentionally empty.*/
}

Core_Result dx_asset_viewer_controllers_rotate_y_construct(dx_asset_viewer_controllers_rotate_y* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_viewer_controllers_rotate_y);
  if (dx_assets_viewer_controller_construct(DX_ASSETS_VIEWER_CONTROLLER(SELF))) {
    return Core_Failure;
  }
  dx_vec3_set(&SELF->target, 0.f, 0.f, 0.f);
  dx_vec3_set(&SELF->source, 0.f, 0.f, 1.f);
  dx_vec3_set(&SELF->up, 0.f, 1.f, 0.f);
  SELF->degrees_per_second = 16.f;
  SELF->degrees = 0.f;
  DX_ASSETS_VIEWER_CONTROLLER(SELF)->update = (Core_Result(*)(dx_assets_viewer_controller*, dx_assets_viewer*, Core_Real32)) & dx_asset_viewer_controllers_rotate_y_update;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_viewer_controllers_rotate_y_create(dx_asset_viewer_controllers_rotate_y** RETURN) {
  DX_CREATE_PREFIX(dx_asset_viewer_controllers_rotate_y);
  if (dx_asset_viewer_controllers_rotate_y_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
