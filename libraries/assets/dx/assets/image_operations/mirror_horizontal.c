#include "dx/assets/image_operations/mirror_horizontal.h"


DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.mirror_horizontal",
                      dx_asset_image_operations_mirror_horizontal,
                      dx_asset_image_operation)

static void dx_asset_image_operations_mirror_horizontal_destruct(dx_asset_image_operations_mirror_horizontal* SELF)
{/*Intentionally empty.*/}

static void dx_asset_image_operations_mirror_horizontal_dispatch_construct(dx_asset_image_operations_mirror_horizontal_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_image_operations_mirror_horizontal_construct(dx_asset_image_operations_mirror_horizontal* SELF) {
  dx_rti_type* TYPE = dx_asset_image_operations_mirror_horizontal_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_image_operations_mirror_horizontal_create(dx_asset_image_operations_mirror_horizontal** RETURN) {
  DX_CREATE_PREFIX(dx_asset_image_operations_mirror_horizontal)
  if (dx_asset_image_operations_mirror_horizontal_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
