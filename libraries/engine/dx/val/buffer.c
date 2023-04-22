#include "dx/val/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.val.buffer",
                      dx_val_buffer,
                      dx_object)

static void dx_val_buffer_destruct(dx_val_buffer* self) {
  self->context = NULL;
}

static void dx_val_buffer_dispatch_construct(dx_val_buffer_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_val_buffer_construct(dx_val_buffer* SELF, dx_val_context* context) {
  dx_rti_type* TYPE = dx_val_buffer_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->context = context;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
