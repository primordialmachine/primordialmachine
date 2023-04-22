#include "dx/val/program.h"

DX_DEFINE_OBJECT_TYPE("dx.val.program",
                      dx_val_program,
                      dx_object);

static void dx_val_program_destruct(dx_val_program* SELF) {
  SELF->ctx = NULL;
}

static void dx_val_program_dispatch_construct(dx_val_program_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_program_construct(dx_val_program* SELF, dx_val_context* ctx) {
  dx_rti_type* TYPE = dx_val_program_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->ctx = ctx;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
