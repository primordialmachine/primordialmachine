// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Visuals/CanvasSizeChangedMessage.h"

Core_defineObjectType("Core.CanvasSizeChangedMessage",
                      Core_CanvasSizeChangedMessage,
                      Core_CanvasMessage);

static void Core_CanvasSizeChangedMessage_destruct(Core_CanvasSizeChangedMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasSizeChangedMessage_constructDispatch(Core_CanvasSizeChangedMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_CanvasSizeChangedMessage_construct(Core_CanvasSizeChangedMessage* SELF, Core_Real32 horizontalSize, Core_Real32 verticalSize) {
  DX_CONSTRUCT_PREFIX(Core_CanvasSizeChangedMessage);
  if (Core_CanvasMessage_construct(CORE_CANVASMESSAGE(SELF), Core_CanvasMessageKind_SizeChanged)) {
    return Core_Failure;
  }
  SELF->horizontalSize = horizontalSize;
  SELF->verticalSize = verticalSize;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_CanvasSizeChangedMessage_create(Core_CanvasSizeChangedMessage** RETURN, Core_Real32 horizontalSize, Core_Real32 verticalSize) {
  DX_CREATE_PREFIX(Core_CanvasSizeChangedMessage);
  if (Core_CanvasSizeChangedMessage_construct(SELF, horizontalSize, verticalSize)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
