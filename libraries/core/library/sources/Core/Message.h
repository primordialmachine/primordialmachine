// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_MESSAGE_H_INCLUDED)
#define CORE_MESSAGE_H_INCLUDED

#include "dx/core/Object.h"

/// @brief The opaque type of a message.
Core_declareObjectType("Core.Message",
                       Core_Message,
                       Core_Object);

static inline Core_Message* CORE_MESSAGE(void* p) {
  return (Core_Message*)p;
}

struct Core_Message {
  Core_Object _parent;
  Core_Natural64 timeStamp;
};

static inline Core_Message_Dispatch* CORE_MESSAGE_DISPATCH(void* p) {
  return (Core_Message_Dispatch*)p;
}

struct Core_Message_Dispatch {
  Core_Object_Dispatch _parent;
};

/* http://localhost/core#core-message-construct */
Core_Result Core_Message_construct(Core_Message* SELF, Core_Natural64 timeStamp);

#endif // CORE_MESSAGE_H_INCLUDED
