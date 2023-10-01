#include "dx/assets/scene.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.scene",
                      dx_asset_scene,
                      dx_object)

static void dx_asset_scene_destruct(dx_asset_scene* SELF) {
  dx_inline_object_array_uninitialize(&SELF->assets);
}

static void dx_asset_scene_dispatch_construct(dx_asset_scene_dispatch* self)
{/*Intentionally empty.*/}

int dx_asset_scene_construct(dx_asset_scene* SELF) {
  dx_rti_type* _type = dx_asset_scene_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_inline_object_array_initialize(&SELF->assets, 0)) {
      return 1;
  }
  DX_OBJECT(SELF)->type = _type;
  return 0;
}

dx_result dx_asset_scene_create(dx_asset_scene** RETURN) {
  DX_CREATE_PREFIX(dx_asset_scene)
  if (dx_asset_scene_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
