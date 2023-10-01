#include "dx/assets/optics.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.optics",
                      dx_assets_optics,
                      dx_object);

static void dx_assets_optics_destruct(dx_assets_optics* SELF)
{/*Intentionally empty.*/}

static void dx_assets_optics_dispatch_construct(dx_assets_optics_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_optics_construct(dx_assets_optics* SELF) {
  dx_rti_type* TYPE = dx_assets_optics_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.optics_orthographic",
                      dx_asset_optics_orthographic,
                      dx_assets_optics)

static void dx_asset_optics_orthographic_destruct(dx_asset_optics_orthographic* SELF) {
  if (SELF->scale_y) {
    dx_memory_deallocate(SELF->scale_y);
    SELF->scale_y = NULL;
  }
  if (SELF->scale_x) {
    dx_memory_deallocate(SELF->scale_x);
    SELF->scale_x = NULL;
  }
  if (SELF->aspect_ratio) {
    dx_memory_deallocate(SELF->aspect_ratio);
    SELF->aspect_ratio = NULL;
  }
}

static void dx_asset_optics_orthographic_dispatch_construct(dx_asset_optics_orthographic_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_optics_orthographic_construct(dx_asset_optics_orthographic* SELF) {
  dx_rti_type* TYPE = dx_asset_optics_orthographic_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_assets_optics_construct(DX_ASSETS_OPTICS(SELF))) {
    return DX_FAILURE;
  }
  SELF->aspect_ratio = NULL;
  SELF->scale_x = NULL;
  SELF->scale_y = NULL;
  if (dx_memory_allocate(&SELF->scale_x, sizeof(dx_f32)) || dx_memory_allocate(&SELF->scale_y, sizeof(dx_f32))) {
    if (SELF->scale_y) {
      dx_memory_deallocate(SELF->scale_y);
      SELF->scale_y = NULL;
    }
    if (SELF->scale_x) {
      dx_memory_deallocate(SELF->scale_x);
      SELF->scale_x = NULL;
    }
    return 1;
  }
  *SELF->scale_x = 1.f;
  *SELF->scale_y = 1.f;
  SELF->near = 0.1f;
  SELF->far = 1000.f;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_optics_orthographic_create(dx_asset_optics_orthographic** RETURN) {
  DX_CREATE_PREFIX(dx_asset_optics_orthographic)
  if (dx_asset_optics_orthographic_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.optics_perspective",
                      dx_asset_optics_perspective,
                      dx_assets_optics);

static void dx_asset_optics_perspective_destruct(dx_asset_optics_perspective* SELF) {
  if (SELF->aspect_ratio) {
    dx_memory_deallocate(SELF->aspect_ratio);
    SELF->aspect_ratio = NULL;
  }
}

static void dx_asset_optics_perspective_dispatch_construct(dx_asset_optics_perspective_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_optics_perspective_construct(dx_asset_optics_perspective* SELF) {
  dx_rti_type* TYPE = dx_asset_optics_perspective_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_assets_optics_construct(DX_ASSETS_OPTICS(SELF))) {
    return DX_FAILURE;
  }
  if (dx_memory_allocate(&SELF->aspect_ratio, sizeof(dx_f32))) {
    return DX_FAILURE;
  }
  *SELF->aspect_ratio = 1.33f;
  SELF->field_of_view_y = 60.f;
  SELF->near = 0.1f;
  SELF->far = 1000.f;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_optics_perspective_create(dx_asset_optics_perspective** RETURN) {
  DX_CREATE_PREFIX(dx_asset_optics_perspective)
  if (dx_asset_optics_perspective_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS; 
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
