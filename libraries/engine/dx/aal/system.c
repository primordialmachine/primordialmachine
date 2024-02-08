#include "dx/aal/system.h"

Core_defineObjectType("dx.aal.system",
                      dx_aal_system,
                      dx_system);

static void dx_aal_system_destruct(dx_aal_system* SELF)
{/*Intentionally empty.*/}

static void dx_aal_system_constructDispatch(dx_aal_system_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_aal_system_construct(dx_aal_system* SELF, Core_MessageQueue* msg_queue) {
  DX_CONSTRUCT_PREFIX(dx_aal_system);
  if (dx_system_construct(DX_SYSTEM(SELF))) {
    return Core_Failure;
  }
  SELF->msg_queue = msg_queue;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
