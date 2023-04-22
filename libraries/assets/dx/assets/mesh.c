#include "dx/assets/mesh.h"

#include "dx/assets/mesh/generators.h"

// memcpy
#include <string.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.face",
                      dx_asset_face,
                       dx_object)

static void dx_asset_face_destruct(dx_asset_face* self) {
  if (self->indices) {
    dx_memory_deallocate(self->indices);
    self->indices = NULL;
  }
  self->number_of_indices = 0;
}

static void dx_asset_face_dispatch_construct(dx_asset_face_dispatch* self)
{/*Intentionally empty.*/}

int dx_asset_face_construct_triangle(dx_asset_face* self, uint32_t a, uint32_t b, uint32_t c) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_face_get_type();
  if (!_type) {
    return 1;
  }
  self->number_of_indices = 3;
  if (dx_memory_allocate(&self->indices, sizeof(uint32_t) * 3)) {
    return 1;
  }
  self->indices[0] = a;
  self->indices[1] = b;
  self->indices[2] = c;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_face* dx_asset_face_create_triangle(uint32_t a, uint32_t b, uint32_t c) {
  dx_asset_face* self = DX_ASSET_FACE(dx_object_alloc(sizeof(dx_asset_face)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_face_construct_triangle(self, a, b, c)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_face_construct_quadriliteral(dx_asset_face* self, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_face_get_type();
  if (!_type) {
    return 1;
  }
  self->number_of_indices = 4;
  if (dx_memory_allocate(&self->indices, sizeof(uint32_t) * 4)) {
    return 1;
  }
  self->indices[0] = a;
  self->indices[1] = b;
  self->indices[2] = c;
  self->indices[3] = d;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_face* dx_asset_face_create_quadriliteral(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
  dx_asset_face* self = DX_ASSET_FACE(dx_object_alloc(sizeof(dx_asset_face)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_face_construct_quadriliteral(self, a, b, c, d)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_face_construct_copy(dx_asset_face* self, dx_asset_face* other) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self == other) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_rti_type* _type = dx_asset_face_get_type();
  if (!_type) {
    return 1;
  }
  self->number_of_indices = other->number_of_indices;
  if (dx_memory_allocate(&self->indices, sizeof(uint32_t) * self->number_of_indices)) {
    return 1;
  }
  dx_memory_copy(self->indices, other->indices, sizeof(uint32_t) * self->number_of_indices);
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_face* dx_asset_face_create_copy(dx_asset_face* other) {
  dx_asset_face* self = DX_ASSET_FACE(dx_object_alloc(sizeof(dx_asset_face)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_face_construct_copy(self, other)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_asset_face_to_triangles(dx_asset_face* self, dx_inline_object_array* faces) {
  if (!self || !self->number_of_indices < 3) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (self->number_of_indices == 3) {
    dx_asset_face* a = dx_asset_face_create_copy(self);
    if (!a) {
      return 1;
    }
    if (dx_inline_object_array_append(faces, DX_OBJECT(a))) {
      DX_UNREFERENCE(a);
      a = NULL;
      return 1;
    }
    DX_UNREFERENCE(a);
    a = NULL;
    return 0;
  } else if (self->number_of_indices == 4) {
    dx_asset_face* a;
    //
    a = dx_asset_face_create_triangle(self->indices[0], self->indices[1], self->indices[3]);
    if (!a) {
      return 1;
    }
    if (dx_inline_object_array_append(faces, DX_OBJECT(a))) {
      DX_UNREFERENCE(a);
      a = NULL;
      return 1;
    }
    DX_UNREFERENCE(a);
    a = NULL;
    //
    a = dx_asset_face_create_triangle(self->indices[3], self->indices[2], self->indices[0]);
    if (!a) {
      return 1;
    }
    if (dx_inline_object_array_append(faces, DX_OBJECT(a))) {
      DX_UNREFERENCE(a);
      a = NULL;
      return 1;
    }
    DX_UNREFERENCE(a);
    a = NULL;
    return 0;
  } else {
    dx_set_error(DX_ERROR_NOT_IMPLEMENTED);
    return 1;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh",
                      dx_asset_mesh,
                      dx_object)

static inline dx_size dx_min_sz(dx_size x, dx_size y) {
  return x < y ? x : y;
}

static int resize_vertex_arrays(dx_asset_mesh* self, dx_bool shrink, dx_size number_of_vertices) {
  if (self->number_of_vertices == number_of_vertices) {
    // if the number of vertices in the mesh is equal to the required number of vertices, return success.
    return 0;
  }
  if (self->number_of_vertices > number_of_vertices && !shrink) {
    // if the number of vertices in the mesh is greater than the required and shrinking is not desired, return success.
    return 0;
  }
  // two cases:
  // - the number of vertices in the mesh is smaller than the required number of vertices or
  // - the number of vertices in the mesh is greater than the deisred number of vertices and shrinking is desired

  DX_VEC3* xyz = NULL;
  if (dx_memory_allocate(&xyz, number_of_vertices * sizeof(DX_VEC3))) {
    return 1;
  }
  dx_memory_copy(xyz, self->vertices.xyz, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC3));

  DX_VEC4* ambient_rgba = NULL;
  if (dx_memory_allocate(&ambient_rgba, number_of_vertices * sizeof(DX_VEC4))) {
    dx_memory_deallocate(xyz);
    xyz = NULL;
    return 1;
  }
  dx_memory_copy(ambient_rgba, self->vertices.ambient_rgba, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC4));

  DX_VEC2* ambient_uv = NULL;
  if (dx_memory_allocate(&ambient_uv, number_of_vertices * sizeof(DX_VEC2))) {
    dx_memory_deallocate(xyz);
    xyz = NULL;
    dx_memory_deallocate(ambient_rgba);
    ambient_rgba = NULL;
    return 1;
  }
  dx_memory_copy(ambient_uv, self->vertices.ambient_uv, dx_min_sz(self->number_of_vertices, number_of_vertices) * sizeof(DX_VEC2));

  self->vertices.xyz = xyz;
  self->vertices.ambient_rgba = ambient_rgba;
  self->vertices.ambient_uv = ambient_uv;
  self->number_of_vertices = number_of_vertices;

  return 0;
}

static void dx_asset_mesh_destruct(dx_asset_mesh* self) {
  dx_inline_object_array_uninitialize(&self->operations);
  if (self->name) {
    DX_UNREFERENCE(self->name);
    self->name = NULL;
  }
  if (self->material_reference) {
    DX_UNREFERENCE(self->material_reference);
    self->material_reference = NULL;
  }
  if (self->vertices.ambient_uv) {
    dx_memory_deallocate(self->vertices.ambient_uv);
    self->vertices.ambient_uv = NULL;
  }
  if (self->vertices.ambient_rgba) {
    dx_memory_deallocate(self->vertices.ambient_rgba);
    self->vertices.ambient_rgba = NULL;
  }
  if (self->vertices.xyz) {
    dx_memory_deallocate(self->vertices.xyz);
    self->vertices.xyz = NULL;
  }
}

static void dx_asset_mesh_dispatch_construct(dx_asset_mesh_dispatch* self)
{/*Intentionally empty.*/}

static dx_result dx_asset_mesh_construct(dx_asset_mesh* SELF, dx_string* name, dx_string* specifier, DX_VERTEX_FORMAT vertex_format, dx_asset_reference* material_reference) {
  if (!SELF || !name || !specifier || !material_reference) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_asset_mesh_get_type();
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
  
  dx_result (*generator)(dx_asset_mesh*)  = NULL;

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
      generator = &dx_asset_mesh_on_##name; \
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

dx_result dx_asset_mesh_create(dx_asset_mesh** RETURN, dx_string * name, dx_string* specifier, DX_VERTEX_FORMAT vertex_format, dx_asset_reference* material_reference) {
  dx_asset_mesh* SELF = DX_ASSET_MESH(dx_object_alloc(sizeof(dx_asset_mesh)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_asset_mesh_construct(SELF, name, specifier, vertex_format, material_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

int dx_asset_mesh_format(dx_asset_mesh* self, DX_VERTEX_FORMAT vertex_format, void** bytes, dx_size* number_of_bytes) {
  switch (vertex_format) {
  case DX_VERTEX_FORMAT_POSITION_XYZ: {
    void* p = NULL;
    if (dx_memory_allocate(&p, self->number_of_vertices * sizeof(DX_VEC3))) {
      return 1;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = self->vertices.xyz[i];
      q += sizeof(DX_VEC3);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC3));
    return 0;
  } break;
  case DX_VERTEX_FORMAT_AMBIENT_RGBA: {
    void* p = NULL;
    if (dx_memory_allocate(&p, self->number_of_vertices * sizeof(DX_VEC4))) {
      return 1;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC4*)q) = self->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC4));
    return 0;
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA: {
    void* p = NULL;
    if (dx_memory_allocate(&p, self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4)))) {
      return 1;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = self->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC4*)q) = self->vertices.ambient_rgba[i];
      q += sizeof(DX_VEC4);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC4));
    return 0;
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV: {
    void* p = NULL;
    if (dx_memory_allocate(&p, self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2)))) {
      return 1;
    }
    char* q = (char*)p;
    for (dx_size i = 0, n = self->number_of_vertices; i < n; ++i) {
      *((DX_VEC3*)q) = self->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((DX_VEC2*)q) = self->vertices.ambient_uv[i];
      q += sizeof(DX_VEC2);
    }
    *bytes = p;
    *number_of_bytes = self->number_of_vertices * (sizeof(DX_VEC3) + sizeof(DX_VEC2));
    return 0;
  } break;
  default: {
    return 1;
  } break;
  };
  return 0;
}

int dx_asset_mesh_transform_range(dx_asset_mesh* self, DX_MAT4 const* a, dx_size i, dx_size n) {
  if (!self || !a) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (i + n > self->number_of_vertices) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  for (dx_size j = i, m = i + n; j < m; ++j) {
    dx_transform_point(&self->vertices.xyz[j], &self->vertices.xyz[j], a);
  }
  return 0;
}

int dx_asset_mesh_append_vertex(dx_asset_mesh* self,
                                DX_VEC3 const* xyz,
                                DX_VEC4 const* ambient_rgba,
                                DX_VEC2 const* ambient_uv)
{
  dx_size i = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, i + 1)) {
    return 1;
  }
  self->vertices.xyz[i] = *xyz;
  self->vertices.ambient_rgba[i] = *ambient_rgba;
  self->vertices.ambient_uv[i] = *ambient_uv;
  return 0;
}

int dx_asset_mesh_clear(dx_asset_mesh* self) {
  return resize_vertex_arrays(self, true, 0);
}

void dx_asset_mesh_set_mesh_ambient_rgba(dx_asset_mesh* self, DX_VEC4 const* value) {
  self->mesh.ambient_rgba = *value;
}

int dx_asset_mesh_append_range(dx_asset_mesh* self, dx_size i, dx_size n) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (i + n > self->number_of_vertices) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  dx_size j = self->number_of_vertices;
  if (resize_vertex_arrays(self, false, j + n)) {
    return 1;
  }
  for (dx_size k = 0; k < n; ++k) {
    self->vertices.xyz[j + k] = self->vertices.xyz[i + k];
    self->vertices.ambient_rgba[j + k] = self->vertices.ambient_rgba[i + k];
    self->vertices.ambient_uv[j + k] = self->vertices.ambient_uv[i + k];
  }
  return 0;
}
