// Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
#include <stdlib.h>

#include "dx/core.h"

int main(int argc, char** argv) {
  if (Core_initialize()) {
    return EXIT_FAILURE;
  }
  Core_uninitialize();
  return EXIT_SUCCESS;
}
