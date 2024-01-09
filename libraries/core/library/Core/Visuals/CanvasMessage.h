// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASMESSAGE_H_INCLUDED)
#define CORE_VISUALS_CANVASMESSAGE_H_INCLUDED

#include "Core/Message.h"
#include "Core/Visuals/CanvasMessageKind.h"

/* http://localhost/val#core-canvasmessage */
Core_declareObjectType("Core.CanvasMessage",
                       Core_CanvasMessage,
                       Core_Message);

static inline Core_CanvasMessage* CORE_CANVASMESSAGE(void* p) {
  return (Core_CanvasMessage*)p;
}

struct Core_CanvasMessage {
  Core_Message _parent;
  Core_CanvasMessageKind kind;
};

static inline Core_CanvasMessage_Dispatch* CORE_CANVASMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasMessage_Dispatch*)p;
}

struct Core_CanvasMessage_Dispatch {
  Core_Message_Dispatch _parent;
};

/// @remarks The canvas message is constructed with the specified Core_CanvasMessageKind.
/// @param kind The kind of this canvas message.
/// @constructor{Core_CanvasMessage}
Core_Result Core_CanvasMessage_construct(Core_CanvasMessage* SELF, Core_CanvasMessageKind kind);

/// @brief Get the kind of this canvas message.
/// @param RETURN A pointer to a <code>Core_CanvasMessageKind</code> variable.
/// @success <code>*RETURN</code> was assigned the kind.
/// @method{Core_CanvasMessage}
Core_Result Core_CanvasMessage_getKind(Core_CanvasMessageKind* RETURN, Core_CanvasMessage* SELF);

#endif // CORE_VISUALS_CANVASMESSAGES_H_INCLUDED
