#include "dx/aal/context.h"

DX_DEFINE_OBJECT_TYPE("dx.aal.context",
                      dx_aal_context,
                      dx_context);

static void dx_aal_context_destruct(dx_aal_context* SELF)
{/*Intentionally empty.*/}

static void dx_aal_context_constructDispatch(dx_aal_context_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_aal_context_construct(dx_aal_context* SELF) {
  DX_CONSTRUCT_PREFIX(dx_aal_context);
  if (dx_context_construct(DX_CONTEXT(SELF))) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
