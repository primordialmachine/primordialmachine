#include "dx/assets/viewer_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.viewer_instance",
                      dx_assets_viewer_instance,
                      Core_Object);

static void dx_assets_viewer_instance_destruct(dx_assets_viewer_instance* SELF) {
  DX_UNREFERENCE(SELF->viewer_reference);
  SELF->viewer_reference = NULL;
}

static void dx_assets_viewer_instance_constructDispatch(dx_assets_viewer_instance_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_viewer_instance_construct(dx_assets_viewer_instance* SELF, dx_asset_reference* viewer_reference) {
  DX_CONSTRUCT_PREFIX(dx_assets_viewer_instance);

  if (!viewer_reference) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->viewer_reference = viewer_reference;
  DX_REFERENCE(SELF->viewer_reference);
  
  dx_mat4_set_identity(&SELF->world_matrix);
  
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_viewer_instance_create(dx_assets_viewer_instance** RETURN, dx_asset_reference* viewer_reference) {
  DX_CREATE_PREFIX(dx_assets_viewer_instance);
  if (dx_assets_viewer_instance_construct(SELF, viewer_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
