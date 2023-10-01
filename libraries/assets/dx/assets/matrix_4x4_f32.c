#include "dx/assets/matrix_4x4_f32.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.matrix_4x4_f32",
                      dx_assets_matrix_4x4_f32,
                      dx_object);

static void dx_assets_matrix_4x4_f32_destruct(dx_assets_matrix_4x4_f32* SELF)
{/*Intentionally empty.*/}

static void dx_assets_matrix_4x4_f32_dispatch_construct(dx_assets_matrix_4x4_f32_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_matrix_4x4_f32_construct(dx_assets_matrix_4x4_f32* SELF, DX_MAT4 const* value) {
  dx_rti_type* TYPE = dx_assets_matrix_4x4_f32_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->value = *value;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_matrix_4x4_f32_create(dx_assets_matrix_4x4_f32** RETURN, DX_MAT4 const* value) {
  DX_CREATE_PREFIX(dx_assets_matrix_4x4_f32)
  if (dx_assets_matrix_4x4_f32_construct(SELF, value)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_assets_matrix_4x4_f32_get_value(DX_MAT4* RETURN, dx_assets_matrix_4x4_f32* SELF) {
  if (!SELF || !RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->value;
  return DX_SUCCESS;
}
