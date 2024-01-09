#include "dx/assets/mesh.h"

#include "dx/assets/mesh/generators.h"

// memcpy
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.assets.face",
                      dx_assets_face,
                      Core_Object);

static void dx_assets_face_destruct(dx_assets_face* SELF) {
  if (SELF->indices) {
    Core_Memory_deallocate(SELF->indices);
    SELF->indices = NULL;
  }
  SELF->number_of_indices = 0;
}

static void dx_assets_face_constructDispatch(dx_assets_face_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_assets_face_construct_triangle(dx_assets_face* SELF, uint32_t a, uint32_t b, uint32_t c) {
  DX_CONSTRUCT_PREFIX(dx_assets_face);
  SELF->number_of_indices = 3;
  if (Core_Memory_allocate(&SELF->indices, sizeof(uint32_t) * 3)) {
    return Core_Failure;
  }
  SELF->indices[0] = a;
  SELF->indices[1] = b;
  SELF->indices[2] = c;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_face_create_triangle(dx_assets_face** RETURN, uint32_t a, uint32_t b, uint32_t c) {
  DX_CREATE_PREFIX(dx_assets_face);
  if (dx_assets_face_construct_triangle(SELF, a, b, c)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_face_construct_quadriliteral(dx_assets_face* SELF, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  DX_CONSTRUCT_PREFIX(dx_assets_face);
  SELF->number_of_indices = 4;
  if (Core_Memory_allocate(&SELF->indices, sizeof(uint32_t) * 4)) {
    return Core_Failure;
  }
  SELF->indices[0] = a;
  SELF->indices[1] = b;
  SELF->indices[2] = c;
  SELF->indices[3] = d;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_face_create_quadriliteral(dx_assets_face** RETURN, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  DX_CREATE_PREFIX(dx_assets_face);
  if (dx_assets_face_construct_quadriliteral(SELF, a, b, c, d)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_face_construct_copy(dx_assets_face* SELF, dx_assets_face* other) {
  DX_CONSTRUCT_PREFIX(dx_assets_face);
  if (SELF == other) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->number_of_indices = other->number_of_indices;
  if (Core_Memory_allocate(&SELF->indices, sizeof(uint32_t) * SELF->number_of_indices)) {
    return Core_Failure;
  }
  Core_Memory_copy(SELF->indices, other->indices, sizeof(uint32_t) * SELF->number_of_indices);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_face_create_copy(dx_assets_face** RETURN, dx_assets_face* other) {
  DX_CREATE_PREFIX(dx_assets_face);
  if (dx_assets_face_construct_copy(SELF, other)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_face_to_triangles(dx_assets_face* SELF, dx_inline_object_array* faces) {
  if (!SELF || !SELF->number_of_indices < 3) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (SELF->number_of_indices == 3) {
    dx_assets_face* a = NULL;
    if (dx_assets_face_create_copy(&a, SELF)) {
      return Core_Failure;
    }
    if (dx_inline_object_array_append(faces, CORE_OBJECT(a))) {
      CORE_UNREFERENCE(a);
      a = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(a);
    a = NULL;
    return 0;
  } else if (SELF->number_of_indices == 4) {
    dx_assets_face* a;
    //
    if (dx_assets_face_create_triangle(&a, SELF->indices[0], SELF->indices[1], SELF->indices[3])) {
      return Core_Failure;
    }
    if (dx_inline_object_array_append(faces, CORE_OBJECT(a))) {
      CORE_UNREFERENCE(a);
      a = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(a);
    a = NULL;
    //
    if (dx_assets_face_create_triangle(&a, SELF->indices[3], SELF->indices[2], SELF->indices[0])) {
      return Core_Failure;
    }
    if (dx_inline_object_array_append(faces, CORE_OBJECT(a))) {
      CORE_UNREFERENCE(a);
      a = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(a);
    a = NULL;
    return Core_Success;
  } else {
    Core_setError(Core_Error_NotImplemented);
    return Core_Failure;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.assets.mesh",
                      dx_assets_mesh,
                      Core_Object);

static Core_Result resize_vertex_arrays(dx_assets_mesh* SELF, Core_Boolean shrink, Core_Size number_of_vertices) {
  if (SELF->number_of_vertices == number_of_vertices) {
    // if the number of vertices in the mesh is equal to the required number of vertices, return success.
    return Core_Success;
  }
  if (SELF->number_of_vertices > number_of_vertices && !shrink) {
    // if the number of vertices in the mesh is greater than the required and shrinking is not desired, return success.
    return Core_Success;
  }
  // two cases:
  // - the number of vertices in the mesh is smaller than the required number of vertices or
  // - the number of vertices in the mesh is greater than the deisred number of vertices and shrinking is desired

  Core_Size n;
  Core_minSz(&n, SELF->number_of_vertices, number_of_vertices); // cannot fail
  
  // (1)
  DX_VEC3* xyz = NULL;
  if (Core_Memory_allocate(&xyz, number_of_vertices * sizeof(DX_VEC3))) {
    return Core_Failure;
  }

  Core_Memory_copy(xyz, SELF->vertices.xyz, n * sizeof(DX_VEC3));

  // (2)
  DX_VEC4* ambient_rgba = NULL;
  if (Core_Memory_allocate(&ambient_rgba, number_of_vertices * sizeof(DX_VEC4))) {
    Core_Memory_deallocate(xyz);
    xyz = NULL;
    return Core_Failure;
  }
  Core_Memory_copy(ambient_rgba, SELF->vertices.ambient_rgba, n * sizeof(DX_VEC4));

  // (3)
  DX_VEC2_F32* ambient_uv = NULL;
  if (Core_Memory_allocate(&ambient_uv, number_of_vertices * sizeof(DX_VEC2_F32))) {
    Core_Memory_deallocate(xyz);
    xyz = NULL;
    Core_Memory_deallocate(ambient_rgba);
    ambient_rgba = NULL;
    return Core_Failure;
  }
  Core_Memory_copy(ambient_uv, SELF->vertices.ambient_uv, n * sizeof(DX_VEC2_F32));

  SELF->vertices.xyz = xyz;
  SELF->vertices.ambient_rgba = ambient_rgba;
  SELF->vertices.ambient_uv = ambient_uv;
  SELF->number_of_vertices = number_of_vertices;

  return Core_Success;
}

static void dx_assets_mesh_destruct(dx_assets_mesh* SELF) {
  dx_inline_object_array_uninitialize(&SELF->operations);
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  if (SELF->material_reference) {
    CORE_UNREFERENCE(SELF->material_reference);
    SELF->material_reference = NULL;
  }
  if (SELF->vertices.ambient_uv) {
    Core_Memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
  }
  if (SELF->vertices.ambient_rgba) {
    Core_Memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
  }
  if (SELF->vertices.xyz) {
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
  }
}

static void dx_assets_mesh_constructDispatch(dx_assets_mesh_Dispatch* SELF)
{/*Intentionally empty.*/}

static Core_Result dx_assets_mesh_construct(dx_assets_mesh* SELF, Core_String* name, Core_String* specifier, Core_VertexFormat vertex_format, dx_asset_reference* material_reference) {
  DX_CONSTRUCT_PREFIX(dx_assets_mesh);
  if (!name || !specifier || !material_reference) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // "default" mesh
  if (Core_Memory_allocate(&SELF->vertices.xyz, 0)) {
    return Core_Failure;
  }

  if (Core_Memory_allocate(&SELF->vertices.ambient_rgba, 0)) {
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }

  if (Core_Memory_allocate(&SELF->vertices.ambient_uv, 0)) {
    Core_Memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }
  SELF->mesh.ambient_rgba = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  SELF->number_of_vertices = 0;
  
  Core_Result (*generator)(dx_assets_mesh*)  = NULL;

#define SELECT_GENERATOR(name) \
  { \
    Core_String* temporary = NULL; \
    if (Core_String_create(&temporary, #name, strlen(#name))) { \
      Core_Memory_deallocate(SELF->vertices.ambient_uv); \
      SELF->vertices.ambient_uv = NULL; \
      Core_Memory_deallocate(SELF->vertices.ambient_rgba); \
      SELF->vertices.ambient_rgba = NULL; \
      Core_Memory_deallocate(SELF->vertices.xyz); \
      SELF->vertices.xyz = NULL; \
      return Core_Failure; \
    } \
    Core_Boolean isEqualTo = Core_False; \
    if (Core_String_isEqualTo(&isEqualTo, specifier, temporary)) { \
      CORE_UNREFERENCE(temporary); \
      temporary = NULL; \
      Core_Memory_deallocate(SELF->vertices.ambient_uv); \
      SELF->vertices.ambient_uv = NULL; \
      Core_Memory_deallocate(SELF->vertices.ambient_rgba); \
      SELF->vertices.ambient_rgba = NULL; \
      Core_Memory_deallocate(SELF->vertices.xyz); \
      SELF->vertices.xyz = NULL; \
      return Core_Failure; \
    } \
    if (isEqualTo) { \
      generator = &dx_assets_mesh_on_##name; \
    } \
    CORE_UNREFERENCE(temporary); \
    temporary = NULL; \
  }

SELECT_GENERATOR(cube)
SELECT_GENERATOR(empty)
SELECT_GENERATOR(quadriliteral)
SELECT_GENERATOR(triangle)
SELECT_GENERATOR(octahedron)

#undef SELECT_GENERATOR
  
  if (!generator) {
    Core_setError(Core_Error_ArgumentInvalid);
    Core_Memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }

  if ((*generator)(SELF)) {
    Core_Memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }
  if (!material_reference) {
    Core_Memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }
  SELF->material_reference = material_reference;
  CORE_REFERENCE(SELF->material_reference);

  SELF->name = name;
  CORE_REFERENCE(name);

  SELF->vertex_format = vertex_format;

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    CORE_UNREFERENCE(SELF->material_reference);
    SELF->material_reference = NULL;
    Core_Memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_assets_mesh_create(dx_assets_mesh** RETURN, Core_String * name, Core_String* specifier, Core_VertexFormat vertex_format, dx_asset_reference* material_reference) {
  DX_CREATE_PREFIX(dx_assets_mesh);
  if (dx_assets_mesh_construct(SELF, name, specifier, vertex_format, material_reference)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_assets_mesh_format(dx_assets_mesh* SELF, Core_VertexFormat vertex_format, void** bytes, Core_Size* number_of_bytes) {
  switch (vertex_format) {
  case Core_VertexFormat_position_xyz: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->number_of_vertices * sizeof(DX_VEC3))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC3));
    return Core_Success;
  } break;
  case Core_VertexFormat_ambient_rgba: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->number_of_vertices * sizeof(DX_VEC4))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC4*)q) = SELF->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC4));
    return Core_Success;
  } break;
  case Core_VertexFormat_position_xyz_ambient_rgba: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4)))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC4*)q) = SELF->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4));
    return Core_Success;
  } break;
  case Core_VertexFormat_position_xyz_ambient_uv: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2_F32)))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC2_F32*)q) = SELF->vertices.ambient_uv[i];
      q += sizeof(DX_VEC2_F32);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2_F32));
    return Core_Success;
  } break;
  default: {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  } break;
  };
  return Core_Success;
}

Core_Result dx_assets_mesh_transform_range(dx_assets_mesh* SELF, DX_MAT4 const* a, Core_Size i, Core_Size n) {
  if (!SELF || !a) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i + n > SELF->number_of_vertices) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  for (Core_Size j = i, m = i + n; j < m; ++j) {
    dx_transform_point(&SELF->vertices.xyz[j], &SELF->vertices.xyz[j], a);
  }
  return Core_Success;
}

Core_Result dx_assets_mesh_append_vertex(dx_assets_mesh* SELF,
                                       DX_VEC3 const* xyz,
                                       DX_VEC4 const* ambient_rgba,
                                       DX_VEC2_F32 const* ambient_uv)
{
  Core_Size i = SELF->number_of_vertices;
  if (resize_vertex_arrays(SELF, false, i + 1)) {
    return Core_Failure;
  }
  SELF->vertices.xyz[i] = *xyz;
  SELF->vertices.ambient_rgba[i] = *ambient_rgba;
  SELF->vertices.ambient_uv[i] = *ambient_uv;
  return Core_Success;
}

Core_Result dx_assets_mesh_clear(dx_assets_mesh* SELF) {
  return resize_vertex_arrays(SELF, true, 0);
}

void dx_assets_mesh_set_mesh_ambient_rgba(dx_assets_mesh* SELF, DX_VEC4 const* value) {
  SELF->mesh.ambient_rgba = *value;
}

Core_Result dx_assets_mesh_append_range(dx_assets_mesh* SELF, Core_Size i, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i + n > SELF->number_of_vertices) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size j = SELF->number_of_vertices;
  if (resize_vertex_arrays(SELF, false, j + n)) {
    return Core_Failure;
  }
  for (Core_Size k = 0; k < n; ++k) {
    SELF->vertices.xyz[j + k] = SELF->vertices.xyz[i + k];
    SELF->vertices.ambient_rgba[j + k] = SELF->vertices.ambient_rgba[i + k];
    SELF->vertices.ambient_uv[j + k] = SELF->vertices.ambient_uv[i + k];
  }
  return Core_Success;
}
