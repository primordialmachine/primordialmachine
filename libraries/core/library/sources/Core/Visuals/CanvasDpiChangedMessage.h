// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASDPICHANGEDMESSAGE_H_INCLUDED)
#define CORE_VISUALS_CANVASDPICHANGEDMESSAGE_H_INCLUDED

#include "Core/Visuals/CanvasMessage.h"

/// @brief Sent when the DPI of a canvas changed.
Core_declareObjectType("Core.CanvasDpiChangedMessage",
                       Core_CanvasDpiChangedMessage,
                       Core_CanvasMessage);

static inline Core_CanvasDpiChangedMessage* CORE_CANVASDPICHANGEDMESSAGE(void* p) {
  return (Core_CanvasDpiChangedMessage*)p;
}

struct Core_CanvasDpiChangedMessage {
  Core_CanvasMessage _parent;
  Core_Real32 horizontalDpi;
  Core_Real32 verticalDpi;
};

static inline Core_CanvasDpiChangedMessage_Dispatch* CORE_CANVASDPICHANGEDMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasDpiChangedMessage_Dispatch*)p;
}

struct Core_CanvasDpiChangedMessage_Dispatch {
  Core_CanvasMessage_Dispatch _parent;
};

Core_Result Core_CanvasDpiChangedMessage_construct(Core_CanvasDpiChangedMessage* SELF, Core_Real32 horizontalDpi, Core_Real32 verticalDpi);

Core_Result Core_CanvasDpiChangedMessage_create(Core_CanvasDpiChangedMessage** RETURN, Core_Real32 horizontalDpi, Core_Real32 verticalDpi);

#endif // CORE_VISUALS_CANVASDPICHANGEDMESSAGE_H_INCLUDED
