#include "dx/val/gl/system.h"

Core_defineObjectType("Core.Val.Gl.System",
                      Core_Val_Gl_System,
                      Core_Visuals_System);

static void Core_Val_Gl_System_destruct(Core_Val_Gl_System* SELF)
{/*Intentionally empty.*/}

static void Core_Val_Gl_System_constructDispatch(Core_Val_Gl_System_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_Val_Gl_System_construct(Core_Val_Gl_System* SELF, Core_MessageQueue* msg_queue) {
  Core_BeginConstructor(Core_Val_Gl_System);
  if (Core_Visuals_System_construct(CORE_VISUALS_SYSTEM(SELF), msg_queue)) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Val_Gl_System);
}
