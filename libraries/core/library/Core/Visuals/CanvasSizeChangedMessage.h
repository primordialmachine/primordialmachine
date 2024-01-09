// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASSIZECHANGEDMESSAGE_H_INCLUDED)
#define CORE_VISUALS_CANVASSIZECHANGEDMESSAGE_H_INCLUDED

#include "Core/Visuals/CanvasMessage.h"

/// @brief Sent when the size of a canvas changed.
Core_declareObjectType("Core.CanvasSizeChangedMessage",
                       Core_CanvasSizeChangedMessage,
                       Core_CanvasMessage);

static inline Core_CanvasSizeChangedMessage* CORE_CANVASSIZECHANGEDMESSAGE(void* p) {
  return (Core_CanvasSizeChangedMessage*)p;
}

struct Core_CanvasSizeChangedMessage {
  Core_CanvasMessage _parent;
  Core_Real32 horizontalSize;
  Core_Real32 verticalSize;
};

static inline Core_CanvasSizeChangedMessage_Dispatch* CORE_CANVASSIZECHANGEDMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasSizeChangedMessage_Dispatch*)p;
}

struct Core_CanvasSizeChangedMessage_Dispatch {
  Core_CanvasMessage_Dispatch _parent;
};

Core_Result Core_CanvasSizeChangedMessage_construct(Core_CanvasSizeChangedMessage* SELF, Core_Real32 horizontalSize, Core_Real32 verticalSize);

Core_Result Core_CanvasSizeChangedMessage_create(Core_CanvasSizeChangedMessage** RETURN, Core_Real32 horizontalSize, Core_Real32 verticalSize);

#endif // CORE_VISUALS_CANVASSIZECHANGEDMESSAGE_H_INCLUDED
