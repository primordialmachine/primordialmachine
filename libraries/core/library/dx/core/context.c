#include "dx/core/context.h"

Core_defineObjectType("dx.context",
                      dx_context,
                      Core_Object);

static void dx_context_destruct(dx_context* SELF)
{/*Intentionally empty.*/}

static void dx_context_constructDispatch(dx_context_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_context_construct(dx_context* SELF) {
  Core_BeginConstructor(dx_context);
  Core_EndConstructor(dx_context);
}
