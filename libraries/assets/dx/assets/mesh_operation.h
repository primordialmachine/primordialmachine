#if !defined(DX_ASSETS_MESH_OPERATION_H_INCLUDED)
#define DX_ASSETS_MESH_OPERATION_H_INCLUDED

#include "dx/assets/mesh.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_ENUMERATION_TYPE("dx.asset.mesh_operation_kind",
                            dx_asset_mesh_operation_kind);

enum dx_asset_mesh_operation_kind {
  /// @brief Set the vertex colors of a mesh.
  dx_asset_mesh_operation_kind_set_vertex_colors,
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.asset.mesh_operation",
                       dx_asset_mesh_operation,
                       Core_Object);

static inline dx_asset_mesh_operation* DX_ASSET_MESH_OPERATION(void* p) {
  return (dx_asset_mesh_operation*)p;
}

struct dx_asset_mesh_operation {
  Core_Object _parent;
  dx_asset_mesh_operation_kind kind;
};

static inline dx_asset_mesh_operation_dispatch* DX_ASSET_MESH_OPERATION_DISPATCH(void* p) {
  return (dx_asset_mesh_operation_dispatch*)p;
}

struct dx_asset_mesh_operation_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*apply)(dx_asset_mesh_operation* SELF, dx_assets_mesh* mesh);
};

Core_Result dx_asset_mesh_operation_construct(dx_asset_mesh_operation* SELF, dx_asset_mesh_operation_kind kind);

static inline Core_Result dx_asset_mesh_operation_apply(dx_asset_mesh_operation* SELF, dx_assets_mesh* mesh) {
  DX_OBJECT_VIRTUALCALL(dx_asset_mesh_operation, apply, SELF, mesh);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_ASSETS_MESH_OPERATION_H_INCLUDED
