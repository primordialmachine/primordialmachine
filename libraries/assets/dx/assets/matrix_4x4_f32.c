#include "dx/assets/matrix_4x4_f32.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.matrix_4x4_f32",
                      dx_assets_matrix_4x4_f32,
                      Core_Object);

static void dx_assets_matrix_4x4_f32_destruct(dx_assets_matrix_4x4_f32* SELF)
{/*Intentionally empty.*/}

static void dx_assets_matrix_4x4_f32_constructDispatch(dx_assets_matrix_4x4_f32_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_matrix_4x4_f32_construct(dx_assets_matrix_4x4_f32* SELF, DX_MAT4 const* value) {
  DX_CONSTRUCT_PREFIX(dx_assets_matrix_4x4_f32);
  if (!value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->value = *value;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_matrix_4x4_f32_create(dx_assets_matrix_4x4_f32** RETURN, DX_MAT4 const* value) {
  DX_CREATE_PREFIX(dx_assets_matrix_4x4_f32);
  if (dx_assets_matrix_4x4_f32_construct(SELF, value)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_matrix_4x4_f32_get_value(DX_MAT4* RETURN, dx_assets_matrix_4x4_f32* SELF) {
  if (!SELF || !RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->value;
  return Core_Success;
}
