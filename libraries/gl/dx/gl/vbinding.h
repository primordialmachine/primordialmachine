#if !defined(DX_GL_VBINDING_H_INCLUDED)
#define DX_GL_VBINDING_H_INCLUDED

#include "dx/gl/context.h"
#include "dx/val/vbinding.h"
typedef struct dx_gl_buffer dx_gl_buffer;

DX_DECLARE_OBJECT_TYPE("dx.gl.vbinding",
                       dx_gl_vbinding,
                       dx_val_vbinding)

static inline dx_gl_vbinding* DX_GL_VBINDING(void* p) {
  return (dx_gl_vbinding*)p;
}

struct dx_gl_vbinding {
  dx_val_vbinding _parent;
  DX_VERTEX_FORMAT vertex_format;
  GLuint id;
};

static inline dx_gl_vbinding_dispatch* DX_GL_VBINDING_DISPATCH(void* p) {
  return (dx_gl_vbinding_dispatch*)p;
}

struct dx_gl_vbinding_dispatch {
  dx_val_vbinding_dispatch _parent;
};

dx_result dx_gl_vbinding_create(dx_gl_vbinding** RETURN, DX_VERTEX_FORMAT vertex_format, dx_gl_buffer* buffer);

#endif // DX_GL_VBINDING_H_INCLUDED
