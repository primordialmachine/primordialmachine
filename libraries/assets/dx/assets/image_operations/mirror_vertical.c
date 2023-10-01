#include "dx/assets/image_operations/mirror_vertical.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.image_operations.mirror_vertical",
                      dx_assets_image_operations_mirror_vertical,
                      dx_assets_image_operation);

static void dx_assets_image_operations_mirror_vertical_destruct(dx_assets_image_operations_mirror_vertical* SELF)
{/*Intentionally empty.*/}

static void dx_assets_image_operations_mirror_vertical_dispatch_construct(dx_assets_image_operations_mirror_vertical_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_image_operations_mirror_vertical_construct(dx_assets_image_operations_mirror_vertical* SELF) {
  dx_rti_type* TYPE = dx_assets_image_operations_mirror_vertical_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_assets_image_operation_construct(DX_ASSETS_IMAGE_OPERATION(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_image_operations_mirror_vertical_create(dx_assets_image_operations_mirror_vertical** RETURN) {
  DX_CREATE_PREFIX(dx_assets_image_operations_mirror_vertical)
  if (dx_assets_image_operations_mirror_vertical_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
