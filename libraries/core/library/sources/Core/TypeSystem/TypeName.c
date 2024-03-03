// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/TypeSystem/TypeName.h"

#include "Core/Memory.h"
#include "Core/Hash.h"

Core_Result _Core_TypeName_create(_Core_TypeName** RETURN, char const* bytes, Core_Size number_of_bytes) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _Core_TypeName* SELF = NULL;
  if (Core_Memory_allocate(&SELF, sizeof(_Core_TypeName) + number_of_bytes)) {
    return Core_Failure;
  }
  if (Core_hashBytes(&SELF->hash_value, bytes, number_of_bytes)) {
    Core_Memory_deallocate(SELF);
    return Core_Failure;
  }
  Core_Memory_copy(SELF->bytes, bytes, number_of_bytes);
  SELF->number_of_bytes = number_of_bytes;
  SELF->reference_count = 1;
  *RETURN = SELF;
  return Core_Success;
}

void _Core_TypeName_reference(_Core_TypeName* a) {
  Core_ReferenceCounter_increment(&a->reference_count);
}

void _Core_TypeName_unreference(_Core_TypeName* a) {
  if (0 == Core_ReferenceCounter_decrement(&a->reference_count)) {
    Core_Memory_deallocate(a);
  }
}
