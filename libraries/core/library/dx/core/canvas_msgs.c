#include "dx/core/canvas_msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_ENUMERATION_TYPE("dx.canvas_msg_kind",
                           dx_canvas_msg_kind);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.canvas_msg",
                      dx_canvas_msg,
                      Core_Message);

static void dx_canvas_msg_destruct(dx_canvas_msg* SELF)
{/*Intentionally empty.*/}

static void dx_canvas_msg_constructDispatch(dx_canvas_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_canvas_msg_construct(dx_canvas_msg* SELF, dx_canvas_msg_kind kind) {
  DX_CONSTRUCT_PREFIX(dx_canvas_msg);
  if (Core_Message_construct(CORE_MESSAGE(SELF))) {
    return Core_Failure;
  }
  SELF->kind = kind;
  CORE_MESSAGE(SELF)->flags = DX_MSG_TYPE_CANVAS;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_canvas_msg_create(dx_canvas_msg** RETURN, dx_canvas_msg_kind kind) {
  DX_CREATE_PREFIX(dx_canvas_msg);
  if (dx_canvas_msg_construct(SELF, kind)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_canvas_msg_get_kind(dx_canvas_msg_kind* RETURN, dx_canvas_msg* SELF) {
  *RETURN = SELF->kind;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.canvas_size_changed_msg",
                      dx_canvas_size_changed_msg,
                      dx_canvas_msg);

static void dx_canvas_size_changed_msg_destruct(dx_canvas_size_changed_msg* SELF)
{/*Intentionally empty.*/}

static void dx_canvas_size_changed_msg_constructDispatch(dx_canvas_size_changed_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_canvas_size_changed_msg_construct(dx_canvas_size_changed_msg* SELF, Core_Real32 width, Core_Real32 height) {
  DX_CONSTRUCT_PREFIX(dx_canvas_size_changed_msg);
  if (dx_canvas_msg_construct(DX_CANVAS_MSG(SELF), dx_canvas_msg_kind_size_changed)) {
    return Core_Failure;
  }
  SELF->width = width;
  SELF->height = height;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_canvas_size_changed_msg_create(dx_canvas_size_changed_msg** RETURN, Core_Real32 width, Core_Real32 height) {
  DX_CREATE_PREFIX(dx_canvas_size_changed_msg);
  if (dx_canvas_size_changed_msg_construct(SELF, width, height)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.canvas_dpi_changed_msg",
                      dx_canvas_dpi_changed_msg,
                      dx_canvas_msg);

static void dx_canvas_dpi_changed_msg_destruct(dx_canvas_dpi_changed_msg* SELF)
{/*Intentionally empty.*/}

static void dx_canvas_dpi_changed_msg_constructDispatch(dx_canvas_dpi_changed_msg_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_canvas_dpi_changed_msg_construct(dx_canvas_dpi_changed_msg* SELF, Core_Real32 horizontal_dpi, Core_Real32 vertical_dpi) {
  DX_CONSTRUCT_PREFIX(dx_canvas_dpi_changed_msg);
  if (dx_canvas_msg_construct(DX_CANVAS_MSG(SELF), dx_canvas_msg_kind_dpi_changed)) {
    return Core_Failure;
  }
  SELF->horizontal_dpi = horizontal_dpi;
  SELF->vertical_dpi = vertical_dpi;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_canvas_dpi_changed_msg_create(dx_canvas_dpi_changed_msg** RETURN, Core_Real32 horizontal_dpi, Core_Real32 vertical_dpi) {
  DX_CREATE_PREFIX(dx_canvas_dpi_changed_msg);
  if (dx_canvas_dpi_changed_msg_construct(SELF, horizontal_dpi, vertical_dpi)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
