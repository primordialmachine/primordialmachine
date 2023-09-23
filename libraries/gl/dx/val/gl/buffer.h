#if !defined(DX_VAL_GL_BUFFER_H_INCLUDED)
#define DX_VAL_GL_BUFFER_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/buffer.h"
typedef struct dx_val_gl_context dx_val_gl_context;

DX_DECLARE_OBJECT_TYPE("dx.val.gl.buffer",
                       dx_val_gl_buffer,
                       dx_val_buffer);

static inline dx_val_gl_buffer* DX_VAL_GL_BUFFER(void* p) {
  return (dx_val_gl_buffer*)p;
}

struct dx_val_gl_buffer {
  dx_val_buffer _parent;
  GLuint id;
};

static inline dx_val_gl_buffer_dispatch* DX_VAL_GL_BUFFER_DISPATCH(void* p) {
  return (dx_val_gl_buffer_dispatch*)p;
}

struct dx_val_gl_buffer_dispatch {
  dx_val_buffer_dispatch _parent;
};

dx_result dx_val_gl_buffer_create(dx_val_gl_buffer** RETURN, dx_val_gl_context* context);

#endif // DX_VAL_GL_BUFFER_H_INCLUDED
