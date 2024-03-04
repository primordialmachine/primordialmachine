// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/System.h"

Core_defineObjectType("Core.System",
                      Core_System,
                      Core_Object);

static void Core_System_destruct(Core_System* SELF)
{/*Intentionally empty.*/}

static void Core_System_constructDispatch(Core_System_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_System_construct(Core_System* SELF) {
  Core_BeginConstructor(Core_System);
  Core_EndConstructor(Core_System);
}
