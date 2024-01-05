// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_MODIFIERKEYS_H_INCLUDED)
#define CORE_MODIFIERKEYS_H_INCLUDED

#include "dx/core/TypeSystem.h"

// http://localhost/core#core-modifierkeys
DX_DECLARE_ENUMERATION_TYPE("Core.ModifierKeys",
                            Core_ModifierKeys);

enum Core_ModifierKeys {

#define Define(Name, Value) \
  Core_ModifierKeys_##Name = Value,
  
#include "Core/ModifierKeys.i"

#undef Define

};

#endif // CORE_MODIFIERKEYS_H_INCLUDED
