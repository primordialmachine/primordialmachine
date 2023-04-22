#include "dx/gl/vbinding.h"

#include "dx/gl/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.vbinding",
                      dx_gl_vbinding,
                      dx_val_vbinding);

static dx_result dx_gl_vbinding_activate(dx_gl_vbinding* SELF);

static dx_result dx_gl_vbinding_activate(dx_gl_vbinding* SELF) {
  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VAL_VBINDING(SELF)->context);
  ctx->glBindVertexArray(SELF->id);
  return DX_SUCCESS;
}

static int dx_gl_vbinding_construct(dx_gl_vbinding* SELF, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  dx_rti_type* TYPE = dx_gl_vbinding_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_vbinding_construct(DX_VAL_VBINDING(SELF), DX_VAL_BUFFER(buffer))) {
    return DX_FAILURE;
  }

  SELF->vertex_format = vertex_format;

  dx_gl_context* ctx = DX_GL_CONTEXT(DX_VAL_VBINDING(SELF)->context);

  ctx->glGetError();
  ctx->glGenVertexArrays(1, &SELF->id);
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  ctx->glBindVertexArray(SELF->id);
  // the vertex attribute 0 is activated and reads from the specified buffer data with the the specified format
  // https://www.khronos.org/opengl/wiki/Vertex_Specification
  ctx->glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
  switch (SELF->vertex_format) {
  case DX_VERTEX_FORMAT_POSITION_XYZ: {
    dx_size stride = 3 * sizeof(dx_f32);
    dx_size offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(dx_f32);
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA: {
    dx_size stride = 3 * sizeof(dx_f32) + 4 * sizeof(dx_f32);
    dx_size offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(dx_f32);
    
    ctx->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 4 * sizeof(dx_f32);
  } break;
  case DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV: {
    dx_size stride = 3 * sizeof(dx_f32) + 2 * sizeof(dx_f32);
    dx_size offset = 0;

    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(dx_f32);

    ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 2 * sizeof(dx_f32);
  } break;
  case DX_VERTEX_FORMAT_AMBIENT_RGBA:
  default: {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    ctx->glDeleteVertexArrays(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  } break;
  };
  if (GL_NO_ERROR != ctx->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    ctx->glDeleteVertexArrays(1, &SELF->id);
    SELF->id = 0;
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_gl_vbinding_destruct(dx_gl_vbinding* SELF) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_VAL_VBINDING(SELF)->context);
  if (SELF->id) {
    context->glDeleteVertexArrays(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_gl_vbinding_dispatch_construct(dx_gl_vbinding_dispatch* SELF) {
  DX_VAL_VBINDING_DISPATCH(SELF)->activate = (dx_result(*)(dx_val_vbinding*)) & dx_gl_vbinding_activate;
}

dx_result dx_gl_vbinding_create(dx_gl_vbinding** RETURN, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer) {
  dx_gl_vbinding* SELF = DX_GL_VBINDING(dx_object_alloc(sizeof(dx_gl_vbinding)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_gl_vbinding_construct(SELF, vertex_format, buffer)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
