#include "dx/val/context.h"

DX_DEFINE_OBJECT_TYPE("dx.val.context",
                      dx_val_context,
                      dx_context);

static void dx_val_context_destruct(dx_val_context* SELF)
{/*Intentionally empty.*/}

static void dx_val_context_dispatch_construct(dx_val_context_dispatch* SELF) {
  SELF->get_information = NULL;
  SELF->bind_texture = NULL;
  SELF->create_vbinding = NULL;
  SELF->create_buffer = NULL;
  SELF->create_program = NULL;
  SELF->create_texture = NULL;
  SELF->execute_commands = NULL;
}

dx_result dx_val_context_construct(dx_val_context* SELF) {
  dx_rti_type* TYPE = dx_val_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_context_construct(DX_CONTEXT(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
