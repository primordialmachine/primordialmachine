#include "dx/assets/scene.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.scene",
                      dx_assets_scene,
                      Core_Object);

static void dx_assets_scene_destruct(dx_assets_scene* SELF) {
  dx_inline_object_array_uninitialize(&SELF->assets);
}

static void dx_assets_scene_constructDispatch(dx_assets_scene_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_assets_scene_construct(dx_assets_scene* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_scene);
  if (dx_inline_object_array_initialize(&SELF->assets, 0)) {
      return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_scene_create(dx_assets_scene** RETURN) {
  DX_CREATE_PREFIX(dx_assets_scene);
  if (dx_assets_scene_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
