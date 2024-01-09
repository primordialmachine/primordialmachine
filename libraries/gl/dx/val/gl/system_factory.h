#if !defined(DX_VAL_GL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_VAL_GL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/val/system_factory.h"

Core_declareObjectType("dx.val.gl.system_factory",
                       dx_val_gl_system_factory,
                       dx_val_system_factory);

static inline dx_val_gl_system_factory* DX_VAL_GL_SYSTEM_FACTORY(void* p) {
  return (dx_val_gl_system_factory*)p;
}

struct dx_val_gl_system_factory {
  dx_val_system_factory _parent;
};

static inline dx_val_gl_system_factory_Dispatch* DX_VAL_GL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_val_gl_system_factory_Dispatch*)p;
}

struct dx_val_gl_system_factory_Dispatch {
  dx_val_system_factory_Dispatch _parent;
};

/// @constructor{dx_val_gl_system_factory}
Core_Result dx_val_gl_system_factory_construct(dx_val_gl_system_factory* SELF);

#endif // DX_VAL_GL_SYSTEM_FACTORY_H_INCLUDED
