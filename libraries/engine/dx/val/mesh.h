#if !defined(DX_MESH_H_INCLUDED)
#define DX_MESH_H_INCLUDED

#include "dx/assets.h"
#include "dx/val/vbinding.h"
#include "dx/val/buffer.h"
#include "dx/val/program.h"
#include "dx/val/material.h"

/// @brief The backend-representation of a mesh.
/// A dx.mesh usually represents a dx.asset.mesh.
DX_DECLARE_OBJECT_TYPE("dx.val.mesh",
                       dx_val_mesh,
                       dx_object)

static inline dx_val_mesh* DX_VAL_MESH(void* p) {
  return (dx_val_mesh*)p;
}

struct dx_val_mesh {
  dx_object _parent;
  /// @brief A pointer to the corresponding asset mesh.
  dx_asset_mesh* asset_mesh;
  /// @brief A pointer to the corresponding assset material.
  /// Usually the asset material of the asset mesh.
  dx_val_material* material;
  dx_val_context* context;
  dx_val_buffer* buffer;
  dx_val_vbinding* vbinding;
  dx_val_program* program;
};

static inline dx_val_mesh_dispatch* DX_VAL_MESH_DISPATCH(void* p) {
  return (dx_val_mesh_dispatch*)p;
}

struct dx_val_mesh_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_val_mesh_construct(dx_val_mesh* self, dx_val_context* context, dx_asset_mesh* asset_mesh);

dx_val_mesh* dx_val_mesh_create(dx_val_context* context, dx_asset_mesh* asset_mesh);

#endif // DX_VAL_MESH_H_INCLUDED
