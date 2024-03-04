// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/StringIterator.h"

Core_defineObjectType("Core.StringIterator",
                      Core_StringIterator,
                      Core_Object);

static void Core_StringIterator_destruct(Core_StringIterator* SELF)
{/*Intentionally empty.*/}

static void Core_StringIterator_constructDispatch(Core_StringIterator_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_StringIterator_construct(Core_StringIterator* SELF) {
  Core_BeginConstructor(Core_StringIterator);
  Core_EndConstructor(Core_StringIterator);
}
