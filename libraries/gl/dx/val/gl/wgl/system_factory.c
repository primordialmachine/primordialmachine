#include "dx/val/gl/wgl/system_factory.h"

#include "dx/application.h"
#include "dx/val/gl/wgl/system.h"

Core_defineObjectType("dx.val.gl.wgl.system_factory",
                      dx_val_gl_wgl_system_factory,
                      dx_val_gl_system_factory);

static Core_Result create_system(dx_val_gl_wgl_system** RETURN, dx_val_gl_wgl_system_factory* SELF, dx_msg_queue* msg_queue);

static Core_Result create_system(dx_val_gl_wgl_system** RETURN, dx_val_gl_wgl_system_factory* SELF, dx_msg_queue* msg_queue) {
  return dx_val_gl_wgl_system_create(RETURN, msg_queue);
}

static void dx_val_gl_wgl_system_factory_destruct(dx_val_gl_wgl_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_val_gl_wgl_system_factory_constructDispatch(dx_val_gl_wgl_system_factory_Dispatch* SELF) {
  DX_SYSTEM_FACTORY_DISPATCH(SELF)->create_system = (Core_Result(*)(dx_system**, dx_system_factory*, dx_msg_queue*)) & create_system;
}

Core_Result dx_val_gl_wgl_system_factory_construct(dx_val_gl_wgl_system_factory* SELF) {
  DX_CONSTRUCT_PREFIX(dx_val_gl_wgl_system_factory);
  if (dx_val_gl_system_factory_construct(DX_VAL_GL_SYSTEM_FACTORY(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_val_gl_wgl_system_factory_create(dx_val_gl_wgl_system_factory** RETURN) {
  DX_CREATE_PREFIX(dx_val_gl_wgl_system_factory);
  if (dx_val_gl_wgl_system_factory_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
