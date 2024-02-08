#include "dx/core/system.h"

Core_defineObjectType("dx.system",
                      dx_system,
                      Core_Object);

static void dx_system_destruct(dx_system* SELF)
{/*Intentionally empty.*/}

static void dx_system_constructDispatch(dx_system_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_system_construct(dx_system* SELF) {
  Core_BeginConstructor(dx_system);
  Core_EndConstructor(dx_system);
}
