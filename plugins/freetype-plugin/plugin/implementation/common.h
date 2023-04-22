// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#if !defined(DX_FREETYPE_PLUGIN_COMMON_H_INCLUDED)
#define DX_FREETYPE_PLUGIN_COMMON_H_INCLUDED

// uint8_t, uint32_t
#include <inttypes.h>

wchar_t* mb_to_wc(const char* source);

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WITH_ATOMIC_REFERENCE_COUNT_ENABLED (1)

/// @brief This must be aligned to 64-bit boundaries on 64-bit systems and to 32-bit boundaries on 32-bit systems.
#if defined(_WIN64)
  typedef int64_t _reference_counter;
#else
  typedef int32_t _reference_counter;
#endif

static inline _reference_counter _reference_counter_increment(_reference_counter* reference_counter) {
#if defined(WITH_ATOMIC_REFERENCE_COUNT_ENABLED) && 1 == WITH_ATOMIC_REFERENCE_COUNT_ENABLED
  #if defined(_WIN64)
    return InterlockedIncrement64(reference_counter);
  #else
    return InterlockedIncrement(reference_counter);
  #endif
#else
  return ++(*reference_counter);
#endif
}

static inline _reference_counter _reference_counter_decrement(_reference_counter* reference_counter) {
#if defined(WITH_ATOMIC_REFERENCE_COUNT_ENABLED) && 1 == WITH_ATOMIC_REFERENCE_COUNT_ENABLED
  #if defined(_WIN64)
    return InterlockedDecrement64(reference_counter);
  #else
    return InterlockedDecrement(reference_counter);
  #endif
#else
  return --(*reference_counter);
#endif
}

#endif // DX_FREETYPE_PLUGIN_COMMON_H_INCLUDED
