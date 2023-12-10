#include "dx/val/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.val.texture",
                      dx_val_texture,
                      Core_Object);

static void dx_val_texture_destruct(dx_val_texture* SELF) {
  SELF->context = NULL;
}

static void dx_val_texture_constructDispatch(dx_val_texture_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_texture_construct(dx_val_texture* SELF, dx_val_context* context) {
  DX_CONSTRUCT_PREFIX(dx_val_texture);
  SELF->context = context;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
