// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_APPLICATIONMESSAGE_H_INCLUDED)
#define CORE_APPLICATIONMESSAGE_H_INCLUDED

#include "Core/ApplicationMessageKind.h"
#include "Core/Message.h"

// The opaque type of an "application" message.
Core_declareObjectType("Core.ApplicationMessage",
                       Core_ApplicationMessage,
                       Core_Message);

static inline Core_ApplicationMessage* CORE_APPLICATIONMESSAGE(void* p) {
  return (Core_ApplicationMessage*)p;
}

struct Core_ApplicationMessage {
  Core_Message _parent;
  Core_ApplicationMessageKind kind;
};

static inline Core_ApplicationMessage_Dispatch* CORE_APPLICATIONMESSAGE_DISPATCH(void* p) {
  return (Core_ApplicationMessage_Dispatch*)p;
}

struct Core_ApplicationMessage_Dispatch {
  Core_Message_Dispatch _parent;
};

/// @constructor{Core_ApplicationMessage}
Core_Result Core_ApplicationMessage_construct(Core_ApplicationMessage* SELF, Core_ApplicationMessageKind kind);

/// @create-operator{Core_ApplicationMessage}
Core_Result Core_ApplicationMessage_create(Core_ApplicationMessage** RETURN, Core_ApplicationMessageKind kind);

#endif // CORE_APPLICATIONMESSAGE_H_INCLUDED
