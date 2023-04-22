#include "dx/val/mesh.h"

// snprintf
#include <stdio.h>
#include "dx/val/program_text.h"

DX_DEFINE_OBJECT_TYPE("dx.val.mesh",
                      dx_val_mesh,
                      dx_object)

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
static dx_val_program_text* load_program(dx_string* path, dx_string* filename, uint8_t flags);

static dx_result add_material_to_backend(dx_val_mesh* self);

static void remove_material_from_backend(dx_val_mesh* self);

static dx_result add_to_backend(dx_val_mesh* self);

static void remove_from_backend(dx_val_mesh* self);

static dx_val_program_text* load_program(dx_string* path, dx_string* filename, uint8_t flags) {
  dx_val_program_text* vertex_program = NULL, * fragment_program = NULL;
  dx_val_program_text* program = NULL;
  {
    dx_string* format = NULL;
    if (dx_string_create(&format, "${s}/${s}.vs", sizeof("${s}/${s}.vs") - 1)) {
      goto on_error;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      goto on_error;
    }
    if (dx_val_program_text_create_from_file(&vertex_program, DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, p)) {
      DX_UNREFERENCE(p);
      p = NULL;
      goto on_error;
    }
    DX_UNREFERENCE(p);
    p = NULL;
  }
  {
    dx_string* format = NULL;
    if (dx_string_create(&format, "${s}/${s}.fs", sizeof("${s}/${s}.fs") - 1)) {
      goto on_error;
    }
    dx_string* p = dx_string_printf(format, path, filename);
    DX_UNREFERENCE(format);
    format = NULL;
    if (!p) {
      goto on_error;
    }
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
      dx_string* name = NULL;
      if (dx_string_create(&name, "WITH_MESH_AMBIENT_RGBA", sizeof("WITH_MESH_AMBIENT_RGBA") - 1)) {
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
      dx_string* name = NULL;
      if (dx_string_create(&name, "WITH_VERTEX_AMBIENT_RGBA", sizeof("WITH_VERTEX_AMBIENT_RGBA") - 1)) {
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
      dx_string* name = NULL;
      if (dx_string_create(&name, "WITH_VERTEX_AMBIENT_UV", sizeof("WITH_VERTEX_AMBIENT_UV") - 1)) {
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
      dx_string* name = NULL;
      if (dx_string_create(&name, "WITH_MATERIAL_AMBIENT_TEXTURE", sizeof("WITH_MATERIAL_AMBIENT_TEXTURE") - 1)) {
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

static dx_result add_material_to_backend(dx_val_mesh* self) {
  if (self->asset_mesh->material_reference && self->asset_mesh->material_reference->object) {
    if (dx_val_material_create(&self->material, self->context, DX_ASSET_MATERIAL(self->asset_mesh->material_reference->object))) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static void remove_material_from_backend(dx_val_mesh* self) {
  if (self->material) {
    DX_UNREFERENCE(self->material);
    self->material = NULL;
  }
}

static dx_result add_to_backend(dx_val_mesh* SELF) {
  DX_VERTEX_FORMAT vertex_format = SELF->asset_mesh->vertex_format;

  // create buffer
  if (dx_val_context_create_buffer(&SELF->buffer, SELF->context)) {
    return DX_FAILURE;
  }

  // upload data to buffer
  void* bytes; dx_size number_of_bytes;
  if (dx_asset_mesh_format(SELF->asset_mesh, vertex_format, &bytes, &number_of_bytes)) {
    return DX_FAILURE;
  }
  if (dx_val_buffer_set_data(SELF->buffer, bytes, number_of_bytes)) {
    dx_memory_deallocate(bytes);
    bytes = NULL;
    return DX_FAILURE;
  }
  dx_memory_deallocate(bytes);
  bytes = NULL;

  // create variable binding
  if (dx_val_context_create_vbinding(&SELF->vbinding, SELF->context, vertex_format, SELF->buffer)) {
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return DX_FAILURE;
  }

  // create the program
  uint8_t flags = DX_PROGRAM_WITH_MESH_AMBIENT_RGBA;
  switch (vertex_format) {
  case DX_VERTEX_FORMAT_POSITION_XYZ: {
    /*Intentionally empty.*/
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA: {
    flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_RGBA;
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV: {
   flags |= DX_PROGRAM_WITH_VERTEX_AMBIENT_UV;
   if (SELF->material->asset_material->ambient_texture_reference && SELF->material->asset_material->ambient_texture_reference->object) {
      flags |= DX_PROGRAM_WITH_MATERIAL_AMBIENT_TEXTURE;
    }
  } break;
  default: {
    DX_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return DX_FAILURE;
  } break;
  };
  dx_string* path = NULL;
  if (dx_string_create(&path, "assets/gl", sizeof("assets/gl") - 1)) {
    return DX_FAILURE;
  }
  dx_string* filename = NULL;
  if (dx_string_create(&filename, "3", sizeof("3") - 1)) {
    DX_UNREFERENCE(path);
    path = NULL;
    return DX_FAILURE;
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
    return DX_FAILURE;
  }
  if (dx_val_context_create_program(&SELF->program, SELF->context, program_text)) {
    DX_UNREFERENCE(program_text);
    program_text = NULL;
    DX_UNREFERENCE(SELF->vbinding);
    SELF->vbinding = NULL;
    DX_UNREFERENCE(SELF->buffer);
    SELF->buffer = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(program_text);
  program_text = NULL;
  return DX_SUCCESS;
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
  DX_UNREFERENCE(SELF->asset_mesh);
  SELF->asset_mesh = NULL;
}

static void dx_val_mesh_dispatch_construct(dx_val_mesh_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_mesh_construct(dx_val_mesh* SELF, dx_val_context* context, dx_asset_mesh* asset_mesh) {
  dx_rti_type* TYPE = dx_val_mesh_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  SELF->asset_mesh = asset_mesh;
  DX_REFERENCE(asset_mesh);

  SELF->context = context;

  if (add_material_to_backend(SELF)) {
    DX_UNREFERENCE(SELF->asset_mesh);
    SELF->asset_mesh = NULL;
  }

  if (add_to_backend(SELF)) {
    remove_material_from_backend(SELF);
    DX_UNREFERENCE(SELF->asset_mesh);
    SELF->asset_mesh = NULL;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_val_mesh* dx_val_mesh_create(dx_val_context* context, dx_asset_mesh* asset_mesh) {
  dx_rti_type* TYPE = dx_val_mesh_get_type();
  if (!TYPE) {
    return NULL;
  }
  dx_val_mesh* SELF = DX_VAL_MESH(dx_object_alloc(sizeof(dx_val_mesh)));
  if (!SELF) {
    return NULL;
  }
  if (dx_val_mesh_construct(SELF, context, asset_mesh)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return NULL;
  }
  return SELF;
}
