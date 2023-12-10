#include "dx/adl/type_handler.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handler",
                      dx_adl_type_handler,
                      Core_Object);

static void dx_adl_type_handler_destruct(dx_adl_type_handler* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handler_constructDispatch(dx_adl_type_handler_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_adl_type_handler_construct(dx_adl_type_handler* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handler);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
