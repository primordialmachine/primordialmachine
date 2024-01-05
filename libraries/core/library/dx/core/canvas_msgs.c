#include "dx/core/canvas_msgs.h"

#include "Core/Time.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("Core.CanvasMessage",
                      Core_CanvasMessage,
                      Core_Message);

static void Core_CanvasMessage_destruct(Core_CanvasMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasMessage_constructDispatch(Core_CanvasMessage_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_CanvasMessage_construct(Core_CanvasMessage* SELF, Core_CanvasMessageKind kind) {
  DX_CONSTRUCT_PREFIX(Core_CanvasMessage);
  Core_Natural64 timeStamp;
  if (Core_getNow(&timeStamp)) {
    return Core_Failure;
  }
  if (Core_Message_construct(CORE_MESSAGE(SELF), timeStamp)) {
    return Core_Failure;
  }
  SELF->kind = kind;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_CanvasMessage_create(Core_CanvasMessage** RETURN, Core_CanvasMessageKind kind) {
  DX_CREATE_PREFIX(Core_CanvasMessage);
  if (Core_CanvasMessage_construct(SELF, kind)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_CanvasMessage_getKind(Core_CanvasMessageKind* RETURN, Core_CanvasMessage* SELF) {
  *RETURN = SELF->kind;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("Core.CanvasSizeChangedMessage",
                      Core_CanvasSizeChangedMessage,
                      Core_CanvasMessage);

static void Core_CanvasSizeChangedMessage_destruct(Core_CanvasSizeChangedMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasSizeChangedMessage_constructDispatch(Core_CanvasSizeChangedMessage_dispatch* SELF)
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
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("Core.CanvasDpiChangedMessage",
                      Core_CanvasDpiChangedMessage,
                      Core_CanvasMessage);

static void Core_CanvasDpiChangedMessage_destruct(Core_CanvasDpiChangedMessage* SELF)
{/*Intentionally empty.*/}

static void Core_CanvasDpiChangedMessage_constructDispatch(Core_CanvasDpiChangedMessage_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_CanvasDpiChangedMessage_construct(Core_CanvasDpiChangedMessage* SELF, Core_Real32 horizontalDpi, Core_Real32 verticalDpi) {
  DX_CONSTRUCT_PREFIX(Core_CanvasDpiChangedMessage);
  if (Core_CanvasMessage_construct(CORE_CANVASMESSAGE(SELF), Core_CanvasMessageKind_DpiChanged)) {
    return Core_Failure;
  }
  SELF->horizontalDpi = horizontalDpi;
  SELF->verticalDpi = verticalDpi;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_CanvasDpiChangedMessage_create(Core_CanvasDpiChangedMessage** RETURN, Core_Real32 horizontalDpi, Core_Real32 verticalDpi) {
  DX_CREATE_PREFIX(Core_CanvasDpiChangedMessage);
  if (Core_CanvasDpiChangedMessage_construct(SELF, horizontalDpi, verticalDpi)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
