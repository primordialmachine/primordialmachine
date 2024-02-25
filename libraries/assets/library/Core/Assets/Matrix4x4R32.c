// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/Matrix4x4R32.h"

Core_defineObjectType("Core.Assets.Matrix4x4R32",
                      Core_Assets_Matrix4x4R32,
                      Core_Assets_Def);

static void Core_Assets_Matrix4x4R32_destruct(Core_Assets_Matrix4x4R32* SELF)
{/*Intentionally empty.*/}

static void Core_Assets_Matrix4x4R32_constructDispatch(Core_Assets_Matrix4x4R32_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_Matrix4x4R32_construct(Core_Assets_Matrix4x4R32* SELF, DX_MAT4 const* value) {
  Core_BeginConstructor(Core_Assets_Matrix4x4R32);
  if (!value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_Assets_Def_construct(CORE_ASSETS_DEF(SELF))) {
    return Core_Failure;
  }
  SELF->value = *value;
  Core_EndConstructor(Core_Assets_Matrix4x4R32);
}

Core_Result Core_Assets_Matrix4x4R32_create(Core_Assets_Matrix4x4R32** RETURN, DX_MAT4 const* value) {
  DX_CREATE_PREFIX(Core_Assets_Matrix4x4R32);
  if (Core_Assets_Matrix4x4R32_construct(SELF, value)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Assets_Matrix4x4R32_get_value(DX_MAT4* RETURN, Core_Assets_Matrix4x4R32* SELF) {
  if (!SELF || !RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->value;
  return Core_Success;
}
