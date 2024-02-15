#include "dx/assets/mesh/generators.h"

Core_Result dx_asset_mesh_append_triangle(dx_assets_mesh* mesh) {
  static Core_Size const number_of_vertices = 3;
  static DX_VEC3 const xyz[] = {
    { -0.5f, -0.5f, 0.f, },
    { +0.5f, -0.5f, 0.f, },
    { +0.0f, +0.5f, 0.f, },
  };
  static DX_VEC4 const ambient_rgba[] = {
    { 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f },
    { 1.f, 1.f, 1.f, 1.f },
  };
  static Core_InlineVector2R32 const ambient_uv[] = {
    { 0.f, 0.f, },
    { 1.f, 0.f, },
    { 0.f, 1.f, },
  };
  for (Core_Size i = 0, n = number_of_vertices; i < n; ++i) {
    if (dx_assets_mesh_append_vertex(mesh, &xyz[i], &ambient_rgba[i], &ambient_uv[i])) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

Core_Result dx_assets_mesh_append_quadriliteral(dx_assets_mesh* mesh) {
  static Core_Real32 const a = -0.5f;
  static Core_Real32 const b = +0.5f;

  DX_VEC3 p[4];
  Core_InlineVector2R32 t[4];
  DX_VEC4 c[4];

  p[0] = (DX_VEC3){ a, a, 0.f }; // left, bottom
  t[0] = (Core_InlineVector2R32){ 0.f, 0.f };
  c[0] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[1] = (DX_VEC3){ b, a, 0.f }; // right, bottom
  t[1] = (Core_InlineVector2R32){ 1.f, 0.f };
  c[1] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[2] = (DX_VEC3){ b, b, 0.f }; // right, top
  t[2] = (Core_InlineVector2R32){ 1.f, 1.f };
  c[2] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };
  p[3] = (DX_VEC3){ a, b, 0.f }; // left, top
  t[3] = (Core_InlineVector2R32){ 0.f, 1.f };
  c[3] = (DX_VEC4){ 1.f, 1.f, 1.f, 1.f };

  // triangle #1

  // left top
  if (dx_assets_mesh_append_vertex(mesh, &p[3], &c[3], &t[3])) {
    return Core_Failure;
  }

  // left bottom
  if (dx_assets_mesh_append_vertex(mesh, &p[0], &c[0], &t[0])) {
    return Core_Failure;
  }

  // right top
  if (dx_assets_mesh_append_vertex(mesh, &p[2], &c[2], &t[2])) {
    return Core_Failure;
  }

  // triangle #2

  // right top
  if (dx_assets_mesh_append_vertex(mesh, &p[2], &c[2], &t[2])) {
    return Core_Failure;
  }

  // left bottom
  if (dx_assets_mesh_append_vertex(mesh, &p[0], &c[0], &t[0])) {
    return Core_Failure;
  }

  // right bottom
  if (dx_assets_mesh_append_vertex(mesh, &p[1], &c[1], &t[1])) {
    return Core_Failure;
  }

  return Core_Success;
}

Core_Result dx_assets_mesh_on_triangle(dx_assets_mesh* mesh) {
  if (dx_assets_mesh_clear(mesh)) {
    return Core_Failure;
  }
  return dx_asset_mesh_append_triangle(mesh);
}

Core_Result dx_assets_mesh_on_cube(dx_assets_mesh* mesh) {
  if (dx_assets_mesh_clear(mesh)) {
    return Core_Failure;
  }
  DX_MAT4 t;
  dx_mat4_set_translate(&t, 0, 0, +0.5f);

  DX_MAT4 r;
  Core_Size i = 0;
  // front face
  dx_assets_mesh_append_quadriliteral(mesh);
  dx_assets_mesh_transform_range(mesh, &t, i, 6);
  i += 6;
  // back face (rotate front face counter-clockwise around the y-axis by -180 (or +180) degrees)
  dx_mat4_set_rotate_y(&r, 180);
  dx_assets_mesh_append_quadriliteral(mesh);
  dx_assets_mesh_transform_range(mesh, &t, i, 6);
  dx_assets_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // left face (rotate front face counter-clockwise around the y-axis by -90 degrees)
  dx_mat4_set_rotate_y(&r, -90);
  dx_assets_mesh_append_quadriliteral(mesh);
  dx_assets_mesh_transform_range(mesh, &t, i, 6);
  dx_assets_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // right face (rotate front face counter-clockwise around the y-axis by +90 degrees)
  dx_mat4_set_rotate_y(&r, +90);
  dx_assets_mesh_append_quadriliteral(mesh);
  dx_assets_mesh_transform_range(mesh, &t, i, 6);
  dx_assets_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // bottom face (rotate front face counter-clockwise around the x-axis by -90 degrees)
  dx_mat4_set_rotate_x(&r, -90);
  dx_assets_mesh_append_quadriliteral(mesh);
  dx_assets_mesh_transform_range(mesh, &t, i, 6);
  dx_assets_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  // top face (rotate front face counter-clockwise around the x-axis by 90 degrees)
  dx_mat4_set_rotate_x(&r, +90);
  dx_assets_mesh_append_quadriliteral(mesh);
  dx_assets_mesh_transform_range(mesh, &t, i, 6);
  dx_assets_mesh_transform_range(mesh, &r, i, 6);
  i += 6;
  //
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_assets_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  //
  return Core_Success;
}

Core_Result dx_assets_mesh_on_empty(dx_assets_mesh* mesh) {
  if (dx_assets_mesh_clear(mesh)) {
    return Core_Failure;
  }
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_assets_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  return Core_Success;
}

Core_Result dx_assets_mesh_on_quadriliteral(dx_assets_mesh* mesh) {
  if (dx_assets_mesh_clear(mesh)) {
    return Core_Failure;
  }
  if (dx_assets_mesh_append_quadriliteral(mesh)) {
    return Core_Failure;
  }
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_assets_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  return Core_Success;
}

Core_Result dx_assets_mesh_on_octahedron(dx_assets_mesh* mesh) {
  if (dx_assets_mesh_clear(mesh)) {
    return Core_Failure;
  }
  DX_MAT4 t;

  DX_MAT4 r;
  Core_Size i = 0;

  static DX_VEC4 const WHITE = { 1.f, 1.f, 1.f, 1.f };

  static DX_VEC3 const top = { 0.f, +0.5f, 0.f };
  static Core_InlineVector2R32 const top_uv = { 0.5f, 1.f };

  static DX_VEC3 const bottom = { 0.f, -0.5f, 0.f };
  static Core_InlineVector2R32 const bottom_uv = { 0.5f, 0.f };

  static DX_VEC3 const left = { -0.5f, 0.f, 0.f };
  static Core_InlineVector2R32 const left_uv = { 0.f, 0.5f };

  static DX_VEC3 const right = { +0.5f, 0.f, 0.f };
  static DX_VEC3 const right_uv = { 1.f, 0.5f };

  static DX_VEC3 const front = { 0.f, 0.f, +0.5f };
  static DX_VEC3 const back = { 0.f, 0.f, -0.5f };

  DX_VEC4 rgba = { 1, 1, 1, 1 };
  
  // left front top
  {
    DX_VEC3 xyz = { -1.f, 0, 0 };
    Core_InlineVector2R32 uv = { 0, 0 };
    dx_assets_mesh_append_vertex(mesh, &xyz, &rgba, &uv);
  }
  {
    DX_VEC3 xyz = { 0, 0, 1.f };
    Core_InlineVector2R32 uv = { 1, 0 };
    dx_assets_mesh_append_vertex(mesh, &xyz, &rgba, &uv);
  }
  {
    DX_VEC3 xyz = { 0, 1.f, 0 };
    Core_InlineVector2R32 uv = { 0.5, 1 };
    dx_assets_mesh_append_vertex(mesh, &xyz, &rgba, &uv);
  }
  // right front
  {
    dx_assets_mesh_append_range(mesh, 0, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    dx_assets_mesh_transform_range(mesh, &r, 3, 3);
  }
  // right back
  {
    dx_assets_mesh_append_range(mesh, 3, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    dx_assets_mesh_transform_range(mesh, &r, 6, 3);
  }
  // left back
  {
    dx_assets_mesh_append_range(mesh, 6, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    dx_assets_mesh_transform_range(mesh, &r, 9, 3);
  }
  // bottom
  {
    dx_assets_mesh_append_range(mesh, 0, 12);
    DX_MAT4 r;
    dx_mat4_set_rotate_x(&r, -180);
    dx_assets_mesh_transform_range(mesh, &r, 12, 12);
  }
  //
  DX_VEC4 const COLOR = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  dx_assets_mesh_set_mesh_ambient_rgba(mesh, &COLOR);
  //
  return Core_Success;
}
