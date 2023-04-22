#include "dx/core/error.h"

#include "dx/core/core.h"

DX_THREAD_LOCAL() dx_error g_error = DX_NO_ERROR;

dx_error dx_get_error() {
  return g_error;
}

void dx_set_error(dx_error error) {
  g_error = error;
}
