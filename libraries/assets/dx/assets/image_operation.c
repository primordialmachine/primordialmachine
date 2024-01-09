#include "dx/assets/image_operation.h"

Core_defineObjectType("dx.assets.image_operation",
                      dx_assets_image_operation,
                      Core_Object);

static void dx_assets_image_operation_destruct(dx_assets_image_operation* SELF)
{/*Intentionally empty.*/}

static void dx_assets_image_operation_constructDispatch(dx_assets_image_operation_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_image_operation_construct(dx_assets_image_operation* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_image_operation);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
