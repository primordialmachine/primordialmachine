#include "dx/val/gl/wgl/system_factory.h"

#include "dx/application.h"
#include "dx/val/gl/wgl/system.h"

Core_defineObjectType("Val.Gl.Wgl.SystemFactory",
                      Core_Val_Gl_Wgl_SystemFactory,
                      dx_val_gl_system_factory);

static Core_Result createSystem(Core_Val_Gl_Wgl_System** RETURN, Core_Val_Gl_Wgl_SystemFactory* SELF, Core_MessageQueue* msg_queue);

static Core_Result createSystem(Core_Val_Gl_Wgl_System** RETURN, Core_Val_Gl_Wgl_SystemFactory* SELF, Core_MessageQueue* msg_queue) {
  return Core_Val_Gl_Wgl_System_create(RETURN, msg_queue);
}

static void Core_Val_Gl_Wgl_SystemFactory_destruct(Core_Val_Gl_Wgl_SystemFactory* SELF)
{/*Intentionally empty.*/}

static void Core_Val_Gl_Wgl_SystemFactory_constructDispatch(Core_Val_Gl_Wgl_SystemFactory_Dispatch* SELF) {
  CORE_SYSTEMFACTORY_DISPATCH(SELF)->createSystem = (Core_Result(*)(Core_System**, Core_SystemFactory*, Core_MessageQueue*)) & createSystem;
}

Core_Result Core_Val_Gl_Wgl_SystemFactory_construct(Core_Val_Gl_Wgl_SystemFactory* SELF) {
  Core_BeginConstructor(Core_Val_Gl_Wgl_SystemFactory);
  if (dx_val_gl_system_factory_construct(DX_VAL_GL_SYSTEM_FACTORY(SELF))) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Val_Gl_Wgl_SystemFactory);
}

Core_Result Core_Val_Gl_Wgl_SystemFactory_create(Core_Val_Gl_Wgl_SystemFactory** RETURN) {
  DX_CREATE_PREFIX(Core_Val_Gl_Wgl_SystemFactory);
  if (Core_Val_Gl_Wgl_SystemFactory_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
