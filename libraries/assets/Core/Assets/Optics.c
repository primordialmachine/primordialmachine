// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "Core/Assets/Optics.h"

Core_defineObjectType("Core.Assets.Optics",
                      Core_Assets_Optics,
                      Core_Assets_Def);

static void Core_Assets_Optics_destruct(Core_Assets_Optics* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_Optics_constructDispatch(Core_Assets_Optics_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Optics_construct(Core_Assets_Optics* SELF) {
  Core_BeginConstructor(Core_Assets_Optics);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Assets_Optics);
}
