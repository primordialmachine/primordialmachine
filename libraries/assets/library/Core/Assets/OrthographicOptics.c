// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/OrthographicOptics.h"

Core_defineObjectType("Core.Assets.OrthographicOptics",
                      Core_Assets_OrthographicOptics,
                      Core_Assets_Optics);

static void Core_Assets_OrthographicOptics_destruct(Core_Assets_OrthographicOptics* SELF) {
  if (SELF->scaleY) {
    Core_Memory_deallocate(SELF->scaleY);
    SELF->scaleY = NULL;
  }
  if (SELF->scaleX) {
    Core_Memory_deallocate(SELF->scaleX);
    SELF->scaleX = NULL;
  }
  if (SELF->aspectRatio) {
    Core_Memory_deallocate(SELF->aspectRatio);
    SELF->aspectRatio = NULL;
  }
}

static void Core_Assets_OrthographicOptics_constructDispatch(Core_Assets_OrthographicOptics_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_OrthographicOptics_construct(Core_Assets_OrthographicOptics* SELF) {
  Core_BeginConstructor(Core_Assets_OrthographicOptics);
  if (Core_Assets_Optics_construct(CORE_ASSETS_OPTICS(SELF))) {
    return Core_Failure;
  }
  SELF->aspectRatio = NULL;
  SELF->scaleX = NULL;
  SELF->scaleY = NULL;
  SELF->near = 0.1f;
  SELF->far = 1000.f;
  Core_EndConstructor(Core_Assets_OrthographicOptics);
}

Core_Result Core_Assets_OrthographicOptics_create(Core_Assets_OrthographicOptics** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_OrthographicOptics);
  if (Core_Assets_OrthographicOptics_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
