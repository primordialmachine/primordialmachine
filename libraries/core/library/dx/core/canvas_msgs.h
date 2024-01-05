/// @file Core/Visuals/CanvasMessages.h
/// @brief Messages pertaining to canvans.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(CORE_VISUALS_CANVASMESSAGES_H_INCLUDED)
#define CORE_VISUALS_CANVASMESSAGES_H_INCLUDED

#include "dx/core/msgs.h"
#include "Core/Visuals/CanvasMessageKind.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("Core.CanvasMessage",
                       Core_CanvasMessage,
                       Core_Message);

static inline Core_CanvasMessage* CORE_CANVASMESSAGE(void* p) {
  return (Core_CanvasMessage*)p;
}

struct Core_CanvasMessage {
  Core_Message _parent;
  uint8_t kind;
};

static inline Core_CanvasMessage_dispatch* CORE_CANVASMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasMessage_dispatch*)p;
}

struct Core_CanvasMessage_dispatch {
  Core_Message_dispatch _parent;
};

/// @remarks The canvas message is constructed with the specified Core_CanvasMessageKind.
/// @param kind The kind of this canvas message.
/// @constructor{Core_CanvasMessage}
Core_Result Core_CanvasMessage_construct(Core_CanvasMessage* SELF, Core_CanvasMessageKind kind);

/// @remarks The canvas message is created with the specified Core_CanvasMessageKind.
/// @param kind The kind of this canvas message. 
/// @create-operator{Core_CanvasMessage}
Core_Result Core_CanvasMessage_create(Core_CanvasMessage** RETURN, Core_CanvasMessageKind kind);

/// @brief Get the kind of this canvas message.
/// @param RETURN A pointer to a <code>Core_CanvasMessageKind</code> variable.
/// @success <code>*RETURN</code> was assigned the kind.
/// @method{Core_CanvasMessage}
Core_Result Core_CanvasMessage_getKind(Core_CanvasMessageKind* RETURN, Core_CanvasMessage* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Sent when the size of a canvas changed.
DX_DECLARE_OBJECT_TYPE("Core.CanvasSizeChangedMessage",
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

static inline Core_CanvasSizeChangedMessage_dispatch* CORE_CANVASSIZECHANGEDMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasSizeChangedMessage_dispatch*)p;
}

struct Core_CanvasSizeChangedMessage_dispatch {
  Core_CanvasMessage_dispatch _parent;
};

Core_Result Core_CanvasSizeChangedMessage_construct(Core_CanvasSizeChangedMessage* SELF, Core_Real32 horizontalSize, Core_Real32 verticalSize);

Core_Result Core_CanvasSizeChangedMessage_create(Core_CanvasSizeChangedMessage** RETURN, Core_Real32 horizontalSize, Core_Real32 verticalSize);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Sent when the DPI of a canvas changed.
DX_DECLARE_OBJECT_TYPE("Core.CanvasDpiChangedMessage",
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

static inline Core_CanvasDpiChangedMessage_dispatch* CORE_CANVASDPICHANGEDMESSAGE_DISPATCH(void* p) {
  return (Core_CanvasDpiChangedMessage_dispatch*)p;
}

struct Core_CanvasDpiChangedMessage_dispatch {
  Core_CanvasMessage_dispatch _parent;
};

Core_Result Core_CanvasDpiChangedMessage_construct(Core_CanvasDpiChangedMessage* SELF, Core_Real32 horizontalDpi, Core_Real32 verticalDpi);

Core_Result Core_CanvasDpiChangedMessage_create(Core_CanvasDpiChangedMessage** RETURN, Core_Real32 horizontalDpi, Core_Real32 verticalDpi);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // CORE_VISUALS_CANVASMESSAGES_H_INCLUDED
