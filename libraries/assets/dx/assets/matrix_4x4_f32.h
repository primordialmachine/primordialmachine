#if !defined(DX_ASSETS_MATRIX_4X4_F32_H_INCLUDED)
#define DX_ASSETS_MATRIX_4X4_F32_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("dx.assets.matrix_4x4_f32",
                       dx_assets_matrix_4x4_f32,
                       Core_Object);

static inline dx_assets_matrix_4x4_f32* DX_ASSETS_MATRIX_4X4_F32(void* p) {
  return (dx_assets_matrix_4x4_f32*)p;
}

struct dx_assets_matrix_4x4_f32 {
  Core_Object _parent;
  DX_MAT4 value;
};

static inline dx_assets_matrix_4x4_f32_Dispatch* DX_ASSETS_MATRIX_4X4_F32_DISPATCH(void* p) {
  return (dx_assets_matrix_4x4_f32_Dispatch*)p;
}

struct dx_assets_matrix_4x4_f32_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @detail The matrix is initialized with the specified values.
/// @param value A pointer to a <code>DX_MAT4</code> object from which the values are read.
/// @constructor{dx_assets_matrix_4x4_f32} 
Core_Result dx_assets_matrix_4x4_f32_construct(dx_assets_matrix_4x4_f32* SELF, DX_MAT4 const* value);

Core_Result dx_assets_matrix_4x4_f32_create(dx_assets_matrix_4x4_f32** RETURN, DX_MAT4 const* value);

/// @brief Get the values.
/// @param RETURN A pointer to a <code>DX_MAT4</code> object.
/// @success <code>*RETURN</code> was assigned the component values.
/// @constructor{dx_assets_matrix_4x4_f32}
Core_Result dx_assets_matrix_4x4_f32_get_value(DX_MAT4* RETURN, dx_assets_matrix_4x4_f32* SELF);

#endif // DX_ASSETS_MATRIX_4X4_F32_H_INCLUDED
