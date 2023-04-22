#include "dx/console.h"

DX_DEFINE_OBJECT_TYPE("dx.console",
                      dx_console,
                      dx_object);

static void dx_console_destruct(dx_console* SELF)
{/*Intentionally empty.*/}

static void dx_console_dispatch_construct(dx_console_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_console_construct(dx_console* SELF) {
  dx_rti_type* TYPE = dx_console_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
