#include "dx/assets/reference.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.reference",
                      dx_asset_reference,
                      Core_Object);

static void dx_asset_reference_destruct(dx_asset_reference* self) {
  if (self->object) {
    DX_UNREFERENCE(self->object);
    self->object = NULL;
  }
  DX_UNREFERENCE(self->name);
  self->name = NULL;
}

static void dx_asset_reference_constructDispatch(dx_asset_reference_dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_asset_reference_construct(dx_asset_reference* SELF, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_asset_reference);
  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->name = name;
  DX_REFERENCE(SELF->name);
  SELF->object = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_reference_create(dx_asset_reference** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(dx_asset_reference);
  if (dx_asset_reference_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
