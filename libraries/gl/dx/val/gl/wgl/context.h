#if !defined(DX_GL_WGL_CONTEXT_H_INCLUDED)
#define DX_GL_WGL_CONTEXT_H_INCLUDED

#include "dx/val/gl/context.h"
#include <GL/wglext.h> /// @todo Should be removed. This is about GL not WGL.
typedef struct dx_val_gl_wgl_window dx_val_gl_wgl_window;

DX_DECLARE_OBJECT_TYPE("dx.gl.wgl.context",
                       dx_gl_wgl_context,
                       dx_val_gl_context);

static inline dx_gl_wgl_context* DX_GL_WGL_CONTEXT(void* p) {
  return (dx_gl_wgl_context*)p;
}

struct dx_gl_wgl_context {
  dx_val_gl_context _parent;
  dx_val_gl_wgl_window* window;
#define DEFINE(TYPE, NAME, EXTENSION_NAME) TYPE NAME;
#include "dx/val/gl/wgl/functions.i"
#undef DEFINE
};

static inline dx_gl_wgl_context_dispatch* DX_GL_WGL_CONTEXT_DISPATCH(void* p) {
  return (dx_gl_wgl_context_dispatch*)p;
}

struct dx_gl_wgl_context_dispatch {
  dx_val_gl_context_dispatch _parent;
};

dx_result dx_gl_wgl_context_construct(dx_gl_wgl_context* SELF, dx_val_gl_wgl_window* window);

dx_result dx_gl_wgl_context_create(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_window * window);

#endif // DX_GL_WGL_CONTEXT_H_INCLUDED
