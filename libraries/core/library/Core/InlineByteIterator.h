#if !defined(CORE_INLINEBYTEITERATOREX_H_INCLUDED)
#define CORE_INLINEBYTEITERATOREX_H_INCLUDED

#include "Core/FundamentalTypes.h"

#define INLINE static inline


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct InlineByteIterator InlineByteIterator;

// This function fails with Core_Error_ArgumentInvalid if at least one of the conditions below is fulfilled
// - SELF is null
// - RETURN is null
typedef Core_Result(InlineByteIterator_HasValue)(Core_Boolean* RETURN, InlineByteIterator* SELF);

// This function fails with Core_Error_ArgumentInvalid if at least one of the conditions below is fulfilled
// - SELF is null
// - RETURN is null
typedef Core_Result(InlineByteIterator_GetIndex)(Core_Size* RETURN, InlineByteIterator* SELF);

// This function fails with Core_Error_ArgumentInvalid if at least one of the conditions below is fulfilled
// - SELF is null
// - RETURN is null
typedef Core_Result(InlineByteIterator_GetValue)(Core_Natural8* RETURN, InlineByteIterator* SELF);

// This function fails with Core_Error_ArgumentInvalid if at least one of the conditions below is fulfilled
// - SELF is null
typedef Core_Result(InlineByteIterator_Next)(InlineByteIterator* SELF);

// This function fails with Core_Error_ArgumentInvalid if at least one of the conditions below is fulfilled
// - SELF is null
typedef Core_Result(InlineByteIterator_Uninitialize)(InlineByteIterator* SELF);

// while (hasValue) { v = value ; next }
struct InlineByteIterator {
  InlineByteIterator_HasValue* hasValue;
  InlineByteIterator_GetIndex* getIndex;
  InlineByteIterator_GetValue* getValue;
  InlineByteIterator_Next* next;
  InlineByteIterator_Uninitialize* uninitialize;
};

INLINE Core_Result
InlineByteIterator_initialize
  (
    InlineByteIterator* SELF,
    InlineByteIterator_HasValue* hasValue,
    InlineByteIterator_GetIndex* getIndex,
    InlineByteIterator_GetValue* getValue,
    InlineByteIterator_Next* next,
    InlineByteIterator_Uninitialize* uninitialize
  )
{
  SELF->hasValue = hasValue;
  SELF->getIndex = getIndex;
  SELF->getValue = getValue;
  SELF->next = next;
  SELF->uninitialize = uninitialize;
  return Core_Success;
}

INLINE Core_Result
InlineByteIterator_uninitialize
  (
    InlineByteIterator* SELF
  )
{ 
  SELF->uninitialize(SELF);
  return Core_Success;
}

INLINE Core_Result
InlineByteIterator_hasValue
  (
    Core_Boolean* RETURN, InlineByteIterator* SELF
  )
{ return SELF->hasValue(RETURN, SELF); }

INLINE Core_Result
InlineByteIterator_getIndex
  (
    Core_Size* RETURN,
    InlineByteIterator* SELF
  )
{ return SELF->getIndex(RETURN, SELF); }

INLINE Core_Result
InlineByteIterator_getValue
  (
    Core_Natural8* RETURN,
    InlineByteIterator* SELF
  )
{ return SELF->getValue(RETURN, SELF); }

INLINE Core_Result
InlineByteIterator_next
  (
    InlineByteIterator* SELF
  )
{ return SELF->next(SELF); }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct InlineByteIterator_C InlineByteIterator_C;

// An iterator for sequences of Bytes.
struct InlineByteIterator_C {
  InlineByteIterator parent;
  Core_Natural8 const* p;
  Core_Size n;
  Core_Size i;
};

INLINE Core_Result
InlineByteIterator_C_hasValue
  (
    Core_Boolean* RETURN,
    InlineByteIterator_C* SELF
  )
{
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->i < SELF->n;
  return Core_Success;
}

INLINE Core_Result
InlineByteIterator_C_getIndex
  (
    Core_Natural8* RETURN,
    InlineByteIterator_C* SELF
  )
{
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->i;
  return Core_Success;
}

INLINE Core_Result
InlineByteIterator_C_getValue
  (
    Core_Natural8* RETURN,
    InlineByteIterator_C* SELF
  )
{
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Boolean hasValue;
  if (InlineByteIterator_hasValue(&hasValue, (InlineByteIterator*)SELF)) {
    return Core_Failure;
  }
  if (!hasValue) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->p[SELF->i];
  return Core_Success;
}

INLINE Core_Result
InlineByteIterator_C_next
  (
    InlineByteIterator_C* SELF
  )
{
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Boolean hasValue;
  if (InlineByteIterator_hasValue(&hasValue, (InlineByteIterator*)SELF)) {
    return Core_Failure;
  }
  if (!hasValue) {
    return Core_Success;
  }
  SELF->i++;
  return Core_Success;
}

INLINE Core_Result
InlineByteIterator_C_uninitialize
  (
    InlineByteIterator_C* SELF
  )
{
  return Core_Success;
}


INLINE Core_Result
InlineByteIterator_C_initialize
  (
    InlineByteIterator_C* SELF,
    Core_Natural8 const* p,
    Core_Size n
  )
{
  if (InlineByteIterator_initialize((InlineByteIterator*)SELF,
                                    (InlineByteIterator_HasValue*) & InlineByteIterator_C_hasValue,
                                    (InlineByteIterator_GetIndex*) & InlineByteIterator_C_getIndex,
                                    (InlineByteIterator_GetValue*) & InlineByteIterator_C_getValue,
                                    (InlineByteIterator_Next*) & InlineByteIterator_C_next,
                                    (InlineByteIterator_Uninitialize*) & InlineByteIterator_C_uninitialize)) {
    return Core_Failure;
  }
  SELF->p = p;
  SELF->n = n;
  SELF->i = 0;
  return Core_Success;
}

#undef INLINE

#endif // CORE_INLINEBYTEITERATOREX_H_INCLUDED
