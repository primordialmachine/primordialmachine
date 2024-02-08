#if !defined(CORE_VAL_GL_WGL_SYSTEMFACTORY_H_INCLUDED)
#define CORE_VAL_GL_WGL_SYSTEMFACTORY_H_INCLUDED

#include "dx/val/gl/system_factory.h"

Core_declareObjectType("Core.Val.Gl.Wgl.SystemFactory",
                       Core_Val_Gl_Wgl_SystemFactory,
                       dx_val_gl_system_factory);

static inline Core_Val_Gl_Wgl_SystemFactory* CORE_VAL_GL_WGL_SYSTEMFACTORY(void* p) {
  return (Core_Val_Gl_Wgl_SystemFactory*)p;
}

struct Core_Val_Gl_Wgl_SystemFactory {
  dx_val_gl_system_factory _parent;
};

static inline Core_Val_Gl_Wgl_SystemFactory_Dispatch* CORE_VAL_WGL_SYSTEMFACTORY_DISPATCH(void* p) {
  return (Core_Val_Gl_Wgl_SystemFactory_Dispatch*)p;
}

struct Core_Val_Gl_Wgl_SystemFactory_Dispatch {
  dx_val_gl_system_factory_Dispatch _parent;
};

/// @constructor{Core_Val_Gl_Wgl_SystemFactory}
Core_Result Core_Val_Gl_Wgl_SystemFactory_construct(Core_Val_Gl_Wgl_SystemFactory* SELF);

/// @create-operator{Core_Val_Gl_Wgl_SystemFactory}
Core_Result Core_Val_Gl_Wgl_SystemFactory_create(Core_Val_Gl_Wgl_SystemFactory** RETURN);

#endif // CORE_VAL_GL_WGL_SYSTEMFACTORY_H_INCLUDED
