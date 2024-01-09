#if !defined(DX_VAL_GL_VBINDING_H_INCLUDED)
#define DX_VAL_GL_VBINDING_H_INCLUDED

#include "dx/val/gl/gl-includes.i"
#include "dx/val/vbinding.h"
typedef struct dx_val_gl_context dx_val_gl_context;
typedef struct dx_val_gl_buffer dx_val_gl_buffer;

Core_declareObjectType("dx.val.gl.vbinding",
                       dx_val_gl_vbinding,
                       dx_val_vbinding);

static inline dx_val_gl_vbinding* DX_VAL_GL_VBINDING(void* p) {
  return (dx_val_gl_vbinding*)p;
}

struct dx_val_gl_vbinding {
  dx_val_vbinding _parent;
  Core_VertexFormat vertex_format;
  GLuint id;
};

static inline dx_val_gl_vbinding_Dispatch* DX_VAL_GL_VBINDING_DISPATCH(void* p) {
  return (dx_val_gl_vbinding_Dispatch*)p;
}

struct dx_val_gl_vbinding_Dispatch {
  dx_val_vbinding_Dispatch _parent;
};

Core_Result dx_val_gl_vbinding_create(dx_val_gl_vbinding** RETURN, Core_VertexFormat vertex_format, dx_val_gl_buffer* buffer);

#endif // DX_VAL_GL_VBINDING_H_INCLUDED
