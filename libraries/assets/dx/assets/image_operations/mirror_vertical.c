#include "dx/assets/image_operations/mirror_vertical.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.image_operations.mirror_vertical",
                      dx_assets_image_operations_mirror_vertical,
                      dx_assets_image_operation);

static void dx_assets_image_operations_mirror_vertical_destruct(dx_assets_image_operations_mirror_vertical* SELF)
{/*Intentionally empty.*/}

static void dx_assets_image_operations_mirror_vertical_constructDispatch(dx_assets_image_operations_mirror_vertical_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_image_operations_mirror_vertical_construct(dx_assets_image_operations_mirror_vertical* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_image_operations_mirror_vertical);
  if (dx_assets_image_operation_construct(DX_ASSETS_IMAGE_OPERATION(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_image_operations_mirror_vertical_create(dx_assets_image_operations_mirror_vertical** RETURN) {
  DX_CREATE_PREFIX(dx_assets_image_operations_mirror_vertical);
  if (dx_assets_image_operations_mirror_vertical_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
