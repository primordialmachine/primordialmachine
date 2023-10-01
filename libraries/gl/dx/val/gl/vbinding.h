#if !defined(DX_VAL_GL_VBINDING_H_INCLUDED)
#define DX_VAL_GL_VBINDING_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/vbinding.h"
typedef struct dx_val_gl_context dx_val_gl_context;
typedef struct dx_val_gl_buffer dx_val_gl_buffer;

DX_DECLARE_OBJECT_TYPE("dx.val.gl.vbinding",
                       dx_val_gl_vbinding,
                       dx_val_vbinding)

static inline dx_val_gl_vbinding* DX_VAL_GL_VBINDING(void* p) {
  return (dx_val_gl_vbinding*)p;
}

struct dx_val_gl_vbinding {
  dx_val_vbinding _parent;
  dx_vertex_format vertex_format;
  GLuint id;
};

static inline dx_val_gl_vbinding_dispatch* DX_VAL_GL_VBINDING_DISPATCH(void* p) {
  return (dx_val_gl_vbinding_dispatch*)p;
}

struct dx_val_gl_vbinding_dispatch {
  dx_val_vbinding_dispatch _parent;
};

dx_result dx_val_gl_vbinding_create(dx_val_gl_vbinding** RETURN, dx_vertex_format vertex_format, dx_val_gl_buffer* buffer);

#endif // DX_VAL_GL_VBINDING_H_INCLUDED
