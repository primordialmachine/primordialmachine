#include "Core/Assets/ImageOperations/MirrorVertical.h"

Core_defineObjectType("Core.Assets.ImageOperations.MirrorVertical",
                      Core_Assets_ImageOperations_MirrorVertical,
                      Core_Assets_ImageOperation);

static void Core_Assets_ImageOperations_MirrorVertical_destruct(Core_Assets_ImageOperations_MirrorVertical* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_ImageOperations_MirrorVertical_constructDispatch(Core_Assets_ImageOperations_MirrorVertical_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ImageOperations_MirrorVertical_construct(Core_Assets_ImageOperations_MirrorVertical* SELF) {
  Core_BeginConstructor(Core_Assets_ImageOperations_MirrorVertical);
  if (Core_Assets_ImageOperation_construct(CORE_ASSETS_IMAGEOPERATION(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Assets_ImageOperations_MirrorVertical);
}

Core_Result Core_Assets_ImageOperations_MirrorVertical_create(Core_Assets_ImageOperations_MirrorVertical** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_ImageOperations_MirrorVertical);
  if (Core_Assets_ImageOperations_MirrorVertical_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
