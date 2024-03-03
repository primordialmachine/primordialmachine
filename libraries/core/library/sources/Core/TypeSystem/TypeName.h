// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_TYPESYSTEM_TYPENAME_H_INCLUDED)
#define CORE_TYPESYSTEM_TYPENAME_H_INCLUDED

#include "Core/FundamentalTypes.h"
#include "Core/ReferenceCounter.h"

typedef struct _Core_TypeName _Core_TypeName;

struct _Core_TypeName {
  Core_ReferenceCounter reference_count;
  Core_Size hash_value;
  Core_Size number_of_bytes;
  char bytes[];
};

// DESCRIPTION Create a type name.
// PARAMETER RETURN A pointer to a <code>_Core_TypeName</code> variable.
// PARAMETER bytes, numberOfBytes An array of Bytes
// SUCCESS
// <code>*RETURN</code> was assigned a pointer to the <code>_Core_TypeName</code> object.
// The caller acquired a reference to that object.
Core_Result _Core_TypeName_create(_Core_TypeName** RETURN, char const* bytes, Core_Size numberOfBytes);

// DESCRIPTION Increment the reference count of a type name.
// PARAMETER a A pointer to a <code>_Core_TypeName</code> object.
void _Core_TypeName_reference(_Core_TypeName* a);

// DESCRIPTION Decrement the reference count of a type name.
// PARAMETER a A pointer to a <code>_Core_TypeName</code> object.
void _Core_TypeName_unreference(_Core_TypeName* a);

#endif // CORE_TYPESYSTEM_TYPENAME_H_INCLUDED
