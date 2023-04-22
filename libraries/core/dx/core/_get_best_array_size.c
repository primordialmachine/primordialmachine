#include "dx/core/_get_best_array_size.h"

#include "dx/core/next_power_of_two.h"

dx_result dx_get_best_array_size(dx_size* RETURN, dx_size current, dx_size additional, dx_size least, dx_size greatest, dx_bool saturate) {
  dx_error old_error = dx_get_error();
  if (least > greatest) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (DX_SIZE_GREATEST - current < additional) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size new = current;
  if (new < least) {
    new = least;
  }
  new = current + additional;
  {
    dx_size temporary = dx_next_power_of_two_gte_sz(new);
    if (dx_get_error()) {
      // no power of two equal to or greater than new is found
      // new is not a power of two and there is no next power of two greater than new
      dx_set_error(old_error);
    } else {
      new = temporary;
    }
  }
  if (new > greatest) {
    if (!saturate) {
      dx_set_error(DX_ERROR_NOT_FOUND);
      return DX_FAILURE;
    } else {
      new = greatest;
      if (new < current + additional) {
        dx_set_error(DX_ERROR_NOT_FOUND);
        return DX_FAILURE;   
      }
    }
  }
  *RETURN = new;
  return DX_SUCCESS;
}
