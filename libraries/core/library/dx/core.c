#include "dx/core.h"

#include "dx/core/object.library.h"

Core_Result Core_initialize() {
  if (Core_Runtime_initialize()) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_uninitialize() {
  Core_Runtime_uninitialize();
  return Core_Success;
}
