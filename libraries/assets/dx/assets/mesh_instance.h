/// @file dx/assets/mesh_instance.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ASSET_MESH_INSTANCE_H_INCLUDED)
#define DX_ASSET_MESH_INSTANCE_H_INCLUDED

#include "dx/assets/matrix_4x4_f32.h"
#include "dx/assets/mesh.h"

/// @brief A mesh instance asset.
Core_declareObjectType("dx.asset.mesh_instance",
                       dx_asset_mesh_instance,
                       Core_Object);

static inline dx_asset_mesh_instance* DX_ASSET_MESH_INSTANCE(void* p) {
    return (dx_asset_mesh_instance*)p;
}

struct dx_asset_mesh_instance {
  Core_Object _parent;
  /// @brief Reference to the mesh of this mesh instance.
  dx_asset_reference* mesh_reference;
  /// @brief The world matrix of this mesh instance.
  dx_assets_matrix_4x4_f32* world_matrix;
};

static inline dx_asset_mesh_instance_Dispatch* DX_ASSET_MESH_INSTANCE_DISPATCH(void* p) {
  return (dx_asset_mesh_instance_Dispatch*)p;
}

struct dx_asset_mesh_instance_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @constructor{dx_asset_mesh_instance}
/// @brief Construct this dx_asset_mesh_instance object.
/// @param mesh_reference A pointer to the mesh reference of this mesh instance.
/// @error #Core_Error_ArgumentInvalid @a mesh_reference is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
Core_Result dx_asset_mesh_instance_construct(dx_asset_mesh_instance* SELF, dx_asset_reference* mesh_reference);

/// @create-operator{dx_asset_mesh_instance}
/// @param mesh_reference A pointer to the mesh reference of this mesh instance.
/// @error #Core_Error_ArgumentInvalid @a mesh_reference is a null pointer
Core_Result dx_asset_mesh_instance_create(dx_asset_mesh_instance** RETURN, dx_asset_reference* mesh_reference);

#endif // DX_ASSET_MESH_INSTANCE_H_INCLUDED
