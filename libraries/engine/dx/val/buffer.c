#include "dx/val/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.val.buffer",
                      dx_val_buffer,
                      Core_Object);

static void dx_val_buffer_destruct(dx_val_buffer* self) {
  self->context = NULL;
}

static void dx_val_buffer_constructDispatch(dx_val_buffer_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_val_buffer_construct(dx_val_buffer* SELF, dx_val_context* context) {
  DX_CONSTRUCT_PREFIX(dx_val_buffer);
  SELF->context = context;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
