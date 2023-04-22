#include "dx/assets/viewer_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.viewer_instance",
                      dx_asset_viewer_instance,
                      dx_object);

static void dx_asset_viewer_instance_destruct(dx_asset_viewer_instance* SELF) {
  DX_UNREFERENCE(SELF->viewer_reference);
  SELF->viewer_reference = NULL;
}

static void dx_asset_viewer_instance_dispatch_construct(dx_asset_viewer_instance_dispatch* SELF)
{/*Intentionally empty.*/}

int dx_asset_viewer_instance_construct(dx_asset_viewer_instance* SELF, dx_asset_reference* viewer_reference) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* TYPE = dx_asset_viewer_instance_get_type();
  if (!TYPE) {
    return 1;
  }

  if (!viewer_reference) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  SELF->viewer_reference = viewer_reference;
  DX_REFERENCE(SELF->viewer_reference);
  
  dx_mat4_set_identity(&SELF->world_matrix);
  
  DX_OBJECT(SELF)->type = TYPE;
  return 0;
}

dx_asset_viewer_instance* dx_asset_viewer_instance_create(dx_asset_reference* viewer_reference) {
  dx_asset_viewer_instance* SELF = DX_ASSET_VIEWER_INSTANCE(dx_object_alloc(sizeof(dx_asset_viewer_instance)));
  if (!SELF) {
    return NULL;
  }
  if (dx_asset_viewer_instance_construct(SELF, viewer_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return NULL;
  }
  return SELF;
}
