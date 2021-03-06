/// @file Runtime/Status.c
/// @author Michael Heilmann <michaelheilmann@primordialmachine.com>
/// @copyright Copyright (c) 2021 Michael Heilmann. All rights reserved.
#define MACHINE_RUNTIME_PRIVATE (1)
#include "Runtime/Status.h"

static Machine_StatusValue g_status = Machine_Status_Success;

Machine_StatusValue Machine_getStatus() {
  return g_status;
}

void Machine_setStatus(Machine_StatusValue status) {
  g_status = status;
}
