#if !defined(DX_ASSETS_VECTOR_3_F32_H_INCLUDED)
#define DX_ASSETS_VECTOR_3_F32_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.assets.vector_3_f32",
                       dx_assets_vector_3_f32,
                       dx_object);

static inline dx_assets_vector_3_f32* DX_ASSETS_VECTOR3_F32(void* p) {
  return (dx_assets_vector_3_f32*)p;
}

struct dx_assets_vector_3_f32 {
  dx_object _parent;
  DX_VEC3 value;
};

static inline dx_assets_vector_3_f32_dispatch* DX_ASSETS_VECTOR_R3_F32_DISPATCH(void* p) {
  return (dx_assets_vector_3_f32_dispatch*)p;
}

struct dx_assets_vector_3_f32_dispatch {
  dx_object_dispatch _parent;
};

/// @detail The vector is initialized with the specified values.
/// @param value A pointer to a <code>DX_VEC3</code> object from which the values are read.
/// @constructor{dx_assets_vector_3_f32} 
dx_result dx_assets_vector_3_f32_construct(dx_assets_vector_3_f32* SELF, DX_VEC3 const* value);

dx_result dx_assets_vector_3_f32_create(dx_assets_vector_3_f32** RETURN, DX_VEC3 const* value);

/// @brief Get the values.
/// @param RETURN A pointer to a <code>DX_VEC3</code> object.
/// @success <code>*RETURN</code> was assigned the component values.
/// @constructor{dx_assets_vector_3_f32}
dx_result dx_assets_vector_3_f32_get_value(DX_VEC3* RETURN, dx_assets_vector_3_f32* SELF);

#endif // DX_ASSETS_VECTOR_3_F32_H_INCLUDED
