#include "dx/core/_get_best_array_size.h"

#include "Core/NextPowerOfTwo.h"

Core_Result dx_get_best_array_size(Core_Size* RETURN, Core_Size current, Core_Size additional, Core_Size least, Core_Size greatest, Core_Boolean saturate) {
  Core_Error old_error = Core_getError();
  if (least > greatest) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_Size_Greatest - current < additional) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size new = current;
  if (new < least) {
    new = least;
  }
  new = current + additional;
  {
    Core_Size temporary;
    if (Core_nextPowerOfTwoGteSz(&temporary, new)) {
      // no power of two equal to or greater than new is found
      // new is not a power of two and there is no next power of two greater than new
      Core_setError(old_error);
    } else {
      new = temporary;
    }
  }
  if (new > greatest) {
    if (!saturate) {
      Core_setError(Core_Error_NotFound);
      return Core_Failure;
    } else {
      new = greatest;
      if (new < current + additional) {
        Core_setError(Core_Error_NotFound);
        return Core_Failure;   
      }
    }
  }
  *RETURN = new;
  return Core_Success;
}
