// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_MATERIAL_H_INCLUDED)
#define CORE_ASSETS_MATERIAL_H_INCLUDED

#include "Core/Assets/Def.h"
#include "Core/Assets/Ref.h"
typedef struct Core_Assets_ColorRgbN8 Core_Assets_ColorRgbN8;
typedef struct Core_Assets_MaterialController Core_Assets_MaterialController;

/// @brief A material asset.
Core_declareObjectType("Core.Assets.Material",
                       Core_Assets_Material,
                       Core_Assets_Def);

static inline Core_Assets_Material* CORE_ASSETS_MATERIAL(void* p) {
    return (Core_Assets_Material*)p;
}

struct Core_Assets_Material {
  Core_Assets_Def _parent;
  /// @brief A pointer to the ADL name of this material.
  Core_String* name;
  /// @brief The ambient color of this material.
  Core_Assets_Ref* ambientColor;
  /// @brief Pointer to the ambient texture of this material or the null pointer.
  /// @default The null pointer.
  Core_Assets_Ref* ambientTextureReference;
  /// @brief A pointer to the viewer controller or a null pointer.
  Core_Assets_MaterialController* controller;
};

static inline Core_Assets_Material_Dispatch* CORE_ASSETS_MATERIAL_DISPATCH(void* p) {
  return (Core_Assets_Material_Dispatch*)p;
}

struct Core_Assets_Material_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @detail The material is constructed with default values.
/// @param name A pointer to the ADL name of this material.
/// @error #Core_Error_AllocationFailed an allocation failed
/// @constructor{Core_Assets_Material}
Core_Result Core_Assets_Material_construct(Core_Assets_Material* SELF, Core_String* name);

/// @create-operator{Core_Assets_Material}
/// @detail The object is created with default values.
/// @param name A pointer to the ADL name of this material.
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result Core_Assets_Material_create(Core_Assets_Material** RETURN, Core_String* name);

/// @brief Set the ambient color.
/// @param ambientColor A pointer to color.
/// @error #Core_Error_ArgumentInvalid @a ambientColor is a null pointer,
/// @method{Core_Assets_Material}
Core_Result Core_Assets_Material_setAmbientColor(Core_Assets_Material* SELF, Core_Assets_ColorRgbN8* ambientColor);

/// @brief Set the ambient texture.
/// @param ambientTextureReference A pointer to the ambient texture reference or the null pointer.
/// @method{Core_Assets_Material}
Core_Result Core_Assets_Material_setAmbientTexture(Core_Assets_Material* SELF, Core_Assets_Ref* ambientTextureReference);

#endif // CORE_ASSETS_MATERIAL_H_INCLUDED
