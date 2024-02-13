#if !defined(CORE_ASSETS_VECTOR3R32_H_INCLUDED)
#define CORE_ASSETS_VECTOR3R32_H_INCLUDED

#include "Core/Assets/Def.h"

Core_declareObjectType("Core.Assets.Vector3R32",
                       Core_Assets_Vector3R32,
                       Core_Assets_Def);

static inline Core_Assets_Vector3R32* CORE_ASSETS_VECTOR3R32(void* p) {
  return (Core_Assets_Vector3R32*)p;
}

struct Core_Assets_Vector3R32 {
  Core_Assets_Def _parent;
  DX_VEC3 value;
};

static inline Core_Assets_Vector3R32_Dispatch* CORE_ASSETS_VECTOR3R32_DISPATCH(void* p) {
  return (Core_Assets_Vector3R32_Dispatch*)p;
}

struct Core_Assets_Vector3R32_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @detail The vector is initialized with the specified values.
/// @param value A pointer to a <code>DX_VEC3</code> object from which the values are read.
/// @constructor{Core_Assets_Vector3R32} 
Core_Result Core_Assets_Vector3R32_construct(Core_Assets_Vector3R32* SELF, DX_VEC3 const* value);

Core_Result Core_Assets_Vector3R32_create(Core_Assets_Vector3R32** RETURN, DX_VEC3 const* value);

/// @brief Get the values.
/// @param RETURN A pointer to a <code>DX_VEC3</code> object.
/// @success <code>*RETURN</code> was assigned the component values.
/// @constructor{Core_Assets_Vector3R32}
Core_Result Core_Assets_Vector3R32_get_value(DX_VEC3* RETURN, Core_Assets_Vector3R32* SELF);

#endif // CORE_ASSETS_VECTOR3R32_H_INCLUDED
