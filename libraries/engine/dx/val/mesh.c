#include "dx/val/mesh.h"

// snprintf
#include <stdio.h>
#include "dx/val/program_text.h"

DX_DEFINE_OBJECT_TYPE("dx.val.mesh",
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
      DX_UNREFERENCE(format);
      format = NULL;
      goto on_error;
    }
    DX_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&vertex_program, DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, p)) {
      DX_UNREFERENCE(p);
      p = NULL;
      goto on_error;
    }
    DX_UNREFERENCE(p);
    p = NULL;
  }
  {
    Core_String* format = NULL;
    if (Core_String_create(&format, "${s}/${s}.fs", sizeof("${s}/${s}.fs") - 1)) {
      goto on_error;
    }
    Core_String* p = NULL;
    if (Core_String_printf(&p, format, path, filename)) {
      DX_UNREFERENCE(format);
      format = NULL;
      goto on_error;
    }
    DX_UNREFERENCE(format);
    format = NULL;
    if (dx_val_program_text_create_from_file(&fragment_program, DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT, p)) {
      DX_UNREFERENCE(p);
      p = NULL;
      goto on_error;
    }
    DX_UNREFERENCE(p);
    p = NULL;
  }
  {
    if (dx_val_program_text_create(&program, vertex_program, fragment_program)) {
      DX_UNREFERENCE(vertex_program);
      vertex_program = NULL;
      DX_UNREFERENCE(fragment_program);
      fragment_program = NULL;
      goto on_error;
    }
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
    if (DX_PROGRAM_WITH_MESH_AMBIENT_RGBA == (flags & DX_PROGRAM_WITH_MESH_AMBIENT_RGBA)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_MESH_AMBIENT_RGBA", sizeof("WITH_MESH_AMBIENT_RGBA") - 1)) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA == (flags & DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_VERTEX_AMBIENT_RGBA", sizeof("WITH_VERTEX_AMBIENT_RGBA") - 1)) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_VERTEX_AMBIENT_UV == (flags & DX_PROGRAM_WITH_VERTEX_AMBIENT_UV)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_VERTEX_AMBIENT_UV", sizeof("WITH_VERTEX_AMBIENT_UV") - 1)) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
    if (DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE == (flags & DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE)) {
      Core_String* name = NULL;
      if (Core_String_create(&name, "WITH_MATERIAL_AMBIENT_TEXTURE", sizeof("WITH_MATERIAL_AMBIENT_TEXTURE") - 1)) {
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      if (dx_val_program_text_add_define(program, name)) {
        DX_UNREFERENCE(name);
        name = NULL;
        DX_UNREFERENCE(program);
        program = NULL;
        goto on_error;
      }
      DX_UNREFERENCE(name);
      name = NULL;
    }
  }
  return program;
on_error:
  if (program) {
    DX_UNREFERENCE(program);
    program = NULL;
  }
  if (fragment_program) {
    DX_UNREFERENCE(fragment_program);
    fragment_program = NULL;
  }
  if (vertex_program) {
    DX_UNREFERENCE(vertex_program);
    vertex_program = NULL;
  }
  return NULL;
}

static Core_Result add_material_to_backend(dx_val_mesh* SELF) {
  if (SELF->mesh_asset->material_reference && SELF->mesh_asset->material_reference->object) {
    if (dx_val_material_create(&SELF->material, SELF->context, DX_ASSETS_MATERIAL(SELF->mesh_asset->material_reference->object))) {
      return Core_Failure;
    }
  }
  return Core_Success;
}

static void remove_material_from_backend(dx_val_mesh* SELF) {
  if (SELF->material) {
    DX_UNREFERENCE(SELF->material);
    SELF->material = NULL;
  }
}

static Core_Result add_to_backend(dx_val_mesh* SELF) {
  Core_VertexFormat vertex_format = SELF->mesh_asset->vertex_format;

  // create buffer
  if (dx_val_context_create_buffer(&SELF->buffer, SELF->context)) {
    return Core_Failure;
  }

  // upload data to buffer
  void* bytes; Core_Size number_of_bytes;
  if (dx_assets_mesh_format(SELF->mesh_asset, vertex_format, &bytes, &number_of_bytes)) {
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
  if (dx_val_context_create_vbinding(&SELF->vbinding, SELF->context, vertex_format, SELF->buffer)) {
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  }

  // create the program
  uint8_t flags = DX_PROGRAM_WITH_MESH_AMBIENT_RGBA;
  switch (vertex_format) {
  case Core_VertexFormat_position_xyz: {
    /*Intentionally empty.*/
  } break;
  case Core_VertexFormat_position_xyz_ambient_rgba: {
    flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA;
  } break;
  case Core_VertexFormat_position_xyz_ambient_uv: {
   flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_UV;
   if (SELF->material->material_asset->ambient_texture_reference && SELF->material->material_asset->ambient_texture_reference->object) {
      flags |= DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE;
    }
  } break;
  default: {
    DX_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    DX_UNREFERENCE(SELF->buffer);
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
    DX_UNREFERENCE(path);
    path = NULL;
    return Core_Failure;
  }
  dx_val_program_text* program_text = load_program(path, filename, flags);
  DX_UNREFERENCE(filename);
  filename = NULL;
  DX_UNREFERENCE(path);
  path = NULL;
  if (!program_text) {
    DX_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  }
  if (dx_val_context_create_program(&SELF->program, SELF->context, program_text)) {
    DX_UNREFERENCE(program_text);
    program_text = NULL;
    DX_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(program_text);
  program_text = NULL;
  return Core_Success;
}

static void remove_from_backend(dx_val_mesh* SELF) {
  if (SELF->program) {
    DX_UNREFERENCE(SELF->program);
    SELF->program = NULL;
  }
  if (SELF->vbinding) {
    DX_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
  }
  if (SELF->buffer) {
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
  }
}

static void dx_val_mesh_destruct(dx_val_mesh* SELF) {
  remove_material_from_backend(SELF);
  remove_from_backend(SELF);
  DX_UNREFERENCE(SELF->mesh_asset);
  SELF->mesh_asset = NULL;
}

static void dx_val_mesh_constructDispatch(dx_val_mesh_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_mesh_construct(dx_val_mesh* SELF, dx_val_context* context, dx_assets_mesh* mesh_asset) {
  DX_CONSTRUCT_PREFIX(dx_val_mesh);

  SELF->mesh_asset = mesh_asset;
  DX_REFERENCE(mesh_asset);

  SELF->context = context;

  if (add_material_to_backend(SELF)) {
    DX_UNREFERENCE(SELF->mesh_asset);
    SELF->mesh_asset = NULL;
  }

  if (add_to_backend(SELF)) {
    remove_material_from_backend(SELF);
    DX_UNREFERENCE(SELF->mesh_asset);
    SELF->mesh_asset = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_mesh_create(dx_val_mesh** RETURN, dx_val_context* context, dx_assets_mesh* mesh_asset) {
  DX_CREATE_PREFIX(dx_val_mesh);
  if (dx_val_mesh_construct(SELF, context, mesh_asset)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
