// Copyright (c) 2019-2023 Michael Heilmann. All rights reserved.

#include "Core/Memory.h"

// malloc, realloc, free
#include <malloc.h>

// memmove, memcpy, memset
#include <string.h>

#include "dx/core/configuration.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(Core_Memory_withTagging) && 1 == Core_Memory_withTagging

typedef struct Tag {
  
  Core_Natural64 n;

} Tag;

#if defined(Core_Memory_withStatistics) && 1 == Core_Memory_withStatistics

typedef struct Statistics {
  Core_Natural64 smallestBlock;     ///< The size, in Bytes, of the greatest block allocated.
                                    ///< Can be @a 0 even if Blocks are allocated as Blocks can have zero size.
                                    ///< If no blocks are currently allocated, then the value is @a 0.
  Core_Natural64 greatestBlock;     ///< The size, in Bytes, of the greatest block allocated.
                                    ///< Can be @a 0 even if Blocks are allocated as Blocks can have zero size.
                                    ///< If no blocks are currently allocated, then the value is @a 0.
  Core_Natural64 numberOfBlocks;    ///< The number of blocks currently allocated.
                                    ///< The sum of the sizes, in Bytes, of all these blocks is the number of Bytes/KiloBytes allocated.
  Core_Natural64 numberOfBytes;     ///< The number of Bytes currently allocated.

} Statistics;

static Statistics g_statistics = {
  .smallestBlock = 0,
  .greatestBlock = 0,
  .numberOfBlocks = 0,
  .numberOfBytes = 0,
};

#endif

#endif


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_Memory_copy(void* p, void const* q, Core_Size n) {
  if (!p || !q) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  memcpy(p, q, n);
  return Core_Success;
}

Core_Result Core_Memory_move(void* p, void const* q, Core_Size n) {
  if (!p || !q) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  memmove(p, q, n);
  return Core_Success;
}

Core_Result Core_Memory_zero(void* p, Core_Size n) {
  return Core_Memory_fill(p, n, 0);
}

Core_Result Core_Memory_fill(void* p, Core_Size n, Core_Natural8 v) {
  if (!p) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  memset(p, v, n);
  return Core_Success;
}

Core_Result Core_Memory_allocate(void **RETURN, Core_Size n) {
#if defined(Core_Memory_withTagging) && 1 == Core_Memory_withTagging
  if (Core_Natural64_Greatest - sizeof(Tag) < n) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
#if defined(Core_Memory_withStatistics) && 1 == Core_Memory_withStatistics
  if (Core_Natural64_Greatest - g_statistics.numberOfBytes < n) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
#endif
  Tag* x = malloc(sizeof(Tag) + n);
  if (!x) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  x->n = n;
#if defined(Core_Memory_withStatistics) && 1 == Core_Memory_withStatistics
  g_statistics.numberOfBytes += n;
  if (g_statistics.greatestBlock < n) g_statistics.greatestBlock = n;
  g_statistics.numberOfBlocks += 1;
#endif
  *RETURN = (void*)(x + 1);
#else
  void* p = malloc(n > 0 ? n : 1);
  if (!p) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  *RETURN = p;
#endif
  return Core_Success;
}

Core_Result Core_Memory_reallocate(void** RETURN, void* p, Core_Size n) {
  if (!RETURN || !p) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
#if defined(Core_Memory_withTagging) && 1 == Core_Memory_withTagging
  if (Core_Size_Greatest - sizeof(Tag) < n) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  Tag* x = ((Tag*)(p)) - 1;
  Core_Size oldn = x->n;
  Core_Size newn = n;
#if defined(Core_Memory_withStatistics) && 1 == Core_Memory_withStatistics
  if (oldn < newn) {
    if (Core_Natural64_Greatest - g_statistics.numberOfBytes < newn - oldn) {
      Core_setError(Core_Error_AllocationFailed);
      return Core_Failure;
    }
  }
#endif
  Tag* y = realloc(x, sizeof(Tag) + n);
  if (!y) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  y->n = newn;
#if defined(Core_Memory_withStatistics) && 1 == Core_Memory_withStatistics
  if (oldn < newn) {
    g_statistics.numberOfBytes += newn - oldn;
    if (g_statistics.greatestBlock < newn) g_statistics.greatestBlock = newn;
  } else {
    g_statistics.numberOfBytes -= oldn - newn;
  }
#endif
  *RETURN = (void*)(y + 1);
#else
  void* q = realloc(p, n > 0 ? n : 1);
  if (!q) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  *RETURN = q;
#endif
  return Core_Success;
}

void Core_Memory_deallocate(void* p) {
  if (p) {
  #if defined(Core_Memory_withTagging) && 1 == Core_Memory_withTagging
    Tag* x = ((Tag*)(p)) - 1;
  #if defined(Core_Memory_withStatistics) && 1 == Core_Memory_withStatistics
    g_statistics.numberOfBytes -= x->n;
    g_statistics.numberOfBlocks -= 1;
  #endif
    free(x);
  #else
    free(p);
  #endif
  }
}

Core_Result Core_Memory_compare(Core_Integer8* RETURN, void const* p, void const* q, Core_Size n) {
  if (!RETURN || !p || !q) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  int temporary = memcmp(p, q, n);
  *RETURN = temporary < 0 ? -1 : (temporary > 0 ? +1 : 0);
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
