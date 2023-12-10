#include "dx/console.h"

DX_DEFINE_OBJECT_TYPE("dx.console",
                      dx_console,
                      Core_Object);

static void dx_console_destruct(dx_console* SELF)
{/*Intentionally empty.*/}

static void dx_console_constructDispatch(dx_console_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_console_construct(dx_console* SELF) {
  DX_CONSTRUCT_PREFIX(dx_console);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
