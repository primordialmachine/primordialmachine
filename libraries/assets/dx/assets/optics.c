#include "dx/assets/optics.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.assets.optics",
                      dx_assets_optics,
                      Core_Object);

static void dx_assets_optics_destruct(dx_assets_optics* SELF)
{/*Intentionally empty.*/}

static void dx_assets_optics_constructDispatch(dx_assets_optics_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_optics_construct(dx_assets_optics* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_optics);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.asset.optics_orthographic",
                      dx_asset_optics_orthographic,
                      dx_assets_optics)

static void dx_asset_optics_orthographic_destruct(dx_asset_optics_orthographic* SELF) {
  if (SELF->scale_y) {
    Core_Memory_deallocate(SELF->scale_y);
    SELF->scale_y = NULL;
  }
  if (SELF->scale_x) {
    Core_Memory_deallocate(SELF->scale_x);
    SELF->scale_x = NULL;
  }
  if (SELF->aspect_ratio) {
    Core_Memory_deallocate(SELF->aspect_ratio);
    SELF->aspect_ratio = NULL;
  }
}

static void dx_asset_optics_orthographic_constructDispatch(dx_asset_optics_orthographic_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_optics_orthographic_construct(dx_asset_optics_orthographic* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_optics_orthographic);
  if (dx_assets_optics_construct(DX_ASSETS_OPTICS(SELF))) {
    return Core_Failure;
  }
  SELF->aspect_ratio = NULL;
  SELF->scale_x = NULL;
  SELF->scale_y = NULL;
  if (Core_Memory_allocate(&SELF->scale_x, sizeof(Core_Real32)) || Core_Memory_allocate(&SELF->scale_y, sizeof(Core_Real32))) {
    if (SELF->scale_y) {
      Core_Memory_deallocate(SELF->scale_y);
      SELF->scale_y = NULL;
    }
    if (SELF->scale_x) {
      Core_Memory_deallocate(SELF->scale_x);
      SELF->scale_x = NULL;
    }
    return 1;
  }
  *SELF->scale_x = 1.f;
  *SELF->scale_y = 1.f;
  SELF->near = 0.1f;
  SELF->far = 1000.f;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_optics_orthographic_create(dx_asset_optics_orthographic** RETURN) {
  DX_CREATE_PREFIX(dx_asset_optics_orthographic);
  if (dx_asset_optics_orthographic_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.asset.optics_perspective",
                      dx_asset_optics_perspective,
                      dx_assets_optics);

static void dx_asset_optics_perspective_destruct(dx_asset_optics_perspective* SELF) {
  if (SELF->aspect_ratio) {
    Core_Memory_deallocate(SELF->aspect_ratio);
    SELF->aspect_ratio = NULL;
  }
}

static void dx_asset_optics_perspective_constructDispatch(dx_asset_optics_perspective_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_optics_perspective_construct(dx_asset_optics_perspective* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_optics_perspective);
  if (dx_assets_optics_construct(DX_ASSETS_OPTICS(SELF))) {
    return Core_Failure;
  }
  if (Core_Memory_allocate(&SELF->aspect_ratio, sizeof(Core_Real32))) {
    return Core_Failure;
  }
  *SELF->aspect_ratio = 1.33f;
  SELF->field_of_view_y = 60.f;
  SELF->near = 0.1f;
  SELF->far = 1000.f;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_optics_perspective_create(dx_asset_optics_perspective** RETURN) {
  DX_CREATE_PREFIX(dx_asset_optics_perspective);
  if (dx_asset_optics_perspective_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success; 
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
