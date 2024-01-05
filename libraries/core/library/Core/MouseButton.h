// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_MOUSEBUTTON_H_INCLUDED)
#define CORE_MOUSEBUTTON_H_INCLUDED

#include "dx/core/TypeSystem.h"

/// @ingroup Core
/// @brief Enumeration of mouse buttons.
DX_DECLARE_ENUMERATION_TYPE("Core_MouseButton",
                            Core_MouseButton);

enum Core_MouseButton {

#define Alias(aliased,alias) Core_MouseButton_##alias = Core_MouseButton_##aliased,
#define Define(name,value,description) Core_MouseButton_##name = value,

#include "Core/MouseButton.i"

#undef Define
#undef Alias

};

#endif // CORE_MOUSEBUTTON_H_INCLUDED
