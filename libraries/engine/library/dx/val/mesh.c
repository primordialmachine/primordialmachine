#include "dx/val/mesh.h"

// snprintf
#include <stdio.h>
#include "dx/val/program_text.h"

Core_defineObjectType("dx.val.mesh",
                      dx_val_mesh,
                      Core_Object);

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_MESH_AMBIENT_RGBA (1)

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA (2)

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_VERTEX_AMBIENT_UV (4)

/// @brief See shader program source code for details.
#define DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE (8)

// @brief
// Load a program consisting of a vertex and a fragment shader.
// @param path
// The path (relative or absolute) to the directory where the two shader programs reside.
// @param filename
// The filename (without path or extension) of the shader programs.
// @param flags
// Flags to customize the behavior of the shaders are load time.
// @remarks
// This function loads the vertex shader @code{<path>/<filename>.vs} and the fragment shader @code{<path>/<filename>.fs}.
// The shaders can be adjusted at load time via the @a flags.
static dx_val_program_text* load_program(Core_String* path, Core_String* filename, uint8_t flags);

static Core_Result add_material_to_backend(dx_val_mesh* SELF);

static void remove_material_from_backend(dx_val_mesh* SELF);

static Core_Result add_to_backend(dx_val_mesh* SELF);

static void remove_from_backend(dx_val_mesh* SELF);

static dx_val_program_text* load_program(Core_String* path, Core_String* filename, uint8_t flags) {
  dx_val_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_val_program_text* program = NULL;
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.vs", sizeof("${s}/${s}.vs") - 1)) {
      goto on_error;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      CORE_UNREFERENCE(format);
      format = NULL;
      goto on_error;
    }
    CORE_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&vertex_program, DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, p)) {
      CORE_UNREFERENCE(p);
      p = NULL;
      goto on_error;
    }
    CORE_UNREFERENCE(p);
    p = NULL;
  }
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.fs", sizeof("${s}/${s}.fs") - 1)) {
      goto on_error;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      CORE_UNREFERENCE(format);
      format = NULL;
      goto on_error;
    }
    CORE_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&fragment_program, DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT, p)) {
      CORE_UNREFERENCE(p);
      p = NULL;
      goto on_error;
    }
    CORE_UNREFERENCE(p);
    p = NULL;
  }
  {
    if (dx_val_program_text_create(&program, vertex_program, fragment_program)) {
      CORE_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      CORE_UNREFERENCE(fragment_program);
      fragment_program = NULL;
      goto on_error;
    }
    CORE_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    CORE_UNREFERENCE(fragment_program);
    fragment_program = NULL;
    if (DX_PROGRAM_WITH_MESH_AMBIENT_RGBA == (flags & DX_PROGRAM_WITH_MESH_AMBIENT_RGBA)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_MESH_AMBIENT_RGBA", sizeof("WITH_MESH_AMBIENT_RGBA") - 1)) {
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        CORE_UNREFERENCE(name);
        name = NULL;
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      CORE_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA == (flags & DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_VERTEX_AMBIENT_RGBA", sizeof("WITH_VERTEX_AMBIENT_RGBA") - 1)) {
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        CORE_UNREFERENCE(name);
        name = NULL;
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      CORE_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_VERTEX_AMBIENT_UV == (flags & DX_PROGRAM_WITH_VERTEX_AMBIENT_UV)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_VERTEX_AMBIENT_UV", sizeof("WITH_VERTEX_AMBIENT_UV") - 1)) {
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        CORE_UNREFERENCE(name);
        name = NULL;
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      CORE_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE == (flags & DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_MATERIAL_AMBIENT_TEXTURE", sizeof("WITH_MATERIAL_AMBIENT_TEXTURE") - 1)) {
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        CORE_UNREFERENCE(name);
        name = NULL;
        CORE_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      CORE_UNREFERENCE(name);
      name = NULL;
    }
  }
  return program;
on_error:
  if (program) {
    CORE_UNREFERENCE(program);
    program = NULL;
  }
  if (fragment_program) {
    CORE_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  if (vertex_program) {
    CORE_UNREFERENCE(vertex_program);
    vertex_program = NULL;
  }
  return NULL;
}

static Core_Result add_material_to_backend(dx_val_mesh* SELF) {
  if (SELF->mesh_asset->materialReference && SELF->mesh_asset->materialReference->object) {
    if (dx_val_material_create(&SELF->material, SELF->context, CORE_ASSETS_MATERIAL(SELF->mesh_asset->materialReference->object))) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

static void remove_material_from_backend(dx_val_mesh* SELF) {
  if (SELF->material) {
    CORE_UNREFERENCE(SELF->material);
    SELF->material = NULL;
  }
}

static Core_Result add_to_backend(dx_val_mesh* SELF) {
  Core_VertexFormat vertex_format = SELF->mesh_asset->vertexFormat;

  // create buffer
  if (Core_Visuals_Context_createBuffer(&SELF->buffer, SELF->context)) {
    return Core_Failure;
  }

  // upload data to buffer
  void* bytes; Core_Size number_of_bytes;
  if (Core_Assets_Mesh_format(SELF->mesh_asset, vertex_format, &bytes, &number_of_bytes)) {
    return Core_Failure;
  }
  if (dx_val_buffer_set_data(SELF->buffer, bytes, number_of_bytes)) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    return Core_Failure;
  }
  Core_Memory_deallocate(bytes);
  bytes = NULL;

  // create variable binding
  if (Core_Visuals_Context_createVariableBinding(&SELF->vbinding, SELF->context, vertex_format, SELF->buffer)) {
    CORE_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  }

  // create the program
  uint8_t flags = DX_PROGRAM_WITH_MESH_AMBIENT_RGBA;
  switch (vertex_format) {
  case Core_VertexFormat_PositionXyz: {
    /*Intentionally empty.*/
  } break;
  case Core_VertexFormat_PositionXyzAmbientRgba: {
    flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA;
  } break;
  case Core_VertexFormat_PositionXyzAmbientUv: {
   flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_UV;
   if (SELF->material->material_asset->ambientTextureReference && SELF->material->material_asset->ambientTextureReference->object) {
      flags |= DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE;
    }
  } break;
  default: {
    CORE_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    CORE_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  } break;
  };
  Core_String* path = NULL;
  if (Core_String_create(&path, "assets/gl", sizeof("assets/gl") - 1)) {
    return Core_Failure;
  }
  Core_String* filename = NULL;
  if (Core_String_create(&filename, "3", sizeof("3") - 1)) {
    CORE_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  dx_val_program_text* program_text = load_program(path, filename, flags);
  CORE_UNREFERENCE(filename);
  filename = NULL;
  CORE_UNREFERENCE(path);
  path = NULL;
  if (!program_text) {
    CORE_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    CORE_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  }
  if (Core_Visuals_Context_createProgram(&SELF->program, SELF->context, program_text)) {
    CORE_UNREFERENCE(program_text);
    program_text = NULL;
    CORE_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    CORE_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(program_text);
  program_text = NULL;
  return Core_Success;
}

static void remove_from_backend(dx_val_mesh* SELF) {
  if (SELF->program) {
    CORE_UNREFERENCE(SELF->program);
    SELF->program = NULL;
  }
  if (SELF->vbinding) {
    CORE_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
  }
  if (SELF->buffer) {
    CORE_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
  }
}

static void dx_val_mesh_destruct(dx_val_mesh* SELF) {
  remove_material_from_backend(SELF);
  remove_from_backend(SELF);
  CORE_UNREFERENCE(SELF->mesh_asset);
  SELF->mesh_asset = NULL;
}

static void dx_val_mesh_constructDispatch(dx_val_mesh_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_mesh_construct(dx_val_mesh* SELF, Core_Visuals_Context* context, Core_Assets_Mesh* mesh_asset) {
  DX_CONSTRUCT_PREFIX(dx_val_mesh);

  SELF->mesh_asset = mesh_asset;
  CORE_REFERENCE(mesh_asset);

  SELF->context = context;

  if (add_material_to_backend(SELF)) {
    CORE_UNREFERENCE(SELF->mesh_asset);
    SELF->mesh_asset = NULL;
    return Core_Failure;
  }

  if (add_to_backend(SELF)) {
    remove_material_from_backend(SELF);
    CORE_UNREFERENCE(SELF->mesh_asset);
    SELF->mesh_asset = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_mesh_create(dx_val_mesh** RETURN, Core_Visuals_Context* context, Core_Assets_Mesh* mesh_asset) {
  DX_CREATE_PREFIX(dx_val_mesh);
  if (dx_val_mesh_construct(SELF, context, mesh_asset)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
