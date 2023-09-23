#include "dx/aal/system.h"

DX_DEFINE_OBJECT_TYPE("dx.aal.system",
                      dx_aal_system,
                      dx_system);

static void dx_aal_system_destruct(dx_aal_system* SELF)
{/*Intentionally empty.*/}

static void dx_aal_system_dispatch_construct(dx_aal_system_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_aal_system_construct(dx_aal_system* SELF, dx_msg_queue* msg_queue) {
  dx_rti_type* TYPE = dx_aal_system_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_system_construct(DX_SYSTEM(SELF))) {
    return DX_FAILURE;
  }
  SELF->msg_queue = msg_queue;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
