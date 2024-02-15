#include "Core/Assets/ImageOperations/MirrorHorizontal.h"

Core_defineObjectType("Core.Assets.ImageOperations.MirrorHorizontal",
                      Core_Assets_ImageOperations_MirrorHorizontal,
                      Core_Assets_ImageOperation);

static void Core_Assets_ImageOperations_MirrorHorizontal_destruct(Core_Assets_ImageOperations_MirrorHorizontal* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_ImageOperations_MirrorHorizontal_constructDispatch(Core_Assets_ImageOperations_MirrorHorizontal_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ImageOperations_MirrorHorizontal_construct(Core_Assets_ImageOperations_MirrorHorizontal* SELF) {
  Core_BeginConstructor(Core_Assets_ImageOperations_MirrorHorizontal);
  if (Core_Assets_ImageOperation_construct(CORE_ASSETS_IMAGEOPERATION(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Assets_ImageOperations_MirrorHorizontal);
}

Core_Result Core_Assets_ImageOperations_MirrorHorizontal_create(Core_Assets_ImageOperations_MirrorHorizontal** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_ImageOperations_MirrorHorizontal);
  if (Core_Assets_ImageOperations_MirrorHorizontal_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
