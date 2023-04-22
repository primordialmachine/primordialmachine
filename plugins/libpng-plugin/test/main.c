// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#include "libpng-plugin.h"

#include "dx/core.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// LoadLibrary, GetProcAddress, FreeLibrary
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static void* allocate(void* context, size_t n) {
  int old_error = dx_get_error();
  void* p = NULL;
  if (dx_memory_allocate(&p, n)) {
    dx_set_error(old_error);
    return NULL;
  }
  return p;
}

static void deallocate(void* context, void* p) {
  if (p) {
    free(p);
  }
}

int main(int argc, char** argv) {
  return EXIT_SUCCESS;
}
