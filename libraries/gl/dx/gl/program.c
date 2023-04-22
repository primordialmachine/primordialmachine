#include "dx/gl/program.h"

// INT_MAX
#include <limits.h>

#include "dx/val/cbinding.h"
#include "dx/gl/context.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.program",
                      dx_gl_program,
                      dx_val_program);

/// @brief Defined and equal to @a 1, then do not display shader logs to the user.
#define DX_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED (0)

// Create a fragment program or vertex program.
static int
create_shader
  (
    GLuint* id,
    dx_gl_context* ctx,
    GLenum type,
    dx_string* program_text
  );

static int
emit_log
  (
    dx_gl_context* ctx,
    GLuint id
  );

static dx_result dx_gl_program_bind(dx_gl_program* SELF, dx_val_cbinding* cbinding);

static dx_result dx_gl_program_activate(dx_gl_program* SELF);

static dx_result dx_gl_program_construct(dx_gl_program* SELF,
                                         dx_gl_context* ctx,
                                         dx_string* vertex_program_text,
                                         dx_string* fragment_program_text);

static int
create_shader
  (
    GLuint* id,
    dx_gl_context* ctx,
    GLenum type,
    dx_string *program_text
  )
{
  if (!id || !ctx || (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) || !program_text) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (program_text->number_of_bytes > INT_MAX) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  GLuint id1 = ctx->glCreateShader(type);
  {
    dx_inline_byte_array byte_array;
    if (dx_inline_byte_array_initialize(&byte_array)) {
      ctx->glDeleteShader(id1);
      id1 = 0;
      return DX_FAILURE;
    }
    if (dx_inline_byte_array_append(&byte_array, "#version 330 core\n", sizeof("#version 330 core\n") - 1)) {
      dx_inline_byte_array_uninitialize(&byte_array);
      ctx->glDeleteShader(id1);
      id1 = 0;
      return DX_FAILURE;
    }
    if (dx_inline_byte_array_append(&byte_array, program_text->bytes, program_text->number_of_bytes)) {
      dx_inline_byte_array_uninitialize(&byte_array);
      ctx->glDeleteShader(id1);
      id1 = 0;
      return DX_FAILURE;
    }
    GLint const m[] = { (GLint)byte_array.size };
    GLchar const *q[] = { byte_array.elements };
    ctx->glShaderSource(id1, 1, q, m);
    dx_inline_byte_array_uninitialize(&byte_array);
  }
  ctx->glCompileShader(id1);
  GLint success;
  ctx->glGetShaderiv(id1, GL_COMPILE_STATUS, &success);
  if (!success) {
    if (type == GL_VERTEX_SHADER) {
      dx_log("failed to compile vertex program\n", sizeof("failed to compile vertex program\n"));
    } else {
      dx_log("failed to compile fragment program\n", sizeof("failed to compile fragment program\n"));
    }
    emit_log(ctx, id1);
    ctx->glDeleteShader(id1);
    id1 = 0;
    return DX_FAILURE;
  }
  *id = id1;
  return DX_SUCCESS;
}

static int
emit_log
  (
    dx_gl_context* ctx,
    GLuint id
  )
{
#if defined(DX_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED) && 1 == DX_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED
  // Do not display shader logs to the user.
#else
  GLint l; // including the zero terminator
  ctx->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);
  char* p = NULL;
  if (dx_memory_allocate(&p, sizeof(char) * l)) {
    return DX_FAILURE;
  }
  ctx->glGetShaderInfoLog(id, l, NULL, p);
  dx_log(p, strlen(p));
  dx_memory_deallocate(p);
#endif
  return DX_SUCCESS;
}

static dx_result dx_gl_program_bind(dx_gl_program* SELF, dx_val_cbinding* cbinding) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VAL_PROGRAM(SELF)->ctx);

  ctx->glUseProgram(SELF->program_id);

  dx_val_cbinding_iter it;
  if (dx_val_cbinding_iter_initialize(&it, cbinding)) {
    return DX_FAILURE;
  }
  while (dx_val_cbinding_iter_has_entry(&it)) {
    dx_string* name = dx_val_cbinding_iter_get_name(&it);
    if (!name) {
      dx_val_cbinding_iter_uninitialize(&it);
      return DX_FAILURE;
    }
    char const* bytes = NULL;
    if (dx_string_get_bytes(&bytes, name)) {
      dx_val_cbinding_iter_uninitialize(&it);
      return DX_FAILURE;
    }
    GLint location = ctx->glGetUniformLocation(SELF->program_id, bytes);
    DX_UNREFERENCE(name);
    name = NULL;
    if (-1 == location) {
      dx_val_cbinding_iter_next(&it);
      continue;
    }
    uint8_t tag = dx_val_cbinding_iter_get_tag(&it);
    switch (tag) {
    case DX_VAL_CBINDING_TYPE_VEC3: {
      DX_VEC3 v;
      if (dx_val_cbinding_iter_get_vec3(&it, &v)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
      ctx->glUniform3fv(location, 1, &(v.e[0]));
    } break;
    case DX_VAL_CBINDING_TYPE_VEC4: {
      DX_VEC4 v;
      if (dx_val_cbinding_iter_get_vec4(&it, &v)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
      ctx->glUniform4fv(location, 1, &(v.e[0]));
    } break;
    case DX_VAL_CBINDING_TYPE_RGBA_F32: {
      DX_RGBA_F32 v;
      if (dx_val_cbinding_iter_get_rgba_f32(&it, &v)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
      ctx->glUniform4fv(location, 1, (GLfloat const *)&v);
    } break;
    case DX_VAL_CBINDING_TYPE_MAT4: {
      DX_MAT4 a;
      if (dx_val_cbinding_iter_get_mat4(&it, &a)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
      ctx->glUniformMatrix4fv(location, 1, GL_TRUE, &(a.e[0][0]));
    } break;
    case DX_VAL_CBINDING_TYPE_TEXTURE_INDEX: {
      dx_size i;
      if (dx_val_cbinding_iter_get_texture_index(&it, &i)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
      GLint max_texture_units;
      ctx->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
      if (i >= max_texture_units) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
      ctx->glUniform1i(location, (GLint)i);
    } break;
    case DX_VAL_CBINDING_TYPE_EMPTY: {
      if (dx_get_error()) {
        dx_val_cbinding_iter_uninitialize(&it);
        return DX_FAILURE;
      }
    } break;
    default: {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      dx_val_cbinding_iter_uninitialize(&it);
      return DX_FAILURE;
    } break;
    };
    dx_val_cbinding_iter_next(&it);
  }
  dx_val_cbinding_iter_uninitialize(&it);
  if (dx_get_error()) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result dx_gl_program_activate(dx_gl_program* SELF) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VAL_PROGRAM(SELF)->ctx);
  ctx->glUseProgram(SELF->program_id);
  return DX_SUCCESS;
}

static void dx_gl_program_destruct(dx_gl_program* SELF) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VAL_PROGRAM(SELF)->ctx);
  if (SELF->program_id) {
    ctx->glDeleteProgram(SELF->program_id);
    SELF->program_id = 0;
  }

  if (SELF->fragment_program_id) {
    ctx->glDeleteShader(SELF->fragment_program_id);
    SELF->fragment_program_id = 0;
  }

  if (SELF->vertex_program_id) {
    ctx->glDeleteShader(SELF->vertex_program_id);
    SELF->vertex_program_id = 0;
  }
}

static void dx_gl_program_dispatch_construct(dx_gl_program_dispatch* SELF) {
  DX_VAL_PROGRAM_DISPATCH(SELF)->bind = (dx_result(*)(dx_val_program*, dx_val_cbinding*)) & dx_gl_program_bind;
  DX_VAL_PROGRAM_DISPATCH(SELF)->activate = (dx_result(*)(dx_val_program*)) & dx_gl_program_activate;
}

static dx_result dx_gl_program_construct(dx_gl_program* SELF,
                                         dx_gl_context* ctx,
                                         dx_string* vertex_program_text,
                                         dx_string* fragment_program_text)
{
  dx_rti_type* TYPE = dx_gl_program_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_program_construct(DX_VAL_PROGRAM(SELF), DX_VAL_CONTEXT(ctx))) {
    return DX_FAILURE;
  }
  SELF->vertex_program_id = 0;
  SELF->fragment_program_id = 0;
  SELF->program_id = 0;

  if (create_shader(&SELF->vertex_program_id, ctx, GL_VERTEX_SHADER, vertex_program_text)) {
    return DX_FAILURE;
  }
  if (create_shader(&SELF->fragment_program_id, ctx, GL_FRAGMENT_SHADER, fragment_program_text)) {
    ctx->glDeleteShader(SELF->vertex_program_id);
    SELF->vertex_program_id = 0;
    return DX_FAILURE;
  }
  GLint success;
  {
    SELF->program_id = ctx->glCreateProgram();
    ctx->glAttachShader(SELF->program_id, SELF->vertex_program_id);
    ctx->glAttachShader(SELF->program_id, SELF->fragment_program_id);
    ctx->glLinkProgram(SELF->program_id);
    ctx->glGetProgramiv(SELF->program_id, GL_LINK_STATUS, &success);
    if (!success) {
      dx_log("failed to link program\n", sizeof("failed to link program program\n"));
      ctx->glDeleteShader(SELF->fragment_program_id);
      SELF->fragment_program_id = 0;
      ctx->glDeleteShader(SELF->vertex_program_id);
      SELF->vertex_program_id = 0;
      return DX_FAILURE;
    }
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_gl_program_create(dx_gl_program** RETURN,
                               dx_gl_context* ctx,
                               dx_string* vertex_program_text,
                               dx_string* fragment_program_text)
{
  dx_gl_program* SELF = DX_GL_PROGRAM(dx_object_alloc(sizeof(dx_gl_program)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_gl_program_construct(SELF, ctx, vertex_program_text, fragment_program_text)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
