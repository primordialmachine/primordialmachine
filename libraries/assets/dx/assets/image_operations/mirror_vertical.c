#include "dx/assets/image_operations/mirror_vertical.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.image_operations.mirror_vertical",
                      dx_asset_image_operations_mirror_vertical,
                      dx_asset_image_operation);

static void dx_asset_image_operations_mirror_vertical_destruct(dx_asset_image_operations_mirror_vertical* SELF)
{/*Intentionally empty.*/}

static void dx_asset_image_operations_mirror_vertical_dispatch_construct(dx_asset_image_operations_mirror_vertical_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_image_operations_mirror_vertical_construct(dx_asset_image_operations_mirror_vertical* SELF) {
  dx_rti_type* TYPE = dx_asset_image_operations_mirror_vertical_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_asset_image_operation_construct(DX_ASSET_IMAGE_OPERATION(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_asset_image_operations_mirror_vertical* dx_asset_image_operations_mirror_vertical_create() {
  dx_rti_type* TYPE = dx_asset_image_operations_mirror_vertical_get_type();
  if (!TYPE) {
    return NULL;
  }
  dx_asset_image_operations_mirror_vertical* SELF = DX_ASSET_IMAGE_OPERATIONS_MIRROR_VERTICAL(dx_object_alloc(sizeof(dx_asset_image_operations_mirror_vertical)));
  if (!SELF) {
    return NULL;
  }
  if (dx_asset_image_operations_mirror_vertical_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return NULL;
  }
  return SELF;
}
