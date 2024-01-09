#if !defined(DX_GL_WGL_CONTEXT_H_INCLUDED)
#define DX_GL_WGL_CONTEXT_H_INCLUDED

#include "dx/val/gl/context.h"
#include <GL/wglext.h>
typedef struct dx_val_gl_wgl_window dx_val_gl_wgl_window;

Core_declareObjectType("dx.gl.wgl.context",
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

static inline dx_gl_wgl_context_Dispatch* DX_GL_WGL_CONTEXT_DISPATCH(void* p) {
  return (dx_gl_wgl_context_Dispatch*)p;
}

struct dx_gl_wgl_context_Dispatch {
  dx_val_gl_context_Dispatch _parent;
};

/// @constructor{dx_gl_wgl_context}
/// @todo Make private to package.
Core_Result dx_gl_wgl_context_construct(dx_gl_wgl_context* SELF, dx_val_gl_wgl_window* window);

/// @create-operator{dx_gl_wgl_context}
/// @todo Scope to package. That is, move to "context.package.h".
Core_Result dx_gl_wgl_context_create(dx_gl_wgl_context** RETURN, dx_val_gl_wgl_window * window);

#endif // DX_GL_WGL_CONTEXT_H_INCLUDED
