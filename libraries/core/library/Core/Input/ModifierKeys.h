// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_MODIFIERKEYS_H_INCLUDED)
#define CORE_INPUT_MODIFIERKEYS_H_INCLUDED

#include "dx/core/TypeSystem.h"

// http://localhost/core#core-modifierkeys
Core_declareEnumerationType("Core.ModifierKeys",
                            Core_ModifierKeys);

enum Core_ModifierKeys {

#define Define(Name, Value) \
  Core_ModifierKeys_##Name = Value,
  
#include "Core/Input/ModifierKeys.i"

#undef Define

};

#endif // CORE_INPUT_MODIFIERKEYS_H_INCLUDED
