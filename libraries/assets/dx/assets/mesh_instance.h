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

/// @brief Construct this dx_asset_mesh_instance object.
/// @param SELF A pointer to this dx_asset_mesh_instance object.
/// @param mesh_reference A pointer to the mesh reference of this mesh instance.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a mesh_reference is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
int dx_asset_mesh_instance_construct(dx_asset_mesh_instance* SELF, dx_asset_reference* mesh_reference);

/// @brief Create a dx_asset_mesh_instance object.
/// @param mesh_reference A pointer to the mesh reference of this mesh instance.
/// @return A pointer to the dx_asset_mesh_instance object on success. The null pointer failure.
/// @default-failure
/// - #DX_INVALID_ARGUMENT @a mesh_reference is a null pointer
/// - #DX_ALLOCATION_FAILED an allocation failed
dx_asset_mesh_instance* dx_asset_mesh_instance_create(dx_asset_reference* mesh_reference);

#endif // DX_ASSET_MESH_INSTANCE_H_INCLUDED
