#include "dx/assets/texture.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.texture",
                      dx_asset_texture,
                      dx_object)

static void dx_asset_texture_destruct(dx_asset_texture* self) {
  if (self->name) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
  }
  if (self->image_reference) {
    DX_UNREFERENCE(self->image_reference);
    self->image_reference = NULL;
  }
}

static void dx_asset_texture_dispatch_construct(dx_asset_texture_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_asset_texture_construct(dx_asset_texture* SELF, dx_string* name, dx_asset_reference* image_reference) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_asset_texture_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (!name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->name = name;
  DX_REFERENCE(name);
  
  if (!image_reference) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->image_reference = image_reference;
  DX_REFERENCE(SELF->image_reference);

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_texture_create(dx_asset_texture** RETURN, dx_string* name, dx_asset_reference* image_reference) {
  DX_CREATE_PREFIX(dx_asset_texture)
  if (dx_asset_texture_construct(SELF, name, image_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
