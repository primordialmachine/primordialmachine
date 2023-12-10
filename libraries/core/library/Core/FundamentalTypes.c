#include "Core/FundamentalTypes.h"

#include <math.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_isInfinityR32(Core_Boolean* RETURN, Core_Real32 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = (FP_INFINITE == fpclassify(x));
  return Core_Success;
}

Core_Result Core_isNoNumberR32(Core_Boolean* RETURN, Core_Real32 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = (FP_NAN == fpclassify(x));
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_isInfinityR64(Core_Boolean* RETURN, Core_Real64 x) {
  *RETURN = (FP_INFINITE == fpclassify(x));
  return Core_Success;
}

Core_Result Core_isNoNumberR64(Core_Boolean* RETURN, Core_Real64 x) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = (FP_NAN == fpclassify(x));
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
