// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_MESSAGE_H_INCLUDED)
#define CORE_MESSAGE_H_INCLUDED

#include "dx/core/Object.h"

/// @brief The opaque type of a message.
DX_DECLARE_OBJECT_TYPE("Core.Message",
                       Core_Message,
                       Core_Object);

static inline Core_Message* CORE_MESSAGE(void* p) {
  return (Core_Message*)p;
}

struct Core_Message {
  Core_Object _parent;
  Core_Natural64 timeStamp;
};

static inline Core_Message_dispatch* CORE_MESSAGE_DISPATCH(void* p) {
  return (Core_Message_dispatch*)p;
}

struct Core_Message_dispatch {
  Core_Object_Dispatch _parent;
};

/// @constructor{Core_Message}
/// @param timeStamp The time stamp of this message. A time stamp value as for example retrieved by Core_getNow().
Core_Result Core_Message_construct(Core_Message* SELF, Core_Natural64 timeStamp);

#endif // CORE_MESSAGE_H_INCLUDED