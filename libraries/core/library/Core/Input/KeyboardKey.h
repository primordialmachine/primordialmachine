// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_KEYBOARDKEY_H_INCLUDED)
#define CORE_INPUT_KEYBOARDKEY_H_INCLUDED

#include "dx/core/TypeSystem.h"
typedef struct Core_String Core_String;

/* http://localhost/core#core-keyboardkey */
Core_declareEnumerationType("Core.KeyboardKey",
                            Core_KeyboardKey);

enum Core_KeyboardKey {

#define withAliases
#define Alias(alias,aliased) Core_KeyboardKey_##alias = Core_KeyboardKey_##aliased,
#define Define(name,value,description) Core_KeyboardKey_##name = value,

#include "Core/Input/KeyboardKey.i"

#undef Define
#undef Alias
#undef withAliases

};

/* http://localhost/core#core-keyboardkey-tostring */
Core_Result Core_KeyboardKey_toString(Core_String** RETURN, Core_KeyboardKey SELF);

#endif // CORE_INPUT_KEYBOARDKEY_H_INCLUDED
