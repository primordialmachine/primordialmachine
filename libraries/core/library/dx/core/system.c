#include "dx/core/system.h"

DX_DEFINE_OBJECT_TYPE("dx.system",
                      dx_system,
                      Core_Object);

static void dx_system_destruct(dx_system* SELF)
{/*Intentionally empty.*/}

static void dx_system_constructDispatch(dx_system_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_system_construct(dx_system* SELF) {
  DX_CONSTRUCT_PREFIX(dx_system);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
