// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "Core/Error.h"

#include "dx/core/annotations.h"

Core_ThreadLocal() Core_Error g_error = Core_Error_NoError;

Core_Error Core_getError() {
  return g_error;
}

void Core_setError(Core_Error error) {
  g_error = error;
}
