#include "dx/core/system_factory.h"

Core_defineObjectType("dx.system_factory",
                      dx_system_factory,
                      Core_Object);

static void dx_system_factory_destruct(dx_system_factory* SELF)
{/*Intentionally empty.*/}

static void dx_system_factory_constructDispatch(dx_system_factory_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_system_factory_construct(dx_system_factory* SELF) {
  DX_CONSTRUCT_PREFIX(dx_system_factory);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
