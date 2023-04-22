#include "dx/val/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.val.texture",
                      dx_val_texture,
                      dx_object);

static void dx_val_texture_destruct(dx_val_texture* SELF) {
  SELF->context = NULL;
}

static void dx_val_texture_dispatch_construct(dx_val_texture_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_texture_construct(dx_val_texture* SELF, dx_val_context* context) {
  dx_rti_type* TYPE = dx_val_texture_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->context = context;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
