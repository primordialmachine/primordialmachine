#include "dx/assets/mesh.h"

#include "dx/assets/mesh/generators.h"

// memcpy
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.face",
                      dx_assets_face,
                      dx_object);

static void dx_assets_face_destruct(dx_assets_face* SELF) {
  if (SELF->indices) {
    dx_memory_deallocate(SELF->indices);
    SELF->indices = NULL;
  }
  SELF->number_of_indices = 0;
}

static void dx_assets_face_dispatch_construct(dx_assets_face_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_assets_face_construct_triangle(dx_assets_face* SELF, uint32_t a, uint32_t b, uint32_t c) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_assets_face_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->number_of_indices = 3;
  if (dx_memory_allocate(&SELF->indices, sizeof(uint32_t) * 3)) {
    return DX_FAILURE;
  }
  SELF->indices[0] = a;
  SELF->indices[1] = b;
  SELF->indices[2] = c;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_face_create_triangle(dx_assets_face** RETURN, uint32_t a, uint32_t b, uint32_t c) {
  DX_CREATE_PREFIX(dx_assets_face)
  if (dx_assets_face_construct_triangle(SELF, a, b, c)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_assets_face_construct_quadriliteral(dx_assets_face* SELF, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_assets_face_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->number_of_indices = 4;
  if (dx_memory_allocate(&SELF->indices, sizeof(uint32_t) * 4)) {
    return DX_FAILURE;
  }
  SELF->indices[0] = a;
  SELF->indices[1] = b;
  SELF->indices[2] = c;
  SELF->indices[3] = d;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_face_create_quadriliteral(dx_assets_face** RETURN, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  DX_CREATE_PREFIX(dx_assets_face)
  if (dx_assets_face_construct_quadriliteral(SELF, a, b, c, d)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_assets_face_construct_copy(dx_assets_face* SELF, dx_assets_face* other) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (SELF == other) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_assets_face_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->number_of_indices = other->number_of_indices;
  if (dx_memory_allocate(&SELF->indices, sizeof(uint32_t) * SELF->number_of_indices)) {
    return DX_FAILURE;
  }
  dx_memory_copy(SELF->indices, other->indices, sizeof(uint32_t) * SELF->number_of_indices);
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_face_create_copy(dx_assets_face** RETURN, dx_assets_face* other) {
  DX_CREATE_PREFIX(dx_assets_face)
  if (dx_assets_face_construct_copy(SELF, other)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_assets_face_to_triangles(dx_assets_face* SELF, dx_inline_object_array* faces) {
  if (!SELF || !SELF->number_of_indices < 3) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (SELF->number_of_indices == 3) {
    dx_assets_face* a = NULL;
    if (dx_assets_face_create_copy(&a, SELF)) {
      return DX_FAILURE;
    }
    if (dx_inline_object_array_append(faces, DX_OBJECT(a))) {
      DX_UNREFERENCE(a);
      a = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(a);
    a = NULL;
    return 0;
  } else if (SELF->number_of_indices == 4) {
    dx_assets_face* a;
    //
    if (dx_assets_face_create_triangle(&a, SELF->indices[0], SELF->indices[1], SELF->indices[3])) {
      return DX_FAILURE;
    }
    if (dx_inline_object_array_append(faces, DX_OBJECT(a))) {
      DX_UNREFERENCE(a);
      a = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(a);
    a = NULL;
    //
    if (dx_assets_face_create_triangle(&a, SELF->indices[3], SELF->indices[2], SELF->indices[0])) {
      return DX_FAILURE;
    }
    if (dx_inline_object_array_append(faces, DX_OBJECT(a))) {
      DX_UNREFERENCE(a);
      a = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(a);
    a = NULL;
    return DX_SUCCESS;
  } else {
    dx_set_error(DX_ERROR_NOT_IMPLEMENTED);
    return DX_FAILURE;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.assets.mesh",
                      dx_assets_mesh,
                      dx_object);

static dx_result resize_vertex_arrays(dx_assets_mesh* SELF, dx_bool shrink, dx_size number_of_vertices) {
  if (SELF->number_of_vertices == number_of_vertices) {
    // if the number of vertices in the mesh is equal to the required number of vertices, return success.
    return DX_SUCCESS;
  }
  if (SELF->number_of_vertices > number_of_vertices && !shrink) {
    // if the number of vertices in the mesh is greater than the required and shrinking is not desired, return success.
    return DX_SUCCESS;
  }
  // two cases:
  // - the number of vertices in the mesh is smaller than the required number of vertices or
  // - the number of vertices in the mesh is greater than the deisred number of vertices and shrinking is desired

  dx_size n;
  dx_min_sz(&n, SELF->number_of_vertices, number_of_vertices); // cannot fail
  
  // (1)
  DX_VEC3* xyz = NULL;
  if (dx_memory_allocate(&xyz, number_of_vertices * sizeof(DX_VEC3))) {
    return DX_FAILURE;
  }

  dx_memory_copy(xyz, SELF->vertices.xyz, n * sizeof(DX_VEC3));

  // (2)
  DX_VEC4* ambient_rgba = NULL;
  if (dx_memory_allocate(&ambient_rgba, number_of_vertices * sizeof(DX_VEC4))) {
    dx_memory_deallocate(xyz);
    xyz = NULL;
    return DX_FAILURE;
  }
  dx_memory_copy(ambient_rgba, SELF->vertices.ambient_rgba, n * sizeof(DX_VEC4));

  // (3)
  DX_VEC2_F32* ambient_uv = NULL;
  if (dx_memory_allocate(&ambient_uv, number_of_vertices * sizeof(DX_VEC2_F32))) {
    dx_memory_deallocate(xyz);
    xyz = NULL;
    dx_memory_deallocate(ambient_rgba);
    ambient_rgba = NULL;
    return DX_FAILURE;
  }
  dx_memory_copy(ambient_uv, SELF->vertices.ambient_uv, n * sizeof(DX_VEC2_F32));

  SELF->vertices.xyz = xyz;
  SELF->vertices.ambient_rgba = ambient_rgba;
  SELF->vertices.ambient_uv = ambient_uv;
  SELF->number_of_vertices = number_of_vertices;

  return DX_SUCCESS;
}

static void dx_assets_mesh_destruct(dx_assets_mesh* SELF) {
  dx_inline_object_array_uninitialize(&SELF->operations);
  if (SELF->name) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  if (SELF->material_reference) {
    DX_UNREFERENCE(SELF->material_reference);
    SELF->material_reference = NULL;
  }
  if (SELF->vertices.ambient_uv) {
    dx_memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
  }
  if (SELF->vertices.ambient_rgba) {
    dx_memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
  }
  if (SELF->vertices.xyz) {
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
  }
}

static void dx_assets_mesh_dispatch_construct(dx_assets_mesh_dispatch* SELF)
{/*Intentionally empty.*/}

static dx_result dx_assets_mesh_construct(dx_assets_mesh* SELF, dx_string* name, dx_string* specifier, dx_vertex_format vertex_format, dx_asset_reference* material_reference) {
  if (!SELF || !name || !specifier || !material_reference) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_assets_mesh_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  // "default" mesh
  if (dx_memory_allocate(&SELF->vertices.xyz, 0)) {
    return DX_FAILURE;
  }

  if (dx_memory_allocate(&SELF->vertices.ambient_rgba, 0)) {
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return DX_FAILURE;
  }

  if (dx_memory_allocate(&SELF->vertices.ambient_uv, 0)) {
    dx_memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return DX_FAILURE;
  }
  SELF->mesh.ambient_rgba = (DX_VEC4){ 0.f, 0.f, 0.f, 0.f };
  SELF->number_of_vertices = 0;
  
  dx_result (*generator)(dx_assets_mesh*)  = NULL;

#define SELECT_GENERATOR(name) \
  { \
    dx_string* temporary = NULL; \
    if (dx_string_create(&temporary, #name, strlen(#name))) { \
      dx_memory_deallocate(SELF->vertices.ambient_uv); \
      SELF->vertices.ambient_uv = NULL; \
      dx_memory_deallocate(SELF->vertices.ambient_rgba); \
      SELF->vertices.ambient_rgba = NULL; \
      dx_memory_deallocate(SELF->vertices.xyz); \
      SELF->vertices.xyz = NULL; \
      return DX_FAILURE; \
    } \
    if (dx_string_is_equal_to(specifier, temporary)) { \
      generator = &dx_assets_mesh_on_##name; \
    } \
    DX_UNREFERENCE(temporary); \
    temporary = NULL; \
  }

SELECT_GENERATOR(cube)
SELECT_GENERATOR(empty)
SELECT_GENERATOR(quadriliteral)
SELECT_GENERATOR(triangle)
SELECT_GENERATOR(octahedron)

#undef SELECT_GENERATOR
  
  if (!generator) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    dx_memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    dx_memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return DX_FAILURE;
  }

  if ((*generator)(SELF)) {
    dx_memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    dx_memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return DX_FAILURE;
  }
  if (!material_reference) {
    dx_memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    dx_memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return DX_FAILURE;
  }
  SELF->material_reference = material_reference;
  DX_REFERENCE(SELF->material_reference);

  SELF->name = name;
  DX_REFERENCE(name);

  SELF->vertex_format = vertex_format;

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    DX_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    DX_UNREFERENCE(SELF->material_reference);
    SELF->material_reference = NULL;
    dx_memory_deallocate(SELF->vertices.ambient_uv);
    SELF->vertices.ambient_uv = NULL;
    dx_memory_deallocate(SELF->vertices.ambient_rgba);
    SELF->vertices.ambient_rgba = NULL;
    dx_memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_assets_mesh_create(dx_assets_mesh** RETURN, dx_string * name, dx_string* specifier, dx_vertex_format vertex_format, dx_asset_reference* material_reference) {
  DX_CREATE_PREFIX(dx_assets_mesh)
  if (dx_assets_mesh_construct(SELF, name, specifier, vertex_format, material_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_assets_mesh_format(dx_assets_mesh* SELF, dx_vertex_format vertex_format, void** bytes, dx_size* number_of_bytes) {
  switch (vertex_format) {
  case dx_vertex_format_position_xyz: {
    void* p = NULL;
    if (dx_memory_allocate(&p, SELF->number_of_vertices * sizeof(DX_VEC3))) {
      return DX_FAILURE;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC3));
    return DX_SUCCESS;
  } break;
  case dx_vertex_format_ambient_rgba: {
    void* p = NULL;
    if (dx_memory_allocate(&p, SELF->number_of_vertices * sizeof(DX_VEC4))) {
      return DX_FAILURE;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC4*)q) = SELF->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC4));
    return DX_SUCCESS;
  } break;
  case dx_vertex_format_position_xyz_ambient_rgba: {
    void* p = NULL;
    if (dx_memory_allocate(&p, SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4)))) {
      return DX_FAILURE;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC4*)q) = SELF->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4));
    return DX_SUCCESS;
  } break;
  case dx_vertex_format_position_xyz_ambient_uv: {
    void* p = NULL;
    if (dx_memory_allocate(&p, SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2_F32)))) {
      return DX_FAILURE;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = SELF->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC2_F32*)q) = SELF->vertices.ambient_uv[i];
      q += sizeof(DX_VEC2_F32);
    }
    *bytes = p;
    *number_of_bytes = SELF->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2_F32));
    return DX_SUCCESS;
  } break;
  default: {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  } break;
  };
  return DX_SUCCESS;
}

dx_result dx_assets_mesh_transform_range(dx_assets_mesh* SELF, DX_MAT4 const* a, dx_size i, dx_size n) {
  if (!SELF || !a) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (i + n > SELF->number_of_vertices) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  for (dx_size j = i, m = i + n; j < m; ++j) {
    dx_transform_point(&SELF->vertices.xyz[j], &SELF->vertices.xyz[j], a);
  }
  return DX_SUCCESS;
}

dx_result dx_assets_mesh_append_vertex(dx_assets_mesh* SELF,
                                       DX_VEC3 const* xyz,
                                       DX_VEC4 const* ambient_rgba,
                                       DX_VEC2_F32 const* ambient_uv)
{
  dx_size i = SELF->number_of_vertices;
  if (resize_vertex_arrays(SELF, false, i + 1)) {
    return DX_FAILURE;
  }
  SELF->vertices.xyz[i] = *xyz;
  SELF->vertices.ambient_rgba[i] = *ambient_rgba;
  SELF->vertices.ambient_uv[i] = *ambient_uv;
  return DX_SUCCESS;
}

dx_result dx_assets_mesh_clear(dx_assets_mesh* SELF) {
  return resize_vertex_arrays(SELF, true, 0);
}

void dx_assets_mesh_set_mesh_ambient_rgba(dx_assets_mesh* SELF, DX_VEC4 const* value) {
  SELF->mesh.ambient_rgba = *value;
}

dx_result dx_assets_mesh_append_range(dx_assets_mesh* SELF, dx_size i, dx_size n) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (i + n > SELF->number_of_vertices) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size j = SELF->number_of_vertices;
  if (resize_vertex_arrays(SELF, false, j + n)) {
    return DX_FAILURE;
  }
  for (dx_size k = 0; k < n; ++k) {
    SELF->vertices.xyz[j + k] = SELF->vertices.xyz[i + k];
    SELF->vertices.ambient_rgba[j + k] = SELF->vertices.ambient_rgba[i + k];
    SELF->vertices.ambient_uv[j + k] = SELF->vertices.ambient_uv[i + k];
  }
  return DX_SUCCESS;
}
