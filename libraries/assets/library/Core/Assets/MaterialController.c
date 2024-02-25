// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/MaterialController.h"

Core_defineObjectType("Core.Assets.MaterialController",
                      Core_Assets_MaterialController,
                      Core_Assets_Def);

static void Core_Assets_MaterialController_destruct(Core_Assets_MaterialController* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_MaterialController_constructDispatch(Core_Assets_MaterialController_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_MaterialController_construct(Core_Assets_MaterialController* SELF) {
  Core_BeginConstructor(Core_Assets_MaterialController);
  Core_EndConstructor(Core_Assets_MaterialController);
}
