#include "dx/assets/image_operations/mirror_horizontal.h"


Core_defineObjectType("dx.assets.image_operations.mirror_horizontal",
                      dx_asset_image_operations_mirror_horizontal,
                      dx_assets_image_operation)

static void dx_asset_image_operations_mirror_horizontal_destruct(dx_asset_image_operations_mirror_horizontal* SELF)
{/*Intentionally empty.*/}

static void dx_asset_image_operations_mirror_horizontal_constructDispatch(dx_asset_image_operations_mirror_horizontal_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_image_operations_mirror_horizontal_construct(dx_asset_image_operations_mirror_horizontal* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_image_operations_mirror_horizontal);
  if (dx_assets_image_operation_construct(DX_ASSETS_IMAGE_OPERATION(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_image_operations_mirror_horizontal_create(dx_asset_image_operations_mirror_horizontal** RETURN) {
  DX_CREATE_PREFIX(dx_asset_image_operations_mirror_horizontal);
  if (dx_asset_image_operations_mirror_horizontal_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
