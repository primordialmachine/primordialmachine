// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Visuals/CanvasDpiChangedMessage.h"

Core_defineObjectType("Core.CanvasDpiChangedMessage",
                      Core_CanvasDpiChangedMessage,
                      Core_CanvasMessage);

static void Core_CanvasDpiChangedMessage_destruct(Core_CanvasDpiChangedMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasDpiChangedMessage_constructDispatch(Core_CanvasDpiChangedMessage_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_CanvasDpiChangedMessage_construct(Core_CanvasDpiChangedMessage* SELF, Core_Real32 horizontalDpi, Core_Real32 verticalDpi) {
  Core_BeginConstructor(Core_CanvasDpiChangedMessage);
  if (Core_CanvasMessage_construct(CORE_CANVASMESSAGE(SELF), Core_CanvasMessageKind_DpiChanged)) {
    return Core_Failure;
  }
  SELF->horizontalDpi = horizontalDpi;
  SELF->verticalDpi = verticalDpi;
  Core_EndConstructor(Core_CanvasDpiChangedMessage);
}

Core_Result Core_CanvasDpiChangedMessage_create(Core_CanvasDpiChangedMessage** RETURN, Core_Real32 horizontalDpi, Core_Real32 verticalDpi) {
  DX_CREATE_PREFIX(Core_CanvasDpiChangedMessage);
  if (Core_CanvasDpiChangedMessage_construct(SELF, horizontalDpi, verticalDpi)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
