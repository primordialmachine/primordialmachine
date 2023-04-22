#if !defined(DX_GL_BUFFER_H_INCLUDED)
#define DX_GL_BUFFER_H_INCLUDED

#include "dx/val/buffer.h"
#include "dx/gl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.gl.buffer",
                       dx_gl_buffer,
                       dx_val_buffer)

static inline dx_gl_buffer* DX_GL_BUFFER(void* p) {
  return (dx_gl_buffer*)p;
}

struct dx_gl_buffer {
  dx_val_buffer _parent;
  GLuint id;
};

static inline dx_gl_buffer_dispatch* DX_GL_BUFFER_DISPATCH(void* p) {
  return (dx_gl_buffer_dispatch*)p;
}

struct dx_gl_buffer_dispatch {
  dx_val_buffer_dispatch _parent;
};

dx_result dx_gl_buffer_create(dx_gl_buffer** RETURN, dx_gl_context* context);

#endif // DX_GL_BUFFER_H_INCLUDED
