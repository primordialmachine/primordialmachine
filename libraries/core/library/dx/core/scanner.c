#include "dx/core/scanner.h"

Core_defineObjectType("dx.scanner",
                      dx_scanner,
                      Core_Object);

static void dx_scanner_destruct(dx_scanner* SELF)
{/*Intentionally empty.*/}

static void dx_scanner_constructDispatch(dx_scanner_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_scanner_construct(dx_scanner* SELF) {
  DX_CONSTRUCT_PREFIX(dx_scanner);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
