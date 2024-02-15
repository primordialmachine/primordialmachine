#include "dx/val/gl/program.h"

// INT_MAX
#include <limits.h>

#include "dx/val/cbinding.h"
#include "dx/val/gl/context.h"

Core_defineObjectType("dx.val.gl.program",
                      dx_val_gl_program,
                      dx_val_program);

/// @brief Defined and equal to @a 1, then do not display shader logs to the user.
#define DX_VAL_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED (0)

// Create a fragment program or vertex program.
static Core_Result
create_shader
  (
    GLuint* id,
    dx_val_gl_context* ctx,
    GLenum type,
    Core_String* program_text
  );

static Core_Result
emit_log
  (
    dx_val_gl_context* ctx,
    GLuint id
  );

static Core_Result dx_val_gl_program_bind(dx_val_gl_program* SELF, dx_val_cbinding* cbinding);

static Core_Result dx_val_gl_program_activate(dx_val_gl_program* SELF);

static Core_Result dx_val_gl_program_construct(dx_val_gl_program* SELF,
                                               dx_val_gl_context* ctx,
                                               Core_String* vertex_program_text,
                                               Core_String* fragment_program_text);

static Core_Result
create_shader
  (
    GLuint* id,
    dx_val_gl_context* ctx,
    GLenum type,
    Core_String *program_text
  )
{
  if (!id || !ctx || (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) || !program_text) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (program_text->numberOfBytes > INT_MAX) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  GLuint id1 = ctx->glCreateShader(type);
  {
    Core_InlineArrayListN8 byte_array;
    Core_InlineArrayListN8_Configuration configuration = {
    .addedCallback = NULL,
    .removedCallback = NULL,
    };
    if (Core_InlineArrayListN8_initialize(&byte_array, 0, &configuration)) {
      ctx->glDeleteShader(id1);
      id1 = 0;
      return Core_Failure;
    }
    if (Core_InlineArrayListN8_appendMany(&byte_array, "#version 330 core\n", sizeof("#version 330 core\n") - 1)) {
      Core_InlineArrayListN8_uninitialize(&byte_array);
      ctx->glDeleteShader(id1);
      id1 = 0;
      return Core_Failure;
    }
    if (Core_InlineArrayListN8_appendMany(&byte_array, program_text->bytes, program_text->numberOfBytes)) {
      Core_InlineArrayListN8_uninitialize(&byte_array);
      ctx->glDeleteShader(id1);
      id1 = 0;
      return Core_Failure;
    }
    GLint const m[] = { (GLint)byte_array.size };
    GLchar const *q[] = { byte_array.elements };
    ctx->glShaderSource(id1, 1, q, m);
    Core_InlineArrayListN8_uninitialize(&byte_array);
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
    return Core_Failure;
  }
  *id = id1;
  return Core_Success;
}

static Core_Result
emit_log
  (
    dx_val_gl_context* ctx,
    GLuint id
  )
{
#if defined(DX_VAL_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED) && 1 == DX_VAL_GL_PROGRAM_WITH_LOG_EMISSION_DISABLED
  // Do not display shader logs to the user.
#else
  GLint l; // including the zero terminator
  ctx->glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);
  char* p = NULL;
  if (Core_Memory_allocate(&p, sizeof(char) * l)) {
    return Core_Failure;
  }
  ctx->glGetShaderInfoLog(id, l, NULL, p);
  dx_log(p, strlen(p));
  Core_Memory_deallocate(p);
#endif
  return Core_Success;
}

static Core_Result dx_val_gl_program_bind(dx_val_gl_program* SELF, dx_val_cbinding* cbinding) {
  dx_val_gl_context* ctx = DX_VAL_GL_CONTEXT(DX_VAL_PROGRAM(SELF)->ctx);

  ctx->glUseProgram(SELF->program_id);

  dx_val_cbinding_iter it;
  if (dx_val_cbinding_iter_initialize(&it, cbinding)) {
    return Core_Failure;
  }
  while (dx_val_cbinding_iter_has_entry(&it)) {
    Core_String* name = dx_val_cbinding_iter_get_name(&it);
    if (!name) {
      dx_val_cbinding_iter_uninitialize(&it);
      return Core_Failure;
    }
    char const* bytes = NULL;
    if (Core_String_getBytes(&bytes, name)) {
      dx_val_cbinding_iter_uninitialize(&it);
      return Core_Failure;
    }
    GLint location = ctx->glGetUniformLocation(SELF->program_id, bytes);
    CORE_UNREFERENCE(name);
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
        return Core_Failure;
      }
      ctx->glUniform3fv(location, 1, &(v.e[0]));
    } break;
    case DX_VAL_CBINDING_TYPE_VEC4: {
      DX_VEC4 v;
      if (dx_val_cbinding_iter_get_vec4(&it, &v)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return Core_Failure;
      }
      ctx->glUniform4fv(location, 1, &(v.e[0]));
    } break;
    case DX_VAL_CBINDING_TYPE_RGBA_F32: {
      Core_InlineRgbaR32 v;
      if (dx_val_cbinding_iter_get_rgba_f32(&it, &v)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return Core_Failure;
      }
      ctx->glUniform4fv(location, 1, (GLfloat const *)&v);
    } break;
    case DX_VAL_CBINDING_TYPE_MAT4: {
      DX_MAT4 a;
      if (dx_val_cbinding_iter_get_mat4(&it, &a)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return Core_Failure;
      }
      ctx->glUniformMatrix4fv(location, 1, GL_TRUE, &(a.e[0][0]));
    } break;
    case DX_VAL_CBINDING_TYPE_TEXTURE_INDEX: {
      Core_Size i;
      if (dx_val_cbinding_iter_get_texture_index(&it, &i)) {
        dx_val_cbinding_iter_uninitialize(&it);
        return Core_Failure;
      }
      GLint max_texture_units;
      ctx->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
      if (i >= max_texture_units) {
        dx_val_cbinding_iter_uninitialize(&it);
        return Core_Failure;
      }
      ctx->glUniform1i(location, (GLint)i);
    } break;
    case DX_VAL_CBINDING_TYPE_EMPTY: {
      if (Core_getError()) {
        dx_val_cbinding_iter_uninitialize(&it);
        return Core_Failure;
      }
    } break;
    default: {
      Core_setError(Core_Error_ArgumentInvalid);
      dx_val_cbinding_iter_uninitialize(&it);
      return Core_Failure;
    } break;
    };
    dx_val_cbinding_iter_next(&it);
  }
  dx_val_cbinding_iter_uninitialize(&it);
  if (Core_getError()) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result dx_val_gl_program_activate(dx_val_gl_program* SELF) {
  dx_val_gl_context* ctx = DX_VAL_GL_CONTEXT(DX_VAL_PROGRAM(SELF)->ctx);
  ctx->glUseProgram(SELF->program_id);
  return Core_Success;
}

static void dx_val_gl_program_destruct(dx_val_gl_program* SELF) {
  dx_val_gl_context* ctx = DX_VAL_GL_CONTEXT(DX_VAL_PROGRAM(SELF)->ctx);
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

static void dx_val_gl_program_constructDispatch(dx_val_gl_program_Dispatch* SELF) {
  DX_VAL_PROGRAM_DISPATCH(SELF)->bind = (Core_Result(*)(dx_val_program*, dx_val_cbinding*)) & dx_val_gl_program_bind;
  DX_VAL_PROGRAM_DISPATCH(SELF)->activate = (Core_Result(*)(dx_val_program*)) & dx_val_gl_program_activate;
}

static Core_Result dx_val_gl_program_construct(dx_val_gl_program* SELF,
                                               dx_val_gl_context* ctx,
                                               Core_String* vertex_program_text,
                                               Core_String* fragment_program_text) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_program);
  if (dx_val_program_construct(DX_VAL_PROGRAM(SELF), CORE_VISUALS_CONTEXT(ctx))) {
    return Core_Failure;
  }
  SELF->vertex_program_id = 0;
  SELF->fragment_program_id = 0;
  SELF->program_id = 0;

  if (create_shader(&SELF->vertex_program_id, ctx, GL_VERTEX_SHADER, vertex_program_text)) {
    return Core_Failure;
  }
  if (create_shader(&SELF->fragment_program_id, ctx, GL_FRAGMENT_SHADER, fragment_program_text)) {
    ctx->glDeleteShader(SELF->vertex_program_id);
    SELF->vertex_program_id = 0;
    return Core_Failure;
  }
  GLint success;
  {
    SELF->program_id = ctx->glCreateProgram();
    ctx->glAttachShader(SELF->program_id, SELF->vertex_program_id);
    ctx->glAttachShader(SELF->program_id, SELF->fragment_program_id);
    ctx->glLinkProgram(SELF->program_id);
    ctx->glGetProgramiv(SELF->program_id, GL_LINK_STATUS, &success);
    if (!success) {
      dx_log("failed to link program\n", sizeof("failed to link program program\n") - 1);
      ctx->glDeleteShader(SELF->fragment_program_id);
      SELF->fragment_program_id = 0;
      ctx->glDeleteShader(SELF->vertex_program_id);
      SELF->vertex_program_id = 0;
      return Core_Failure;
    }
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_gl_program_create(dx_val_gl_program** RETURN,
                                     dx_val_gl_context* ctx,
                                     Core_String* vertex_program_text,
                                     Core_String* fragment_program_text) {
  DX_CREATE_PREFIX(dx_val_gl_program);
  if (dx_val_gl_program_construct(SELF, ctx, vertex_program_text, fragment_program_text)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
