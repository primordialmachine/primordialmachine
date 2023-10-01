#include "dx/assets/reference.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.reference",
                      dx_asset_reference,
                      dx_object)

static void dx_asset_reference_destruct(dx_asset_reference* self) {
  if (self->object) {
    DX_UNREFERENCE(self->object);
    self->object = NULL;
  }
  DX_UNREFERENCE(self->name);
  self->name = NULL;
}

static void dx_asset_reference_dispatch_construct(dx_asset_reference_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_asset_reference_construct(dx_asset_reference* SELF, dx_string* name) {
  dx_rti_type* TYPE = dx_asset_reference_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!SELF || !name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->name = name;
  DX_REFERENCE(SELF->name);
  SELF->object = NULL;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_reference_create(dx_asset_reference** RETURN, dx_string* name) {
  DX_CREATE_PREFIX(dx_asset_reference)
  if (dx_asset_reference_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
