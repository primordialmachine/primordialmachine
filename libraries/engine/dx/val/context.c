#include "dx/val/context.h"

DX_DEFINE_OBJECT_TYPE("dx.val.context",
                      dx_val_context,
                      dx_context);

static void dx_val_context_destruct(dx_val_context* SELF)
{/*Intentionally empty.*/}

static void dx_val_context_constructDispatch(dx_val_context_dispatch* SELF) {
  SELF->get_information = NULL;
  SELF->bind_texture = NULL;
  SELF->create_vbinding = NULL;
  SELF->create_buffer = NULL;
  SELF->create_program = NULL;
  SELF->create_texture = NULL;
  SELF->execute_commands = NULL;
}

Core_Result dx_val_context_construct(dx_val_context* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_context);
  if (dx_context_construct(DX_CONTEXT(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
