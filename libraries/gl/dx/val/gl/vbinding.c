#include "dx/val/gl/vbinding.h"

#include "dx/val/gl/buffer.h"
#include "dx/val/gl/context.h"

Core_defineObjectType("dx.val.gl.vbinding",
                      dx_val_gl_vbinding,
                      dx_val_vbinding);

static Core_Result dx_val_gl_vbinding_activate(dx_val_gl_vbinding* SELF);

static Core_Result dx_val_gl_vbinding_activate(dx_val_gl_vbinding* SELF) {
  dx_val_gl_context* ctx = DX_VAL_GL_CONTEXT(DX_VAL_VBINDING(SELF)->context);
  ctx->glBindVertexArray(SELF->id);
  return Core_Success;
}

static Core_Result dx_val_gl_vbinding_construct(dx_val_gl_vbinding* SELF, Core_VertexFormat vertex_format, dx_val_gl_buffer* buffer) {
  Core_BeginConstructor(dx_val_gl_vbinding);
  if (dx_val_vbinding_construct(DX_VAL_VBINDING(SELF), DX_VAL_BUFFER(buffer))) {
    return Core_Failure;
  }

  SELF->vertex_format = vertex_format;

  dx_val_gl_context* ctx = DX_VAL_GL_CONTEXT(DX_VAL_VBINDING(SELF)->context);

  ctx->glGetError();
  ctx->glGenVertexArrays(1, &SELF->id);
  if (GL_NO_ERROR != ctx->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  ctx->glBindVertexArray(SELF->id);
  // the vertex attribute 0 is activated and reads from the specified buffer data with the the specified format
  // https://www.khronos.org/opengl/wiki/Vertex_Specification
  ctx->glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
  switch (SELF->vertex_format) {
  case Core_VertexFormat_position_xyz: {
    Core_Size stride = 3 * sizeof(Core_Real32);
    Core_Size offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(Core_Real32);
  } break;
  case Core_VertexFormat_position_xyz_ambient_rgba: {
    Core_Size stride = 3 * sizeof(Core_Real32) + 4 * sizeof(Core_Real32);
    Core_Size offset = 0;
    
    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(Core_Real32);
    
    ctx->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 4 * sizeof(Core_Real32);
  } break;
  case Core_VertexFormat_position_xyz_ambient_uv: {
    Core_Size stride = 3 * sizeof(Core_Real32) + 2 * sizeof(Core_Real32);
    Core_Size offset = 0;

    ctx->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(0);
    offset += 3 * sizeof(Core_Real32);

    ctx->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
    ctx->glEnableVertexAttribArray(1);
    offset += 2 * sizeof(Core_Real32);
  } break;
  case Core_VertexFormat_ambient_rgba:
  default: {
    Core_setError(Core_Error_ArgumentInvalid);
    ctx->glDeleteVertexArrays(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  } break;
  };
  if (GL_NO_ERROR != ctx->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    ctx->glDeleteVertexArrays(1, &SELF->id);
    SELF->id = 0;
    return Core_Failure;
  }
  Core_EndConstructor(dx_val_gl_vbinding);
}

static void dx_val_gl_vbinding_destruct(dx_val_gl_vbinding* SELF) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_VBINDING(SELF)->context);
  if (SELF->id) {
    context->glDeleteVertexArrays(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_val_gl_vbinding_constructDispatch(dx_val_gl_vbinding_Dispatch* SELF) {
  DX_VAL_VBINDING_DISPATCH(SELF)->activate = (Core_Result(*)(dx_val_vbinding*)) & dx_val_gl_vbinding_activate;
}

Core_Result dx_val_gl_vbinding_create(dx_val_gl_vbinding** RETURN, Core_VertexFormat vertex_format, dx_val_gl_buffer* buffer) {
  DX_CREATE_PREFIX(dx_val_gl_vbinding);
  if (dx_val_gl_vbinding_construct(SELF, vertex_format, buffer)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
