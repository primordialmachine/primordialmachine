#include "dx/val/buffer.h"

Core_defineObjectType("dx.val.buffer",
                      dx_val_buffer,
                      Core_Object);

static void dx_val_buffer_destruct(dx_val_buffer* SELF) {
  SELF->context = NULL;
}

static void dx_val_buffer_constructDispatch(dx_val_buffer_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_buffer_construct(dx_val_buffer* SELF, Core_Visuals_Context* context) {
  Core_BeginConstructor(dx_val_buffer);
  SELF->context = context;
  Core_EndConstructor(dx_val_buffer);
}
