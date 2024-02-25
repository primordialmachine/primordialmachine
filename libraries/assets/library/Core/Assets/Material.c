// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/Material.h"

Core_defineObjectType("Core.Assets.Material",
                      Core_Assets_Material,
                      Core_Assets_Def);

static void Core_Assets_Material_destruct(Core_Assets_Material* SELF) {
  if (SELF->ambientColor) {
    CORE_UNREFERENCE(SELF->ambientColor);
    SELF->ambientColor = NULL;
  }
  if (SELF->controller) {
    CORE_UNREFERENCE(SELF->controller);
    SELF->controller = NULL;
  }
  if (SELF->ambientTextureReference) {
    CORE_UNREFERENCE(SELF->ambientTextureReference);
    SELF->ambientTextureReference = NULL;
  }
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
}

static void Core_Assets_Material_constructDispatch(Core_Assets_Material_Dispatch* self)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Material_construct(Core_Assets_Material* SELF, Core_String* name) {
  Core_BeginConstructor(Core_Assets_Material);
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->name = name;
  CORE_REFERENCE(SELF->name);

  SELF->ambientColor = NULL;
  
  SELF->ambientTextureReference = NULL;
  
  SELF->controller = NULL;
  Core_EndConstructor(Core_Assets_Material);
}

Core_Result Core_Assets_Material_create(Core_Assets_Material** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(Core_Assets_Material);
  if (Core_Assets_Material_construct(SELF, name)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Assets_Material_setAmbientColor(Core_Assets_Material* SELF, Core_Assets_ColorRgbN8* ambientColor) {
  if (!SELF || !ambientColor) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!SELF->ambientColor) {
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous") - 1)) {
      return Core_Failure;
    }
    if (Core_Assets_Ref_create(&SELF->ambientColor, name)) {
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
  }
  CORE_REFERENCE(ambientColor);
  if (SELF->ambientColor->object) {
    CORE_UNREFERENCE(SELF->ambientColor->object);
  }
  SELF->ambientColor->object = CORE_OBJECT(ambientColor);
  return Core_Success;
}

Core_Result Core_Assets_Material_setAmbientTexture(Core_Assets_Material* SELF, Core_Assets_Ref* ambientTextureReference) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (ambientTextureReference) {
    CORE_REFERENCE(ambientTextureReference);
  }
  if (SELF->ambientTextureReference) {
    CORE_UNREFERENCE(SELF->ambientTextureReference);
  }
  SELF->ambientTextureReference = ambientTextureReference;
  return Core_Success;
}
