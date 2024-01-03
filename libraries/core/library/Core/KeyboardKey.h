#if !defined(CORE_KEYBOARDKEY_H_INCLUDED)
#define CORE_KEYBOARDKEY_H_INCLUDED

#include "dx/core/TypeSystem.h"

/// @ingroup Core
/// @brief An enumeration of keyboard keys.
DX_DECLARE_ENUMERATION_TYPE("Core.KeyboardKey",
                            Core_KeyboardKey);

enum Core_KeyboardKey {

#define withAliases
#define Alias(alias,aliased) Core_KeyboardKey_##alias = Core_KeyboardKey_##aliased,
#define Define(name,value,description) Core_KeyboardKey_##name = value,

#include "Core/KeyboardKey.i"

#undef Define
#undef Alias
#undef withAliases

};

#endif // CORE_KEYBOARDKEY_H_INCLUDED
