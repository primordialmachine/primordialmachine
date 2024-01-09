// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASSTATECHANGEDMESSAGE_H_INCLUDED)
#define CORE_VISUALS_CANVASSTATECHANGEDMESSAGE_H_INCLUDED

#include "Core/Visuals/CanvasMessage.h"

/// @brief Sent when the state of a canvas changed.
Core_declareObjectType("Core.CanvasStateChangedMessage",
                       Core_CanvasStateChangedMessage,
                       Core_CanvasMessage);

static inline Core_CanvasStateChangedMessage* CORE_CANVASSTATECHANGEDMESSAGE(void* p) {
  return (Core_CanvasStateChangedMessage*)p;
}

struct Core_CanvasStateChangedMessage {
  Core_CanvasMessage _parent;
  Core_Boolean active;
};

static inline Core_CanvasStateChangedMessage_Dispatch* CORE_CANVASSTATECHANGEDMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasStateChangedMessage_Dispatch*)p;
}

struct Core_CanvasStateChangedMessage_Dispatch {
  Core_CanvasMessage_Dispatch _parent;
};

Core_Result Core_CanvasStateChangedMessage_construct(Core_CanvasStateChangedMessage* SELF, Core_Boolean active);

Core_Result Core_CanvasStateChangedMessage_create(Core_CanvasStateChangedMessage** RETURN, Core_Boolean active);

#endif // CORE_VISUALS_CANVASSTATECHANGEDMESSAGE_H_INCLUDED
