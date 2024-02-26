#include "dx/core/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// fwrite
#include <stdio.h>

// isnan, fabsf, fabs
#include <math.h>

#if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
// InterlockedIncrement64, InterlockedDecrement64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem

#else
  #error("operating system not (yet) supported")
#endif

#include "Core/safeMulNx.h"
#include "Core/safeAddNx.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_reference_counter dx_reference_counter_increment(dx_reference_counter* reference_counter) {
#if 1 == DX_ATOMIC_REFERENCE_COUNTING_ENABLED
  #if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
    #if defined(_WIN64)
        return InterlockedIncrement64(reference_counter);
    #else
        return InterlockedIncrement(reference_counter);
    #endif
  #elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem
    return __sync_add_and_fetch(&reference_counter, 1);
  #else
    #error("operating system not (yet) supported")
  #endif
#else
  return ++(*reference_counter);
#endif
}

dx_reference_counter dx_reference_counter_decrement(dx_reference_counter* reference_counter) {
#if 1 == DX_ATOMIC_REFERENCE_COUNTING_ENABLED
  #if defined(Core_OperatingSystem) && Core_OperatingSystem_Windows == Core_OperatingSystem
    #if defined(_WIN64)
        return InterlockedDecrement64(reference_counter);
    #else
        return InterlockedDecrement(reference_counter);
    #endif
  #elif defined(Core_OperatingSystem) && Core_OperatingSystem_Linux == Core_OperatingSystem
    return __sync_subtract_and_fetch(&reference_counter, 1);
  #else
    #error("operating system not (yet) supported")
  #endif
#else
  return --(*reference_counter);
#endif
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void dx_log(char const *p, Core_Size n) {
  if (!p || !n) {
    return;
  } else {
   if (n != fwrite(p, 1, n, stdout)) {
    return;
   }
  }
}


void dx_log_i64(Core_Integer64 value) {
  fprintf(stdout, "%"PRIi64, value);
}

void dx_log_i32(Core_Integer32 value) {
  fprintf(stdout, "%"PRIi32, value);
}

void dx_log_i16(Core_Integer16 value) {
  fprintf(stdout, "%"PRIi16, value);
}

void dx_log_i8(Core_Integer8 value) {
  fprintf(stdout, "%"PRIi8, value);
}


void dx_log_n64(Core_Natural64 value) {
  fprintf(stdout, "%"PRIu64, value);
}

void dx_log_n32(Core_Natural32 value) {
  fprintf(stdout, "%"PRIu32, value);
}

void dx_log_n16(Core_Natural16 value) {
  fprintf(stdout, "%"PRIu16, value);
}

void dx_log_n8(Core_Natural8 value) {
  fprintf(stdout, "%"PRIu8, value);
}


void dx_log_p(void const* p) {
  fprintf(stdout, "%p", p);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

bool dx_almost_equal_f32(Core_Real32 a, Core_Real32 b, int method, Core_Real32 epsilon) {
  if (isnan(a) && isnan(b)) {
    return true;
  }
  if (isnan(epsilon)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
  switch (method) {
  case 1:
    return a == b;
  case 2:
    return fabsf(a - b) <= epsilon;
  case 3:
    return fabsf(a - b) <= (epsilon * max(fabsf(a), fabsf(b)));
  default:
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
}

bool dx_almost_equal_f64(Core_Real64 a, Core_Real64 b, int method, Core_Real64 epsilon) {
  if (isnan(a) && isnan(b)) {
    return true;
  }
  if (isnan(epsilon)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
  switch (method) {
  case 1:
    return a == b;
  case 2:
    return fabs(a - b) <= epsilon;
  case 3:
    return fabs(a - b) <= (epsilon * max(fabs(a), fabs(b)));
  default:
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Boolean dx_fp32_is_subnormal(Core_Real32 x) {
  return FP_SUBNORMAL == fpclassify(x);
}

Core_Boolean dx_fp64_is_subnormal(Core_Real64 x) {
  return FP_SUBNORMAL == fpclassify(x);
}

Core_Boolean dx_fp32_is_nan(Core_Real32 x) {
  return FP_NAN == fpclassify(x);
}

Core_Boolean dx_fp64_is_nan(Core_Real64 x) {
  return FP_NAN == fpclassify(x);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
