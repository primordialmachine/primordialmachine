#include "dx/val/gl/buffer.h"

#include "dx/val/gl/context.h"

Core_defineObjectType("dx.val.gl.buffer",
                      dx_val_gl_buffer,
                      dx_val_buffer);

static Core_Result dx_val_gl_buffer_set_data(dx_val_gl_buffer* SELF, void const* p, Core_Size n);

static void dx_val_gl_buffer_destruct(dx_val_gl_buffer* SELF) {
  if (SELF->id) {
    dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_BUFFER(SELF)->context);
    context->glDeleteBuffers(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_val_gl_buffer_constructDispatch(dx_val_gl_buffer_Dispatch* SELF) {
  DX_VAL_BUFFER_DISPATCH(SELF)->set_data = (Core_Result(*)(dx_val_buffer*, void const*, Core_Size)) & dx_val_gl_buffer_set_data;
}

static Core_Result dx_val_gl_buffer_construct(dx_val_gl_buffer* SELF, dx_val_gl_context* context) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_buffer);
  if (dx_val_buffer_construct(DX_VAL_BUFFER(SELF), DX_VAL_CONTEXT(context))) {
    return Core_Failure;
  }
  context->glGetError(); // clear the error variable
  context->glGenBuffers(1, &SELF->id);
  if (GL_NO_ERROR != context->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static Core_Result dx_val_gl_buffer_set_data(dx_val_gl_buffer* SELF, void const* p, Core_Size n) {
  dx_val_gl_context* context = DX_VAL_GL_CONTEXT(DX_VAL_BUFFER(SELF)->context);
  context->glGetError(); // clear the error variable
  context->glBindBuffer(GL_ARRAY_BUFFER, SELF->id);
  context->glBufferData(GL_ARRAY_BUFFER, n, p, GL_STATIC_DRAW);
  if (GL_NO_ERROR != context->glGetError()) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_val_gl_buffer_create(dx_val_gl_buffer** RETURN, dx_val_gl_context* context) {
  DX_CREATE_PREFIX(dx_val_gl_buffer);
  if (dx_val_gl_buffer_construct(SELF, context)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
