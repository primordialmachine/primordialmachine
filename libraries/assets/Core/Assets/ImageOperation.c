// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "Core/Assets/ImageOperation.h"

Core_defineObjectType("Core.Assets.ImageOperation",
                      Core_Assets_ImageOperation,
                      Core_Assets_Def);

static void Core_Assets_ImageOperation_destruct(Core_Assets_ImageOperation* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_ImageOperation_constructDispatch(Core_Assets_ImageOperation_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_ImageOperation_construct(Core_Assets_ImageOperation* SELF) {
  Core_BeginConstructor(Core_Assets_ImageOperation);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Assets_ImageOperation);
}
