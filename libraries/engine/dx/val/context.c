#include "dx/val/context.h"

DX_DEFINE_OBJECT_TYPE("dx.val.context",
                      dx_val_context,
                      dx_object)

static void dx_val_context_destruct(dx_val_context* self)
{/*Intentionally empty.*/}

static void dx_val_context_dispatch_construct(dx_val_context_dispatch* self) {
  self->get_information = NULL;
  self->bind_texture = NULL;
  self->create_vbinding = NULL;
  self->create_buffer = NULL;
  self->create_program = NULL;
  self->create_texture = NULL;
  self->execute_commands = NULL;
}

dx_result dx_val_context_construct(dx_val_context* self) {
  dx_rti_type* TYPE = dx_val_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(self)->type = TYPE;
  return DX_SUCCESS;
}
