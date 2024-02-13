#include "dx/aal/system.h"

Core_defineObjectType("dx.aal.system",
                      dx_aal_system,
                      Core_System);

static void dx_aal_system_destruct(dx_aal_system* SELF)
{/*Intentionally empty.*/}

static void dx_aal_system_constructDispatch(dx_aal_system_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_aal_system_construct(dx_aal_system* SELF, Core_MessageQueue* msg_queue) {
  Core_BeginConstructor(dx_aal_system);
  if (Core_System_construct(CORE_SYSTEM(SELF))) {
    return Core_Failure;
  }
  SELF->msg_queue = msg_queue;
  Core_EndConstructor(dx_aal_system);
}
