#include "dx/sal/context.h"

DX_DEFINE_OBJECT_TYPE("dx.sal.context",
                      dx_sal_context,
                      dx_object)

static void dx_sal_context_destruct(dx_sal_context* SELF)
{/*Intentionally empty.*/}

static void dx_sal_context_dispatch_construct(dx_sal_context_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_sal_context_construct(dx_sal_context* SELF) {
  dx_rti_type* TYPE = dx_sal_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
