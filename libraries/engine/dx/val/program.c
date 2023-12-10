#include "dx/val/program.h"

DX_DEFINE_OBJECT_TYPE("dx.val.program",
                      dx_val_program,
                      Core_Object);

static void dx_val_program_destruct(dx_val_program* SELF) {
  SELF->ctx = NULL;
}

static void dx_val_program_constructDispatch(dx_val_program_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_program_construct(dx_val_program* SELF, dx_val_context* ctx) {
  DX_CONSTRUCT_PREFIX(dx_val_program);
  SELF->ctx = ctx;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
