#include "dx/assets/scene.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.scene",
                      dx_asset_scene,
                      dx_object)

static void dx_asset_scene_destruct(dx_asset_scene* self) {
  dx_inline_object_array_uninitialize(&self->assets);
}

static void dx_asset_scene_dispatch_construct(dx_asset_scene_dispatch* self)
{/*Intentionally empty.*/}

int dx_asset_scene_construct(dx_asset_scene* self) {
  dx_rti_type* _type = dx_asset_scene_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_inline_object_array_initialize(&self->assets, 0)) {
      return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_scene* dx_asset_scene_create() {
  dx_asset_scene* self = DX_ASSET_SCENE(dx_object_alloc(sizeof(dx_asset_scene)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_scene_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
