#include "Core/Tests/fundamentalTypes.h"

Core_Result Core_Tests_fundamentalTypesTests() {
  Core_Type* type = NULL;

  if (Core_Natural8_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Natural16_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Natural32_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Natural64_getType(&type)) {
    return Core_Failure;
  }

  if (Core_Integer8_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Integer16_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Integer32_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Integer64_getType(&type)) {
    return Core_Failure;
  }

  if (Core_Boolean_getType(&type)) {
    return Core_Failure;
  }

  if (Core_Real32_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Real64_getType(&type)) {
    return Core_Failure;
  }

  if (Core_Size_getType(&type)) {
    return Core_Failure;
  }

  return Core_Success;
}
