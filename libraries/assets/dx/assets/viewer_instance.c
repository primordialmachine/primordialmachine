#include "dx/assets/viewer_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.viewer_instance",
                      dx_assets_viewer_instance,
                      dx_object);

static void dx_assets_viewer_instance_destruct(dx_assets_viewer_instance* SELF) {
  DX_UNREFERENCE(SELF->viewer_reference);
  SELF->viewer_reference = NULL;
}

static void dx_assets_viewer_instance_dispatch_construct(dx_assets_viewer_instance_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_viewer_instance_construct(dx_assets_viewer_instance* SELF, dx_asset_reference* viewer_reference) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_assets_viewer_instance_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (!viewer_reference) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->viewer_reference = viewer_reference;
  DX_REFERENCE(SELF->viewer_reference);
  
  dx_mat4_set_identity(&SELF->world_matrix);
  
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_viewer_instance_create(dx_assets_viewer_instance** RETURN, dx_asset_reference* viewer_reference) {
  DX_CREATE_PREFIX(dx_assets_viewer_instance)
  if (dx_assets_viewer_instance_construct(SELF, viewer_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
