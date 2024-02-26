// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Input/MouseButton.h"

#include "Core/String.h"

Core_defineEnumerationType("Core.MouseButton",
                           Core_MouseButton);

Core_Result Core_MouseButton_toString(Core_String** RETURN, Core_MouseButton SELF) {
  switch (SELF) {
  #define Alias(aliased,alias)
  #define Define(name,value,description)\
    case Core_MouseButton_##name: {\
      return Core_String_create(RETURN, description, sizeof(description) - 1); \
    };
  #include "Core/Input/MouseButton.i"
  #undef Define
  #undef Alias
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    };
  };
}
