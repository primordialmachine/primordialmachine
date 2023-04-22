#include "dx/core/canvas_msgs.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_ENUMERATION_TYPE("dx.canvas_msg_kind",
                           dx_canvas_msg_kind);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.canvas_msg",
                      dx_canvas_msg,
                      dx_msg);

static void dx_canvas_msg_destruct(dx_canvas_msg* SELF)
{/*Intentionally empty.*/}

static void dx_canvas_msg_dispatch_construct(dx_canvas_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_canvas_msg_construct(dx_canvas_msg* SELF, dx_canvas_msg_kind kind) {
  dx_rti_type* TYPE = dx_canvas_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_msg_construct(DX_MSG(SELF))) {
    return DX_FAILURE;
  }
  SELF->kind = kind;
  DX_MSG(SELF)->flags = DX_MSG_TYPE_CANVAS;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_canvas_msg_create(dx_canvas_msg** RETURN, dx_canvas_msg_kind kind) {
  dx_rti_type* TYPE = dx_canvas_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  dx_canvas_msg* SELF = DX_CANVAS_MSG(dx_object_alloc(sizeof(dx_canvas_msg)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_canvas_msg_construct(SELF, kind)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_canvas_msg_get_kind(dx_canvas_msg_kind* RETURN, dx_canvas_msg* SELF) {
  *RETURN = SELF->kind;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.canvas_size_changed_msg",
                      dx_canvas_size_changed_msg,
                      dx_canvas_msg);

static void dx_canvas_size_changed_msg_destruct(dx_canvas_size_changed_msg* SELF)
{/*Intentionally empty.*/}

static void dx_canvas_size_changed_msg_dispatch_construct(dx_canvas_size_changed_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_canvas_size_changed_msg_construct(dx_canvas_size_changed_msg* SELF, dx_f32 width, dx_f32 height) {
  dx_rti_type* TYPE = dx_canvas_size_changed_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_canvas_msg_construct(DX_CANVAS_MSG(SELF), dx_canvas_msg_kind_size_changed)) {
    return DX_FAILURE;
  }
  SELF->width = width;
  SELF->height = height;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_canvas_size_changed_msg_create(dx_canvas_size_changed_msg** RETURN, dx_f32 width, dx_f32 height) {
  dx_canvas_size_changed_msg* SELF = DX_CANVAS_SIZE_CHANGED_MSG(dx_object_alloc(sizeof(dx_canvas_size_changed_msg)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_canvas_size_changed_msg_construct(SELF, width, height)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.canvas_dpi_changed_msg",
                      dx_canvas_dpi_changed_msg,
                      dx_canvas_msg);

static void dx_canvas_dpi_changed_msg_destruct(dx_canvas_dpi_changed_msg* SELF)
{/*Intentionally empty.*/}

static void dx_canvas_dpi_changed_msg_dispatch_construct(dx_canvas_dpi_changed_msg_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_canvas_dpi_changed_msg_construct(dx_canvas_dpi_changed_msg* SELF, dx_f32 horizontal_dpi, dx_f32 vertical_dpi) {
  dx_rti_type* TYPE = dx_canvas_size_changed_msg_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_canvas_msg_construct(DX_CANVAS_MSG(SELF), dx_canvas_msg_kind_dpi_changed)) {
    return DX_FAILURE;
  }
  SELF->horizontal_dpi = horizontal_dpi;
  SELF->vertical_dpi = vertical_dpi;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_canvas_dpi_changed_msg_create(dx_canvas_dpi_changed_msg** RETURN, dx_f32 horizontal_dpi, dx_f32 vertical_dpi) {
  dx_canvas_dpi_changed_msg* SELF = DX_CANVAS_DPI_CHANGED_MSG(dx_object_alloc(sizeof(dx_canvas_dpi_changed_msg)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_canvas_dpi_changed_msg_construct(SELF, horizontal_dpi, vertical_dpi)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
