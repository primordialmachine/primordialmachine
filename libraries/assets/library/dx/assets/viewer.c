// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#include "dx/assets/viewer.h"

#include "Core/Assets/Optics.h"
#include "Core/Assets/OrthographicOptics.h"
#include "Core/Assets/PerspectiveOptics.h"

Core_defineObjectType("dx.assets.viewer",
                      dx_assets_viewer,
                      Core_Object);

static void dx_assets_viewer_destruct(dx_assets_viewer* self) {
  if (self->controller) {
    CORE_UNREFERENCE(self->controller);
    self->controller = NULL;
  }
  if (self->optics) {
    CORE_UNREFERENCE(self->optics);
    self->optics = NULL;
  }
  CORE_UNREFERENCE(self->name);
  self->name = NULL;
}

static void dx_assets_viewer_constructDispatch(dx_assets_viewer_Dispatch* self)
{/*Intentionally empty.*/}

Core_Result dx_assets_viewer_construct(dx_assets_viewer* SELF, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_assets_viewer);

  if (!name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }

  SELF->name = name;
  CORE_REFERENCE(name);

  if (Core_Assets_PerspectiveOptics_create((Core_Assets_PerspectiveOptics**)&SELF->optics)) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    return Core_Failure;
  }

  SELF->controller = NULL;

  dx_vec3_set(&SELF->source, 0.f, 0.f, 0.f);
  dx_vec3_set(&SELF->target, 0.f, 0.f, -1.f);
  dx_vec3_set(&SELF->up, 0.f, 1.f, 0.f);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_viewer_create(dx_assets_viewer** RETURN, Core_String* name) {
  DX_CREATE_PREFIX(dx_assets_viewer);
  if (dx_assets_viewer_construct(SELF, name)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
