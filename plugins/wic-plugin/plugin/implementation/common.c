// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#include "common.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

wchar_t* mb_to_wc(const char* source) {
  int result;
  result = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
  if (!result) {
    return NULL;
  }
  wchar_t* target = malloc(sizeof(wchar_t) * result);
  if (!target) {
    return NULL;
  }
  result = MultiByteToWideChar(CP_ACP, 0, source, -1, target, result);
  if (!result) {
    free(target);
    target = NULL;
    return NULL;
  }
  return target;
}
