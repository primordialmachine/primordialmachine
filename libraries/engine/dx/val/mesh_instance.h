#if !defined(DX_VAL_MESH_INSTANCE_H_INCLUDED)
#define DX_VAL_MESH_INSTANCE_H_INCLUDED

#include "dx/val/material.h"
#include "dx/val/mesh.h"

/// @brief The instance of a mesh in world space.
DX_DECLARE_OBJECT_TYPE("dx.val.mesh_instance",
                       dx_val_mesh_instance,
                       Core_Object);

static dx_val_mesh_instance* DX_VAL_MESH_INSTANCE(void* p) {
  return (dx_val_mesh_instance*)p;
}

struct dx_val_mesh_instance {
  Core_Object _parent;
  /// @brief The mesh of this mesh instance.
  dx_val_mesh* mesh;
  /// @brief The world matrix of this mesh instance.
  DX_MAT4 world_matrix;
  /// @brief The commands to render this mesh instance.
  dx_val_command_list* commands;
};

static dx_val_mesh_instance_dispatch* DX_VAL_MESH_INSTANCE_DISPATCH(void* p) {
  return (dx_val_mesh_instance_dispatch*)p;
}

struct dx_val_mesh_instance_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_val_mesh_instance_construct(dx_val_mesh_instance* SELF, DX_MAT4 world_matrix, dx_val_mesh* mesh);

Core_Result dx_val_mesh_instance_create(dx_val_mesh_instance** RETURN, DX_MAT4 world_matrix, dx_val_mesh* mesh);

/// @brief Get the constant binding of this mesh instance.
/// @param SELF This mesh instance.
/// @return The constant binding of this mesh instance on success. The null pointer on failure.
dx_val_cbinding* dx_val_mesh_instance_get_cbinding(dx_val_mesh_instance* SELF);

/// @brief Update the specified constant binding with the constant binding of this mesh instance.
/// @param SELF This mesh instance.
/// @param cbinding The constant binding.
/// @return The zero value on success. A non-zero value on failure.
Core_Result dx_val_mesh_instance_update_cbinding(dx_val_mesh_instance* SELF, dx_val_cbinding* cbinding);

#endif // DX_VAL_MESH_INSTANCE_H_INCLUDED
