#if !defined(CORE_STRINGITERATOR_H_INCLUDED)
#define CORE_STRINGITERATOR_H_INCLUDED

#include "dx/core/Object.h"

/// @brief The base of all string iterators for UTF-8 strings.
/// String iterators of Core.String and Core.StringBuffer are all based on this.
/// @extends Core.Object
Core_declareObjectType("Core.StringIterator",
                       Core_StringIterator,
                       Core_Object);

static inline Core_StringIterator* CORE_STRINGITERATOR(void* p) {
  return (Core_StringIterator*)p;
}

struct Core_StringIterator {
  Core_Object _parent;
};

static inline Core_StringIterator_Dispatch* CORE_STRINGITERATOR_DISPATCH(void* p) {
  return (Core_StringIterator_Dispatch*)p;
}

struct Core_StringIterator_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*hasValue)(Core_Boolean *RETURN, Core_StringIterator* SELF);
  Core_Result (*getValue)(uint32_t* RETURN, Core_StringIterator* SELF);
  Core_Result (*next)(Core_StringIterator* SELF);
  Core_Result (*next_n)(Core_StringIterator* SELF, Core_Size n);
};

Core_Result Core_StringIterator_construct(Core_StringIterator* SELF);

static inline Core_Result Core_StringIterator_hasValue(Core_Boolean *RETURN, Core_StringIterator* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_StringIterator, hasValue, RETURN, SELF);
}

static inline Core_Result Core_StringIterator_getValue(uint32_t* RETURN, Core_StringIterator* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_StringIterator, getValue, RETURN, SELF);
}

static inline Core_Result Core_StringIterator_next(Core_StringIterator* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_StringIterator, next, SELF);
}

static inline Core_Result Core_StringIterator_next_many(Core_StringIterator* SELF, Core_Size n) {
  DX_OBJECT_VIRTUALCALL(Core_StringIterator, next_n, SELF, n);
}

#endif // CORE_STRINGITERATOR_H_INCLUDED
