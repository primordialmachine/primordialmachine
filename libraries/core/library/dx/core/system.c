#include "dx/core/system.h"

DX_DEFINE_OBJECT_TYPE("dx.system",
                      dx_system,
                      dx_object);

static void dx_system_destruct(dx_system* SELF)
{/*Intentionally empty.*/}

static void dx_system_dispatch_construct(dx_system_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_system_construct(dx_system* SELF) {
  dx_rti_type* TYPE = dx_system_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
