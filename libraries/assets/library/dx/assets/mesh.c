#include "dx/assets/mesh.h"

#include "dx/assets/mesh/generators.h"

// memcpy
#include <string.h>

Core_defineObjectType("Core.Assets.Mesh",
                      Core_Assets_Mesh,
                      Core_Object);

static Core_Result resize_vertex_arrays(Core_Assets_Mesh* SELF, Core_Boolean shrink, Core_Size numberOfVertices) {
  if (SELF->numberOfVertices == numberOfVertices) {
    // if the number of vertices in the mesh is equal to the required number of vertices, return success.
    return Core_Success;
  }
  if (SELF->numberOfVertices > numberOfVertices && !shrink) {
    // if the number of vertices in the mesh is greater than the required and shrinking is not desired, return success.
    return Core_Success;
  }
  // two cases:
  // - the number of vertices in the mesh is smaller than the required number of vertices or
  // - the number of vertices in the mesh is greater than the deisred number of vertices and shrinking is desired

  Core_Size n;
  Core_minSz(&n, SELF->numberOfVertices, numberOfVertices); // cannot fail
  
  // (1)
  DX_VEC3* xyz = NULL;
  if (Core_Memory_allocate(&xyz, numberOfVertices * sizeof(DX_VEC3))) {
    return Core_Failure;
  }

  Core_Memory_copy(xyz, SELF->vertices.xyz, n * sizeof(DX_VEC3));

  // (2)
  Core_InlineRgbaR32* ambientRgba = NULL;
  if (Core_Memory_allocate(&ambientRgba, numberOfVertices * sizeof(Core_InlineRgbaR32))) {
    Core_Memory_deallocate(xyz);
    xyz = NULL;
    return Core_Failure;
  }
  Core_Memory_copy(ambientRgba, SELF->vertices.ambientRgba, n * sizeof(Core_InlineRgbaR32));

  // (3)
  Core_InlineVector2R32* ambientUv = NULL;
  if (Core_Memory_allocate(&ambientUv, numberOfVertices * sizeof(Core_InlineVector2R32))) {
    Core_Memory_deallocate(xyz);
    xyz = NULL;
    Core_Memory_deallocate(ambientRgba);
    ambientRgba = NULL;
    return Core_Failure;
  }
  Core_Memory_copy(ambientUv, SELF->vertices.ambientUv, n * sizeof(Core_InlineVector2R32));

  SELF->vertices.xyz = xyz;
  SELF->vertices.ambientRgba = ambientRgba;
  SELF->vertices.ambientUv = ambientUv;
  SELF->numberOfVertices = numberOfVertices;

  return Core_Success;
}

static void Core_Assets_Mesh_destruct(Core_Assets_Mesh* SELF) {
  dx_inline_object_array_uninitialize(&SELF->operations);
  if (SELF->name) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
  }
  if (SELF->materialReference) {
    CORE_UNREFERENCE(SELF->materialReference);
    SELF->materialReference = NULL;
  }
  if (SELF->vertices.ambientUv) {
    Core_Memory_deallocate(SELF->vertices.ambientUv);
    SELF->vertices.ambientUv = NULL;
  }
  if (SELF->vertices.ambientRgba) {
    Core_Memory_deallocate(SELF->vertices.ambientRgba);
    SELF->vertices.ambientRgba = NULL;
  }
  if (SELF->vertices.xyz) {
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
  }
}

static void Core_Assets_Mesh_constructDispatch(Core_Assets_Mesh_Dispatch* SELF)
{/*Intentionally empty.*/}

static Core_Result Core_Assets_Mesh_construct(Core_Assets_Mesh* SELF, Core_String* name, Core_String* specifier, Core_VertexFormat vertex_format, Core_Assets_Ref* material_reference) {
  DX_CONSTRUCT_PREFIX(Core_Assets_Mesh);
  if (!name || !specifier || !material_reference) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // "default" mesh
  if (Core_Memory_allocate(&SELF->vertices.xyz, 0)) {
    return Core_Failure;
  }

  if (Core_Memory_allocate(&SELF->vertices.ambientRgba, 0)) {
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }

  if (Core_Memory_allocate(&SELF->vertices.ambientUv, 0)) {
    Core_Memory_deallocate(SELF->vertices.ambientRgba);
    SELF->vertices.ambientRgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }
  SELF->mesh.ambient_rgba = (Core_InlineRgbaR32){ .r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f };
  SELF->numberOfVertices = 0;
  
  Core_Result (*generator)(Core_Assets_Mesh*)  = NULL;

#define SELECT_GENERATOR(SPECIFIER, TYPE) \
  { \
    Core_String* temporary = NULL; \
    if (Core_String_create(&temporary, SPECIFIER, strlen(SPECIFIER))) { \
      Core_Memory_deallocate(SELF->vertices.ambientUv); \
      SELF->vertices.ambientUv = NULL; \
      Core_Memory_deallocate(SELF->vertices.ambientRgba); \
      SELF->vertices.ambientRgba = NULL; \
      Core_Memory_deallocate(SELF->vertices.xyz); \
      SELF->vertices.xyz = NULL; \
      return Core_Failure; \
    } \
    Core_Boolean isEqualTo = Core_False; \
    if (Core_String_isEqualTo(&isEqualTo, specifier, temporary)) { \
      CORE_UNREFERENCE(temporary); \
      temporary = NULL; \
      Core_Memory_deallocate(SELF->vertices.ambientUv); \
      SELF->vertices.ambientUv = NULL; \
      Core_Memory_deallocate(SELF->vertices.ambientRgba); \
      SELF->vertices.ambientRgba = NULL; \
      Core_Memory_deallocate(SELF->vertices.xyz); \
      SELF->vertices.xyz = NULL; \
      return Core_Failure; \
    } \
    if (isEqualTo) { \
      generator = &Core_Assets_Mesh_on##TYPE; \
    } \
    CORE_UNREFERENCE(temporary); \
    temporary = NULL; \
  }

SELECT_GENERATOR("cube", Cube)
SELECT_GENERATOR("empty", Empty)
SELECT_GENERATOR("quadriliteral", Quadriliteral)
SELECT_GENERATOR("triangle", Triangle)
SELECT_GENERATOR("octahedron", Octahedron)

#undef SELECT_GENERATOR
  
  if (!generator) {
    Core_setError(Core_Error_ArgumentInvalid);
    Core_Memory_deallocate(SELF->vertices.ambientUv);
    SELF->vertices.ambientUv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambientRgba);
    SELF->vertices.ambientRgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }

  if ((*generator)(SELF)) {
    Core_Memory_deallocate(SELF->vertices.ambientUv);
    SELF->vertices.ambientUv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambientRgba);
    SELF->vertices.ambientRgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }
  if (!material_reference) {
    Core_Memory_deallocate(SELF->vertices.ambientUv);
    SELF->vertices.ambientUv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambientRgba);
    SELF->vertices.ambientRgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }
  SELF->materialReference = material_reference;
  CORE_REFERENCE(SELF->materialReference);

  SELF->name = name;
  CORE_REFERENCE(name);

  SELF->vertexFormat = vertex_format;

  if (dx_inline_object_array_initialize(&SELF->operations, 0)) {
    CORE_UNREFERENCE(SELF->name);
    SELF->name = NULL;
    CORE_UNREFERENCE(SELF->materialReference);
    SELF->materialReference = NULL;
    Core_Memory_deallocate(SELF->vertices.ambientUv);
    SELF->vertices.ambientUv = NULL;
    Core_Memory_deallocate(SELF->vertices.ambientRgba);
    SELF->vertices.ambientRgba = NULL;
    Core_Memory_deallocate(SELF->vertices.xyz);
    SELF->vertices.xyz = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result Core_Assets_Mesh_create(Core_Assets_Mesh** RETURN, Core_String * name, Core_String* specifier, Core_VertexFormat vertex_format, Core_Assets_Ref* material_reference) {
  DX_CREATE_PREFIX(Core_Assets_Mesh);
  if (Core_Assets_Mesh_construct(SELF, name, specifier, vertex_format, material_reference)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Assets_Mesh_format(Core_Assets_Mesh* SELF, Core_VertexFormat vertex_format, void** bytes, Core_Size* number_of_bytes) {
  switch (vertex_format) {
  case Core_VertexFormat_PositionXyz: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->numberOfVertices * sizeof(DX_VEC3))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->numberOfVertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
    }
    *bytes = p;
    *number_of_bytes = SELF->numberOfVertices * (sizeof(DX_VEC3));
    return Core_Success;
  } break;
  case Core_VertexFormat_AmbientRgba: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->numberOfVertices * sizeof(Core_InlineRgbaR32))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->numberOfVertices; i < n; ++i) {
      *((Core_InlineRgbaR32*)q) = SELF->vertices.ambientRgba[i];
      q += sizeof(Core_InlineRgbaR32);
    }
    *bytes = p;
    *number_of_bytes = SELF->numberOfVertices * (sizeof(Core_InlineRgbaR32));
    return Core_Success;
  } break;
  case Core_VertexFormat_PositionXyzAmbientRgba: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->numberOfVertices * (sizeof(DX_VEC3) + sizeof(Core_InlineRgbaR32)))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->numberOfVertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((Core_InlineRgbaR32*)q) = SELF->vertices.ambientRgba[i];
      q += sizeof(Core_InlineRgbaR32);
    }
    *bytes = p;
    *number_of_bytes = SELF->numberOfVertices * (sizeof(DX_VEC3) + sizeof(Core_InlineRgbaR32));
    return Core_Success;
  } break;
  case Core_VertexFormat_PositionXyzAmbientUv: {
    void* p = NULL;
    if (Core_Memory_allocate(&p, SELF->numberOfVertices * (sizeof(DX_VEC3) + sizeof(Core_InlineVector2R32)))) {
      return Core_Failure;
    }
    char* q = (char*)p;
    for (Core_Size i = 0, n = SELF->numberOfVertices; i < n; ++i) {
      *((DX_VEC3*)q) = SELF->vertices.xyz[i];
      q += sizeof(DX_VEC3);
      *((Core_InlineVector2R32*)q) = SELF->vertices.ambientUv[i];
      q += sizeof(Core_InlineVector2R32);
    }
    *bytes = p;
    *number_of_bytes = SELF->numberOfVertices * (sizeof(DX_VEC3) + sizeof(Core_InlineVector2R32));
    return Core_Success;
  } break;
  default: {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  } break;
  };
  return Core_Success;
}

Core_Result Core_Assets_Mesh_transformRange(Core_Assets_Mesh* SELF, DX_MAT4 const* a, Core_Size i, Core_Size n) {
  if (!SELF || !a) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i + n > SELF->numberOfVertices) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  for (Core_Size j = i, m = i + n; j < m; ++j) {
    dx_transform_point(&SELF->vertices.xyz[j], &SELF->vertices.xyz[j], a);
  }
  return Core_Success;
}

Core_Result Core_Assets_Mesh_appendVertex(Core_Assets_Mesh* SELF,
                                          DX_VEC3 const* xyz,
                                          Core_InlineRgbaR32 const* ambientRgba,
                                          Core_InlineVector2R32 const* ambientUv)
{
  Core_Size i = SELF->numberOfVertices;
  if (resize_vertex_arrays(SELF, false, i + 1)) {
    return Core_Failure;
  }
  SELF->vertices.xyz[i] = *xyz;
  SELF->vertices.ambientRgba[i] = *ambientRgba;
  SELF->vertices.ambientUv[i] = *ambientUv;
  return Core_Success;
}

Core_Result Core_Assets_Mesh_clear(Core_Assets_Mesh* SELF) {
  return resize_vertex_arrays(SELF, true, 0);
}

Core_Result Core_Assets_Mesh_setMeshAmbientRgba(Core_Assets_Mesh* SELF, Core_InlineRgbaR32 const* value) {
  SELF->mesh.ambient_rgba = *value;
  return Core_Success;
}

Core_Result Core_Assets_Mesh_appendRange(Core_Assets_Mesh* SELF, Core_Size i, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (i + n > SELF->numberOfVertices) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size j = SELF->numberOfVertices;
  if (resize_vertex_arrays(SELF, false, j + n)) {
    return Core_Failure;
  }
  for (Core_Size k = 0; k < n; ++k) {
    SELF->vertices.xyz[j + k] = SELF->vertices.xyz[i + k];
    SELF->vertices.ambientRgba[j + k] = SELF->vertices.ambientRgba[i + k];
    SELF->vertices.ambientUv[j + k] = SELF->vertices.ambientUv[i + k];
  }
  return Core_Success;
}
