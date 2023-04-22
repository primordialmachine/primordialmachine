#include "dx/gl/application.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.application",
                      dx_gl_application,
                      dx_application);

static void dx_gl_application_destruct(dx_gl_application* SELF) {
  DX_UNREFERENCE(SELF->mouse_state);
  SELF->mouse_state = NULL;
  DX_UNREFERENCE(SELF->keyboard_state);
  SELF->keyboard_state = NULL;
}

static void dx_gl_application_dispatch_construct(dx_gl_application_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_gl_application_construct(dx_gl_application* SELF, dx_msg_queue* msg_queue) {
  dx_rti_type* TYPE = dx_gl_application_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  //
  if (dx_application_construct(DX_APPLICATION(SELF))) {
    return DX_FAILURE;
  }
  //
  SELF->msg_queue = msg_queue;
  //
  if (dx_keyboard_state_create(&SELF->keyboard_state)) {
    return DX_FAILURE;
  }
  if (dx_mouse_state_create(&SELF->mouse_state)) {
    DX_UNREFERENCE(SELF->keyboard_state);
    SELF->keyboard_state = NULL;
    return DX_FAILURE;
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
