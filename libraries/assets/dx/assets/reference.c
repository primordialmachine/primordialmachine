#include "dx/assets/reference.h"

Core_defineObjectType("dx.asset.reference",
                      dx_asset_reference,
                      Core_Object);

static void dx_asset_reference_destruct(dx_asset_reference* self) {
  if (self->object) {
    CORE_UNREFERENCE(self->object);
    self->object = NULL;
  }
  CORE_UNREFERENCE(self->name);
  self->name = NULL;
}

static void dx_asset_reference_constructDispatch(dx_asset_reference_Dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_asset_reference_construct(dx_asset_reference* SELF, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_asset_reference);
  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->name = name;
  CORE_REFERENCE(SELF->name);
  SELF->object = NULL;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_reference_create(dx_asset_reference** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(dx_asset_reference);
  if (dx_asset_reference_construct(SELF, name)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
