#include "dx/val/vbinding.h"

#include "dx/val/buffer.h"

Core_defineObjectType("dx.val.vbinding",
                      dx_val_vbinding,
                      Core_Object);

static void dx_val_vbinding_destruct(dx_val_vbinding* SELF) {
  if (SELF->buffer) {
    CORE_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
  }
  SELF->context = NULL;
}

static void dx_val_vbinding_constructDispatch(dx_val_vbinding_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_vbinding_construct(dx_val_vbinding* SELF, dx_val_buffer* buffer) {
  DX_CONSTRUCT_PREFIX(dx_val_vbinding);
  SELF->buffer = buffer;
  CORE_REFERENCE(SELF->buffer);
  SELF->context = buffer->context;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
