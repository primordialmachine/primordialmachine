#if !defined(DX_ASSETS_MESH_OPERATIONS_SET_VERTEX_COLORS_H_INCLUDED)
#define DX_ASSETS_MESH_OPERATIONS_SET_VERTEX_COLORS_H_INCLUDED

#include "dx/assets/mesh_operation.h"

Core_declareObjectType("dx.assets.mesh_operations.set_vertex_colors",
                       dx_assets_mesh_operations_set_vertex_colors,
                       dx_asset_mesh_operation);

static inline dx_assets_mesh_operations_set_vertex_colors* DX_ASSETS_MESH_OPERATIONS_SET_VERTEX_COLORS(void* p) {
  return (dx_assets_mesh_operations_set_vertex_colors*)p;
}

struct dx_assets_mesh_operations_set_vertex_colors {
  dx_asset_mesh_operation _parent;
};

static inline dx_assets_mesh_operations_set_vertex_colors_Dispatch* DX_ASSET_MESH_OPERATIONS_SET_VERTEX_COLORS_DISPATCH(void* p) {
  return (dx_assets_mesh_operations_set_vertex_colors_Dispatch*)p;
}

struct dx_assets_mesh_operations_set_vertex_colors_Dispatch {
  dx_asset_mesh_operation_Dispatch _parent;
};

Core_Result dx_assets_mesh_operations_set_vertex_colors_construct(dx_assets_mesh_operations_set_vertex_colors* SELF);

Core_Result dx_assets_mesh_operations_set_vertex_colors_create(dx_assets_mesh_operations_set_vertex_colors** RETURN);

#endif // DX_ASSETS_MESH_OPERATIONS_SET_VERTEX_COLORS_H_INCLUDED
