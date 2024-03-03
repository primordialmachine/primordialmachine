#include "dx/assets/mesh/generators.h"

Core_Result Core_Assets_Mesh_appendTriangle(Core_Assets_Mesh* mesh) {
  static Core_Size const number_of_vertices = 3;
  static DX_VEC3 const xyz[] = {
    { -0.5f, -0.5f, 0.f, },
    { +0.5f, -0.5f, 0.f, },
    { +0.0f, +0.5f, 0.f, },
  };
  static Core_InlineRgbaR32 const ambient_rgba[] = {
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
    if (Core_Assets_Mesh_appendVertex(mesh, &xyz[i], &ambient_rgba[i], &ambient_uv[i])) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

Core_Result Core_Assets_Mesh_appendQuadriliteral(Core_Assets_Mesh* mesh) {
  static Core_Real32 const a = -0.5f;
  static Core_Real32 const b = +0.5f;

  DX_VEC3 p[4];
  Core_InlineVector2R32 t[4];
  Core_InlineRgbaR32 c[4];

  p[0] = (DX_VEC3){ a, a, 0.f }; // left, bottom
  t[0] = (Core_InlineVector2R32){ 0.f, 0.f };
  c[0] = (Core_InlineRgbaR32){ 1.f, 1.f, 1.f, 1.f };
  p[1] = (DX_VEC3){ b, a, 0.f }; // right, bottom
  t[1] = (Core_InlineVector2R32){ 1.f, 0.f };
  c[1] = (Core_InlineRgbaR32){ 1.f, 1.f, 1.f, 1.f };
  p[2] = (DX_VEC3){ b, b, 0.f }; // right, top
  t[2] = (Core_InlineVector2R32){ 1.f, 1.f };
  c[2] = (Core_InlineRgbaR32){ 1.f, 1.f, 1.f, 1.f };
  p[3] = (DX_VEC3){ a, b, 0.f }; // left, top
  t[3] = (Core_InlineVector2R32){ 0.f, 1.f };
  c[3] = (Core_InlineRgbaR32){ 1.f, 1.f, 1.f, 1.f };

  // triangle #1

  // left top
  if (Core_Assets_Mesh_appendVertex(mesh, &p[3], &c[3], &t[3])) {
    return Core_Failure;
  }

  // left bottom
  if (Core_Assets_Mesh_appendVertex(mesh, &p[0], &c[0], &t[0])) {
    return Core_Failure;
  }

  // right top
  if (Core_Assets_Mesh_appendVertex(mesh, &p[2], &c[2], &t[2])) {
    return Core_Failure;
  }

  // triangle #2

  // right top
  if (Core_Assets_Mesh_appendVertex(mesh, &p[2], &c[2], &t[2])) {
    return Core_Failure;
  }

  // left bottom
  if (Core_Assets_Mesh_appendVertex(mesh, &p[0], &c[0], &t[0])) {
    return Core_Failure;
  }

  // right bottom
  if (Core_Assets_Mesh_appendVertex(mesh, &p[1], &c[1], &t[1])) {
    return Core_Failure;
  }

  return Core_Success;
}

Core_Result Core_Assets_Mesh_onTriangle(Core_Assets_Mesh* mesh) {
  if (Core_Assets_Mesh_clear(mesh)) {
    return Core_Failure;
  }
  return Core_Assets_Mesh_appendTriangle(mesh);
}

Core_Result Core_Assets_Mesh_onCube(Core_Assets_Mesh* mesh) {
  if (Core_Assets_Mesh_clear(mesh)) {
    return Core_Failure;
  }
  DX_MAT4 t;
  dx_mat4_set_translate(&t, 0, 0, +0.5f);

  DX_MAT4 r;
  Core_Size i = 0;
  // front face
  Core_Assets_Mesh_appendQuadriliteral(mesh);
  Core_Assets_Mesh_transformRange(mesh, &t, i, 6);
  i += 6;
  // back face (rotate front face counter-clockwise around the y-axis by -180 (or +180) degrees)
  dx_mat4_set_rotate_y(&r, 180);
  Core_Assets_Mesh_appendQuadriliteral(mesh);
  Core_Assets_Mesh_transformRange(mesh, &t, i, 6);
  Core_Assets_Mesh_transformRange(mesh, &r, i, 6);
  i += 6;
  // left face (rotate front face counter-clockwise around the y-axis by -90 degrees)
  dx_mat4_set_rotate_y(&r, -90);
  Core_Assets_Mesh_appendQuadriliteral(mesh);
  Core_Assets_Mesh_transformRange(mesh, &t, i, 6);
  Core_Assets_Mesh_transformRange(mesh, &r, i, 6);
  i += 6;
  // right face (rotate front face counter-clockwise around the y-axis by +90 degrees)
  dx_mat4_set_rotate_y(&r, +90);
  Core_Assets_Mesh_appendQuadriliteral(mesh);
  Core_Assets_Mesh_transformRange(mesh, &t, i, 6);
  Core_Assets_Mesh_transformRange(mesh, &r, i, 6);
  i += 6;
  // bottom face (rotate front face counter-clockwise around the x-axis by -90 degrees)
  dx_mat4_set_rotate_x(&r, -90);
  Core_Assets_Mesh_appendQuadriliteral(mesh);
  Core_Assets_Mesh_transformRange(mesh, &t, i, 6);
  Core_Assets_Mesh_transformRange(mesh, &r, i, 6);
  i += 6;
  // top face (rotate front face counter-clockwise around the x-axis by 90 degrees)
  dx_mat4_set_rotate_x(&r, +90);
  Core_Assets_Mesh_appendQuadriliteral(mesh);
  Core_Assets_Mesh_transformRange(mesh, &t, i, 6);
  Core_Assets_Mesh_transformRange(mesh, &r, i, 6);
  i += 6;
  //
  Core_InlineRgbaR32 const COLOR = (Core_InlineRgbaR32){ .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
  Core_Assets_Mesh_setMeshAmbientRgba(mesh, &COLOR);
  //
  return Core_Success;
}

Core_Result Core_Assets_Mesh_onEmpty(Core_Assets_Mesh* mesh) {
  if (Core_Assets_Mesh_clear(mesh)) {
    return Core_Failure;
  }
  Core_InlineRgbaR32 const COLOR = (Core_InlineRgbaR32){ 0.f, 0.f, 0.f, 0.f };
  Core_Assets_Mesh_setMeshAmbientRgba(mesh, &COLOR);
  return Core_Success;
}

Core_Result Core_Assets_Mesh_onQuadriliteral(Core_Assets_Mesh* mesh) {
  if (Core_Assets_Mesh_clear(mesh)) {
    return Core_Failure;
  }
  if (Core_Assets_Mesh_appendQuadriliteral(mesh)) {
    return Core_Failure;
  }
  Core_InlineRgbaR32 const COLOR = (Core_InlineRgbaR32){ 0.f, 0.f, 0.f, 0.f };
  Core_Assets_Mesh_setMeshAmbientRgba(mesh, &COLOR);
  return Core_Success;
}

Core_Result Core_Assets_Mesh_onOctahedron(Core_Assets_Mesh* mesh) {
  if (Core_Assets_Mesh_clear(mesh)) {
    return Core_Failure;
  }
  DX_MAT4 t;

  DX_MAT4 r;
  Core_Size i = 0;

  static Core_InlineRgbaR32 const WHITE = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f };

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

  Core_InlineRgbaR32 rgba = WHITE;
  
  // left front top
  {
    DX_VEC3 xyz = { -1.f, 0, 0 };
    Core_InlineVector2R32 uv = { 0, 0 };
    Core_Assets_Mesh_appendVertex(mesh, &xyz, &rgba, &uv);
  }
  {
    DX_VEC3 xyz = { 0, 0, 1.f };
    Core_InlineVector2R32 uv = { 1, 0 };
    Core_Assets_Mesh_appendVertex(mesh, &xyz, &rgba, &uv);
  }
  {
    DX_VEC3 xyz = { 0, 1.f, 0 };
    Core_InlineVector2R32 uv = { 0.5, 1 };
    Core_Assets_Mesh_appendVertex(mesh, &xyz, &rgba, &uv);
  }
  // right front
  {
    Core_Assets_Mesh_appendRange(mesh, 0, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    Core_Assets_Mesh_transformRange(mesh, &r, 3, 3);
  }
  // right back
  {
    Core_Assets_Mesh_appendRange(mesh, 3, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    Core_Assets_Mesh_transformRange(mesh, &r, 6, 3);
  }
  // left back
  {
    Core_Assets_Mesh_appendRange(mesh, 6, 3);
    DX_MAT4 r;
    dx_mat4_set_rotate_y(&r, 90);
    Core_Assets_Mesh_transformRange(mesh, &r, 9, 3);
  }
  // bottom
  {
    Core_Assets_Mesh_appendRange(mesh, 0, 12);
    DX_MAT4 r;
    dx_mat4_set_rotate_x(&r, -180);
    Core_Assets_Mesh_transformRange(mesh, &r, 12, 12);
  }
  //
  Core_InlineRgbaR32 const COLOR = (Core_InlineRgbaR32){ .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
  Core_Assets_Mesh_setMeshAmbientRgba(mesh, &COLOR);
  //
  return Core_Success;
}
