// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_INPUT_MOUSEBUTTON_H_INCLUDED)
#define CORE_INPUT_MOUSEBUTTON_H_INCLUDED

#include "dx/core/TypeSystem.h"
typedef struct Core_String Core_String;

/// @ingroup Core
/// @brief Enumeration of mouse buttons.
Core_declareEnumerationType("Core_MouseButton",
                            Core_MouseButton);

enum Core_MouseButton {

#define Alias(aliased,alias) Core_MouseButton_##alias = Core_MouseButton_##aliased,
#define Define(name,value,description) Core_MouseButton_##name = value,

#include "Core/Input/MouseButton.i"

#undef Define
#undef Alias

};

/* http://localhost/core#core-mousebutton-tostring */
Core_Result Core_MouseButton_toString(Core_String** RETURN, Core_MouseButton SELF);

#endif // CORE_INPUT_MOUSEBUTTON_H_INCLUDED
