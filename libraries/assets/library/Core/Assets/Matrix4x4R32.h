// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_ASSETS_MATRIX4X4R32_H_INCLUDED)
#define CORE_ASSETS_MATRIX4X4R32_H_INCLUDED

#include "Core/Assets/Def.h"

Core_declareObjectType("Core.Assets.Matrix4x4R32",
                       Core_Assets_Matrix4x4R32,
                       Core_Assets_Def);

static inline Core_Assets_Matrix4x4R32* DX_ASSETS_MATRIX_4X4_F32(void* p) {
  return (Core_Assets_Matrix4x4R32*)p;
}

struct Core_Assets_Matrix4x4R32 {
  Core_Assets_Def _parent;
  DX_MAT4 value;
};

static inline Core_Assets_Matrix4x4R32_Dispatch* DX_ASSETS_MATRIX_4X4_F32_DISPATCH(void* p) {
  return (Core_Assets_Matrix4x4R32_Dispatch*)p;
}

struct Core_Assets_Matrix4x4R32_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @detail The matrix is initialized with the specified values.
/// @param value A pointer to a <code>DX_MAT4</code> object from which the values are read.
/// @constructor{Core_Assets_Matrix4x4R32} 
Core_Result Core_Assets_Matrix4x4R32_construct(Core_Assets_Matrix4x4R32* SELF, DX_MAT4 const* value);

Core_Result Core_Assets_Matrix4x4R32_create(Core_Assets_Matrix4x4R32** RETURN, DX_MAT4 const* value);

/// @brief Get the values.
/// @param RETURN A pointer to a <code>DX_MAT4</code> object.
/// @success <code>*RETURN</code> was assigned the component values.
/// @constructor{Core_Assets_Matrix4x4R32}
Core_Result Core_Assets_Matrix4x4R32_get_value(DX_MAT4* RETURN, Core_Assets_Matrix4x4R32* SELF);

#endif // CORE_ASSETS_MATRIX4X4R32_H_INCLUDED
