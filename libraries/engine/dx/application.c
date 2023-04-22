#include "dx/application.h"

DX_DEFINE_OBJECT_TYPE("dx.application",
                      dx_application,
                      dx_object);

static void dx_application_destruct(dx_application* SELF)
{/*Intentionally empty.*/}

static void dx_application_dispatch_construct(dx_application_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_application_construct(dx_application* SELF) {
  dx_rti_type* TYPE = dx_application_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
