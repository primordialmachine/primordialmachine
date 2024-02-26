// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_SCANNER_H_INCLUDED)
#define CORE_SCANNER_H_INCLUDED

#include "dx/core/Object.h"

/* http://localhost/core#core-scanner */
Core_declareObjectType("Core.Scanner",
                       Core_Scanner,
                       Core_Object);

static inline Core_Scanner* CORE_SCANNER(void* p) {
  return (Core_Scanner*)p;
}

struct Core_Scanner {
  Core_Object _parent;
};

static inline Core_Scanner_Dispatch* CORE_SCANNER_DISPATCH(void* p) {
  return (Core_Scanner_Dispatch*)p;
}

struct Core_Scanner_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*setInput)(Core_Scanner* SELF, char const* bytes, Core_Size number_of_bytes);
  Core_Result(*step)(Core_Scanner* SELF);
  Core_Result(*getWordTextBytes)(char const** RETURN, Core_Scanner* SELF);
  Core_Result(*getWordTextNumberOfBytes)(Core_Size* RETURN, Core_Scanner* SELF);
  Core_Result(*getWordStartOffset)(Core_Size* RETURN, Core_Scanner* SELF);
  Core_Result(*getWordEndOffset)(Core_Size* RETURN, Core_Scanner* SELF);
};

/* http://localhost/core#core-scanner-construct */
Core_Result Core_Scanner_construct(Core_Scanner* SELF);

/* http://localhost/core#core-scanner-setinput */
static inline Core_Result Core_Scanner_setInput(Core_Scanner* SELF, char const* bytes, Core_Size number_of_bytes) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, setInput, SELF, bytes, number_of_bytes);
}

/* http://localhost/core#core-scanner-step */
static inline Core_Result Core_Scanner_step(Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, step, SELF);
}

/* http://localhost/core#core-scanner-getwordtextbytes */
static inline Core_Result Core_Scanner_getWordTextBytes(char const** RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordTextBytes, RETURN, SELF);
}

/* http://localhost/core#core-scanner-getwordtextnumberofbytes */
static inline Core_Result Core_Scanner_getWordTextNumberOfBytes(Core_Size* RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordTextNumberOfBytes, RETURN, SELF);
}

/* http://localhost/core#core-scanner-getwordstartoffset */
static inline Core_Result Core_Scanner_getWordStartOffset(Core_Size* RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordStartOffset, RETURN, SELF);
}

/* http://localhost/core#core-scanner-getwordendoffset */
static inline Core_Result Core_Scanner_getWordEndOffset(Core_Size* RETURN, Core_Scanner* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Scanner, getWordEndOffset, RETURN, SELF);
}

#endif // CORE_SCANNER_H_INCLUDED
