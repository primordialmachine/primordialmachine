#include "dx/assets/viewer.h"

#include "dx/assets/optics.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.viewer",
                      dx_asset_viewer,
                      dx_object);

static void dx_asset_viewer_destruct(dx_asset_viewer* self) {
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

static void dx_asset_viewer_dispatch_construct(dx_asset_viewer_dispatch* self)
{/*Intentionally empty.*/}

int dx_asset_viewer_construct(dx_asset_viewer* self, dx_string* name) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_viewer_get_type();
  if (!_type) {
    return 1;
  }

  if (!name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }

  self->name = name;
  DX_REFERENCE(name);

  self->optics = DX_ASSET_OPTICS(dx_asset_optics_perspective_create());
  if (!self->optics) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
    return 1;
  }

  self->controller = NULL;

  dx_vec3_set(&self->source, 0.f, 0.f, 0.f);
  dx_vec3_set(&self->target, 0.f, 0.f, -1.f);
  dx_vec3_set(&self->up, 0.f, 1.f, 0.f);

  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_viewer* dx_asset_viewer_create(dx_string* name) {
  dx_asset_viewer* self = DX_ASSET_VIEWER(dx_object_alloc(sizeof(dx_asset_viewer)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_viewer_construct(self, name)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
