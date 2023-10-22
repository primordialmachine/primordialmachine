#include "dx/adl/type_handler.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handler",
                      dx_adl_type_handler,
                      dx_object);

static void dx_adl_type_handler_destruct(dx_adl_type_handler* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handler_dispatch_construct(dx_adl_type_handler_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_adl_type_handler_construct(dx_adl_type_handler* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handler_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}