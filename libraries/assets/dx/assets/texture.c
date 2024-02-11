#include "dx/assets/texture.h"

Core_defineObjectType("dx.assets.texture",
                      dx_assets_texture,
                      Core_Object);

static void dx_assets_texture_destruct(dx_assets_texture* SELF) {
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  if (SELF->image_reference) {
    CORE_UNREFERENCE(SELF->image_reference);
    SELF->image_reference = NULL;
  }
}

static void dx_assets_texture_constructDispatch(dx_assets_texture_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_texture_construct(dx_assets_texture* SELF, Core_String* name, Core_Assets_Ref* image_reference) {
  DX_CONSTRUCT_PREFIX(dx_assets_texture);
  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->name = name;
  CORE_REFERENCE(name);
  
  if (!image_reference) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->image_reference = image_reference;
  CORE_REFERENCE(SELF->image_reference);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_texture_create(dx_assets_texture** RETURN, Core_String* name, Core_Assets_Ref* image_reference) {
  DX_CREATE_PREFIX(dx_assets_texture);
  if (dx_assets_texture_construct(SELF, name, image_reference)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
