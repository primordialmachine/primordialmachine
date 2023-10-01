/// @file dx/assets/mesh_instance.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ASSET_MESH_INSTANCE_H_INCLUDED)
#define DX_ASSET_MESH_INSTANCE_H_INCLUDED

#include "dx/assets/mesh.h"

/// @brief A mesh instance asset.
DX_DECLARE_OBJECT_TYPE("dx.asset.mesh_instance",
                       dx_asset_mesh_instance,
                       dx_object)

static inline dx_asset_mesh_instance* DX_ASSET_MESH_INSTANCE(void* p) {
    return (dx_asset_mesh_instance*)p;
}

struct dx_asset_mesh_instance {
  dx_object _parent;
  /// @brief Reference to the mesh of this mesh instance.
  dx_asset_reference* mesh_reference;
  /// @brief The world matrix of this mesh instance.
  DX_MAT4 world_matrix;
};

static inline dx_asset_mesh_instance_dispatch* DX_ASSET_MESH_INSTANCE_DISPATCH(void* p) {
  return (dx_asset_mesh_instance_dispatch*)p;
}

struct dx_asset_mesh_instance_dispatch {
  dx_object_dispatch _parent;
};

/// @constructor{dx_asset_mesh_instance}
/// @brief Construct this dx_asset_mesh_instance object.
/// @param mesh_reference A pointer to the mesh reference of this mesh instance.
/// @error #DX_ERROR_INVALID_ARGUMENT @a mesh_reference is a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
dx_result dx_asset_mesh_instance_construct(dx_asset_mesh_instance* SELF, dx_asset_reference* mesh_reference);

/// @create-operator{dx_asset_mesh_instance}
/// @param mesh_reference A pointer to the mesh reference of this mesh instance.
/// @error #DX_ERROR_INVALID_ARGUMENT @a mesh_reference is a null pointer
dx_result dx_asset_mesh_instance_create(dx_asset_mesh_instance** RETURN, dx_asset_reference* mesh_reference);

#endif // DX_ASSET_MESH_INSTANCE_H_INCLUDED
