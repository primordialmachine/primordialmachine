#include "dx/core/scanner.h"

DX_DEFINE_OBJECT_TYPE("dx.scanner",
                      dx_scanner,
                      dx_object);

static void dx_scanner_destruct(dx_scanner* SELF)
{/*Intentionally empty.*/}

static void dx_scanner_dispatch_construct(dx_scanner_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_scanner_construct(dx_scanner* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_scanner_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
