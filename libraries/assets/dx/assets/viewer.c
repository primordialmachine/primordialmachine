#include "dx/assets/viewer.h"

#include "dx/assets/optics.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.viewer",
                      dx_assets_viewer,
                      dx_object);

static void dx_assets_viewer_destruct(dx_assets_viewer* self) {
  if (self->controller) {
    DX_UNREFERENCE(self->controller);
    self->controller = NULL;
  }
  if (self->optics) {
    DX_UNREFERENCE(self->optics);
    self->optics = NULL;
  }
  DX_UNREFERENCE(self->name);
  self->name = NULL;
}

static void dx_assets_viewer_dispatch_construct(dx_assets_viewer_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_assets_viewer_construct(dx_assets_viewer* SELF, dx_string* name) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_assets_viewer_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (!name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }

  SELF->name = name;
  DX_REFERENCE(name);

  if (dx_asset_optics_perspective_create((dx_asset_optics_perspective**)&SELF->optics)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    return DX_FAILURE;
  }

  SELF->controller = NULL;

  dx_vec3_set(&SELF->source, 0.f, 0.f, 0.f);
  dx_vec3_set(&SELF->target, 0.f, 0.f, -1.f);
  dx_vec3_set(&SELF->up, 0.f, 1.f, 0.f);

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_viewer_create(dx_assets_viewer** RETURN, dx_string* name) {
  DX_CREATE_PREFIX(dx_assets_viewer)
  if (dx_assets_viewer_construct(SELF, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
