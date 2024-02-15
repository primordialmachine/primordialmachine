#if !defined(DX_MESH_H_INCLUDED)
#define DX_MESH_H_INCLUDED

#include "dx/assets.h"
#include "dx/val/vbinding.h"
#include "dx/val/buffer.h"
#include "dx/val/program.h"
#include "dx/val/material.h"

/// @brief The backend-representation of a mesh.
/// A dx.mesh usually represents a dx.asset.mesh.
Core_declareObjectType("dx.val.mesh",
                       dx_val_mesh,
                       Core_Object);

static inline dx_val_mesh* DX_VAL_MESH(void* p) {
  return (dx_val_mesh*)p;
}

struct dx_val_mesh {
  Core_Object _parent;
  /// @brief A pointer to the corresponding asset mesh.
  dx_assets_mesh* mesh_asset;
  /// @brief A pointer to the corresponding assset material.
  /// Usually the asset material of the asset mesh.
  dx_val_material* material;
  Core_Visuals_Context* context;
  dx_val_buffer* buffer;
  dx_val_vbinding* vbinding;
  dx_val_program* program;
};

static inline dx_val_mesh_Dispatch* DX_VAL_MESH_DISPATCH(void* p) {
  return (dx_val_mesh_Dispatch*)p;
}

struct dx_val_mesh_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_val_mesh_construct(dx_val_mesh* SELF, Core_Visuals_Context* context, dx_assets_mesh* mesh_asset);

Core_Result dx_val_mesh_create(dx_val_mesh** RETURN, Core_Visuals_Context* context, dx_assets_mesh* mesh_asset);

#endif // DX_VAL_MESH_H_INCLUDED
