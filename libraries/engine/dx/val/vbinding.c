#include "dx/val/vbinding.h"

#include "dx/val/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.val.vbinding",
                      dx_val_vbinding,
                      dx_object);

static void dx_val_vbinding_destruct(dx_val_vbinding* SELF) {
  if (SELF->buffer) {
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
  }
  SELF->context = NULL;
}

static void dx_val_vbinding_dispatch_construct(dx_val_vbinding_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_vbinding_construct(dx_val_vbinding* SELF, dx_val_buffer* buffer) {
  dx_rti_type* TYPE = dx_val_vbinding_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->buffer = buffer;
  DX_REFERENCE(SELF->buffer);
  SELF->context = buffer->context;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
