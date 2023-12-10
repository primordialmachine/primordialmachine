#include "dx/assets/mesh_operations/set_vertex_colors.h"

DX_DEFINE_OBJECT_TYPE("dx.assets.mesh_operations.set_vertex_colors",
                      dx_assets_mesh_operations_set_vertex_colors,
                      dx_asset_mesh_operation);

static const DX_VEC4 colors[] = {
  { 1.f, 0.f, 0.f, 1.f },
  { 0.f, 1.f, 0.f, 1.f },
  { 0.f, 0.f, 1.f, 1.f },
  //
  { 1.f, 1.f, 0.f, 1.f },
  { 0.f, 1.f, 1.f, 1.f },
  { 1.f, 0.f, 1.f, 1.f },
};

static DX_VEC4 const* get_color(Core_Size index) {
  return &(colors[index % 6]);
}

static Core_Result apply(dx_assets_mesh_operations_set_vertex_colors* SELF, dx_assets_mesh* mesh) {
  for (Core_Size i = 0, n = mesh->number_of_vertices; i < n; ++i) {
    mesh->vertices.ambient_rgba[i] = *get_color(i);
  }
  return Core_Success;
}

Core_Result dx_assets_mesh_operations_set_vertex_colors_construct(dx_assets_mesh_operations_set_vertex_colors* SELF) {
  DX_CONSTRUCT_PREFIX(dx_assets_mesh_operations_set_vertex_colors);
  if (dx_asset_mesh_operation_construct(DX_ASSET_MESH_OPERATION(SELF), dx_asset_mesh_operation_kind_set_vertex_colors)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_assets_mesh_operations_set_vertex_colors_destruct(dx_assets_mesh_operations_set_vertex_colors* SELF)
{/*Intentionally empty.*/}

static void dx_assets_mesh_operations_set_vertex_colors_constructDispatch(dx_assets_mesh_operations_set_vertex_colors_dispatch* SELF) {
  DX_ASSET_MESH_OPERATION_DISPATCH(SELF)->apply = (Core_Result(*)(dx_asset_mesh_operation*, dx_assets_mesh*)) & apply;
}

Core_Result dx_assets_mesh_operations_set_vertex_colors_create(dx_assets_mesh_operations_set_vertex_colors** RETURN) {
  DX_CREATE_PREFIX(dx_assets_mesh_operations_set_vertex_colors);
  if (dx_assets_mesh_operations_set_vertex_colors_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;  
}
