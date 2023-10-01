#include "dx/assets/scene.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.scene",
                      dx_assets_scene,
                      dx_object);

static void dx_assets_scene_destruct(dx_assets_scene* SELF) {
  dx_inline_object_array_uninitialize(&SELF->assets);
}

static void dx_assets_scene_dispatch_construct(dx_assets_scene_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_assets_scene_construct(dx_assets_scene* SELF) {
  dx_rti_type* TYPE = dx_assets_scene_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_inline_object_array_initialize(&SELF->assets, 0)) {
      return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_scene_create(dx_assets_scene** RETURN) {
  DX_CREATE_PREFIX(dx_assets_scene)
  if (dx_assets_scene_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
