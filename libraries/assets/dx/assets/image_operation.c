#include "dx/assets/image_operation.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.image_operation",
                      dx_assets_image_operation,
                      dx_object);

static void dx_assets_image_operation_destruct(dx_assets_image_operation* SELF)
{/*Intentionally empty.*/}

static void dx_assets_image_operation_dispatch_construct(dx_assets_image_operation_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_image_operation_construct(dx_assets_image_operation* SELF) {
  dx_rti_type* TYPE = dx_assets_image_operation_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
