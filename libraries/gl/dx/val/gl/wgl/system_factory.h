#if !defined(DX_VAL_GL_WGL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_VAL_GL_WGL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/val/gl/system_factory.h"

DX_DECLARE_OBJECT_TYPE("dx.val.gl.wgl.system_factory",
                       dx_val_gl_wgl_system_factory,
                       dx_val_gl_system_factory);

static inline dx_val_gl_wgl_system_factory* DX_VAL_GL_WGL_SYSTEM_FACTORY(void* p) {
  return (dx_val_gl_wgl_system_factory*)p;
}

struct dx_val_gl_wgl_system_factory {
  dx_val_gl_system_factory _parent;
};

static inline dx_val_gl_wgl_system_factory_dispatch* DX_VAL_GL_WGL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_val_gl_wgl_system_factory_dispatch*)p;
}

struct dx_val_gl_wgl_system_factory_dispatch {
  dx_val_gl_system_factory_dispatch _parent;
};

/// @brief Construct this VAL GL/WGL system factory.
/// @param SELF A pointer to this VAL GL/WGL system factory.
/// @method-call
dx_result dx_val_gl_wgl_system_factory_construct(dx_val_gl_wgl_system_factory* SELF);

dx_result dx_val_gl_wgl_system_factory_create(dx_val_gl_wgl_system_factory** RETURN);

#endif // DX_VAL_GL_WGL_SYSTEM_FACTORY_H_INCLUDED
