#include "dx/gl/buffer.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.buffer",
                      dx_gl_buffer,
                      dx_val_buffer)

static dx_result dx_gl_buffer_set_data(dx_gl_buffer* SELF, void const* p, dx_size n);

static void dx_gl_buffer_destruct(dx_gl_buffer* SELF) {
  if (SELF->id) {
    dx_gl_context* context = DX_GL_CONTEXT(DX_VAL_BUFFER(SELF)->context);
    context->glDeleteBuffers(1, &SELF->id);
    SELF->id = 0;
  }
}

static void dx_gl_buffer_dispatch_construct(dx_gl_buffer_dispatch* SELF) {
  DX_VAL_BUFFER_DISPATCH(SELF)->set_data = (dx_result(*)(dx_val_buffer*, void const*, dx_size)) & dx_gl_buffer_set_data;
}

static dx_result dx_gl_buffer_construct(dx_gl_buffer* SELF, dx_gl_context* context) {
  dx_rti_type* TYPE = dx_gl_buffer_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_val_buffer_construct(DX_VAL_BUFFER(SELF), DX_VAL_CONTEXT(context))) {
    return DX_FAILURE;
  }
  context->glGetError(); // clear the error variable
  context->glGenBuffers(1, &SELF->id);
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static dx_result dx_gl_buffer_set_data(dx_gl_buffer* SELF, void const* p, dx_size n) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_VAL_BUFFER(SELF)->context);
  context->glGetError(); // clear the error variable
  context->glBindBuffer(GL_ARRAY_BUFFER, SELF->id);
  context->glBufferData(GL_ARRAY_BUFFER, n, p, GL_STATIC_DRAW);
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ERROR_ENVIRONMENT_FAILED);
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_gl_buffer_create(dx_gl_buffer** RETURN, dx_gl_context* context) {
  dx_gl_buffer* SELF = DX_GL_BUFFER(dx_object_alloc(sizeof(dx_gl_buffer)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_gl_buffer_construct(SELF, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
