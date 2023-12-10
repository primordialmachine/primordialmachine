#include "dx/core/context.h"

DX_DEFINE_OBJECT_TYPE("dx.context",
                      dx_context,
                      Core_Object);

static void dx_context_destruct(dx_context* SELF)
{/*Intentionally empty.*/}

static void dx_context_constructDispatch(dx_context_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_context_construct(dx_context* SELF) {
  DX_CONSTRUCT_PREFIX(dx_context);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
