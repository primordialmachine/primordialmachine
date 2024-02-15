// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "dx/assets/texture.h"

Core_defineObjectType("Core.Assets.Texture",
                      Core_Assets_Texture,
                      Core_Assets_Def);

static void Core_Assets_Texture_destruct(Core_Assets_Texture* SELF) {
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  if (SELF->image_reference) {
    CORE_UNREFERENCE(SELF->image_reference);
    SELF->image_reference = NULL;
  }
}

static void Core_Assets_Texture_constructDispatch(Core_Assets_Texture_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Texture_construct(Core_Assets_Texture* SELF, Core_String* name, Core_Assets_Ref* image_reference) {
  Core_BeginConstructor(Core_Assets_Texture);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
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
  Core_EndConstructor(Core_Assets_Texture);
}

Core_Result Core_Assets_Texture_create(Core_Assets_Texture** RETURN, Core_String* name, Core_Assets_Ref* image_reference) {
  DX_CREATE_PREFIX(Core_Assets_Texture);
  if (Core_Assets_Texture_construct(SELF, name, image_reference)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
