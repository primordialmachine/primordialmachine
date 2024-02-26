// Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.

#include "dx/player/run.h"

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

int main(int argc, char **argv) {
  if (dx_run(argc, argv)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
