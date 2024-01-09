#if !defined(DX_VAL_GL_BUFFER_H_INCLUDED)
#define DX_VAL_GL_BUFFER_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/buffer.h"
typedef struct dx_val_gl_context dx_val_gl_context;

Core_declareObjectType("dx.val.gl.buffer",
                       dx_val_gl_buffer,
                       dx_val_buffer);

static inline dx_val_gl_buffer* DX_VAL_GL_BUFFER(void* p) {
  return (dx_val_gl_buffer*)p;
}

struct dx_val_gl_buffer {
  dx_val_buffer _parent;
  GLuint id;
};

static inline dx_val_gl_buffer_Dispatch* DX_VAL_GL_BUFFER_DISPATCH(void* p) {
  return (dx_val_gl_buffer_Dispatch*)p;
}

struct dx_val_gl_buffer_Dispatch {
  dx_val_buffer_Dispatch _parent;
};

Core_Result dx_val_gl_buffer_create(dx_val_gl_buffer** RETURN, dx_val_gl_context* context);

#endif // DX_VAL_GL_BUFFER_H_INCLUDED
