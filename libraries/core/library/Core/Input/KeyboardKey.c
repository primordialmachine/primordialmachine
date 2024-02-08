// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Input/KeyboardKey.h"

#include "Core/String.h"

Core_defineEnumerationType("Core.KeyboardKey",
                           Core_KeyboardKey);

Core_Result Core_KeyboardKey_toString(Core_String** RETURN, Core_KeyboardKey SELF) {
  //if (Nucleus_Unlikely(!string)) return Nucleus_Status_InvalidArgument;
  switch (SELF) {
  #define Alias(alias, aliased)
  #define Define(name, value, description)\
    case Core_KeyboardKey_##name: {\
      return Core_String_create(RETURN, description, sizeof(description) - 1); \
    };
  #include "Core/Input/KeyboardKey.i"
  #undef Define
  #undef Alias
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    };
  };
}
