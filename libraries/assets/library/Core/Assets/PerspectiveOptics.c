// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Assets/PerspectiveOptics.h"

Core_defineObjectType("Core.Assets.PerspectiveOptics",
                      Core_Assets_PerspectiveOptics,
                      Core_Assets_Optics);

static void Core_Assets_PerspectiveOptics_destruct(Core_Assets_PerspectiveOptics* SELF) {
  if (SELF->aspectRatio) {
    Core_Memory_deallocate(SELF->aspectRatio);
    SELF->aspectRatio = NULL;
  }
}

static void Core_Assets_PerspectiveOptics_constructDispatch(Core_Assets_PerspectiveOptics_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Assets_PerspectiveOptics_construct(Core_Assets_PerspectiveOptics* SELF) {
  Core_BeginConstructor(Core_Assets_PerspectiveOptics);
  if (Core_Assets_Optics_construct(CORE_ASSETS_OPTICS(SELF))) {
    return Core_Failure;
  }
  if (Core_Memory_allocate(&SELF->aspectRatio, sizeof(Core_Real32))) {
    return Core_Failure;
  }
  *SELF->aspectRatio = 1.33f;
  SELF->fieldOfViewY = 60.f;
  SELF->near = 0.1f;
  SELF->far = 1000.f;
  Core_EndConstructor(Core_Assets_PerspectiveOptics);
}

Core_Result Core_Assets_PerspectiveOptics_create(Core_Assets_PerspectiveOptics** RETURN) {
  DX_CREATE_PREFIX(Core_Assets_PerspectiveOptics);
  if (Core_Assets_PerspectiveOptics_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success; 
}
