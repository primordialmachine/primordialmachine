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

static DX_VEC4 const* get_color(dx_size index) {
  return &(colors[index % 6]);
}

static dx_result apply(dx_assets_mesh_operations_set_vertex_colors* SELF, dx_assets_mesh* mesh) {
  for (dx_size i = 0, n = mesh->number_of_vertices; i < n; ++i) {
    mesh->vertices.ambient_rgba[i] = *get_color(i);
  }
  return DX_SUCCESS;
}

dx_result dx_assets_mesh_operations_set_vertex_colors_construct(dx_assets_mesh_operations_set_vertex_colors* SELF) {
  dx_rti_type* TYPE = dx_assets_mesh_operations_set_vertex_colors_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_asset_mesh_operation_construct(DX_ASSET_MESH_OPERATION(SELF), dx_asset_mesh_operation_kind_set_vertex_colors)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_assets_mesh_operations_set_vertex_colors_destruct(dx_assets_mesh_operations_set_vertex_colors* SELF)
{/*Intentionally empty.*/}

static void dx_assets_mesh_operations_set_vertex_colors_dispatch_construct(dx_assets_mesh_operations_set_vertex_colors_dispatch* SELF) {
  DX_ASSET_MESH_OPERATION_DISPATCH(SELF)->apply = (dx_result(*)(dx_asset_mesh_operation*, dx_assets_mesh*)) & apply;
}

dx_result dx_assets_mesh_operations_set_vertex_colors_create(dx_assets_mesh_operations_set_vertex_colors** RETURN) {
  DX_CREATE_PREFIX(dx_assets_mesh_operations_set_vertex_colors)
  if (dx_assets_mesh_operations_set_vertex_colors_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;  
}
