#include "dx/assets/viewer_controller.h"

#include <math.h>
#include "dx/assets/viewer.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.viewer_controller",
                      dx_assets_viewer_controller,
                      dx_object);

static void dx_assets_viewer_controller_destruct(dx_assets_viewer_controller* SELF)
{/*Intentionally empty.*/}

static void dx_assets_viewer_controller_dispatch_construct(dx_assets_viewer_controller_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_viewer_controller_construct(dx_assets_viewer_controller* SELF) {
  dx_rti_type* TYPE = dx_assets_viewer_controller_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->update = NULL;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_viewer_controller_update(dx_assets_viewer_controller* SELF, dx_assets_viewer* viewer, dx_f32 delta_seconds) {
  return SELF->update(SELF, viewer, delta_seconds);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.viewer_controllers.rotate_y",
                      dx_asset_viewer_controllers_rotate_y,
                      dx_assets_viewer_controller);

static dx_result dx_asset_viewer_controllers_rotate_y_update(dx_asset_viewer_controllers_rotate_y* SELF, dx_assets_viewer* viewer, dx_f32 delta_seconds) {
  dx_f32 degrees = SELF->degrees_per_second * delta_seconds;
  SELF->degrees = fmodf(SELF->degrees + degrees, 360.f);
  DX_MAT4 a;
  dx_mat4_set_rotate_y(&a, SELF->degrees);
  dx_transform_point(&viewer->source, &SELF->source, &a);
  return DX_SUCCESS;
}

static void dx_asset_viewer_controllers_rotate_y_destruct(dx_asset_viewer_controllers_rotate_y* SELF)
{/*Intentionally empty.*/}

static void dx_asset_viewer_controllers_rotate_y_dispatch_construct(dx_asset_viewer_controllers_rotate_y_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_asset_viewer_controllers_rotate_y_construct(dx_asset_viewer_controllers_rotate_y* SELF) {
  dx_rti_type* TYPE = dx_asset_viewer_controllers_rotate_y_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_assets_viewer_controller_construct(DX_ASSETS_VIEWER_CONTROLLER(SELF))) {
    return DX_FAILURE;
  }
  dx_vec3_set(&SELF->target, 0.f, 0.f, 0.f);
  dx_vec3_set(&SELF->source, 0.f, 0.f, 1.f);
  dx_vec3_set(&SELF->up, 0.f, 1.f, 0.f);
  SELF->degrees_per_second = 16.f;
  SELF->degrees = 0.f;
  DX_ASSETS_VIEWER_CONTROLLER(SELF)->update = (dx_result(*)(dx_assets_viewer_controller*,dx_assets_viewer*, dx_f32))&dx_asset_viewer_controllers_rotate_y_update;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_viewer_controllers_rotate_y_create(dx_asset_viewer_controllers_rotate_y** RETURN) {
  DX_CREATE_PREFIX(dx_asset_viewer_controllers_rotate_y)
  if (dx_asset_viewer_controllers_rotate_y_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
