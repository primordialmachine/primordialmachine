#include "dx/core/os.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "Core/Memory.h"
#include "dx/core/safe_add_nx.h"

#if !defined(Core_OperatingSystem)

  #error("target operating system not defined")

#endif

#if Core_OperatingSystem == DX_OPERATING_SYSTEM_WINDOWS

  // SYSTEM_INFO, GetSystemInfo, MAX_PATH
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

#endif

#if Core_OperatingSystem == DX_OPERATING_SYSTEM_CYGWIN

  // PATH_MAX
  #include <limits.h>

#endif
#if Core_OperatingSystem == DX_OPERATING_SYSTEM_LINUX || \
    Core_OperatingSystem == DX_OPERATING_SYSTEM_MACOS

  // PATH_MAX
  #include <linux/limits.h>

#endif

#if (Core_OperatingSystem == DX_OPERATING_SYSTEM_LINUX)  || \
    (Core_OperatingSystem == DX_OPERATING_SYSTEM_CYGWIN) || \
    (Core_OperatingSystem == DX_OPERATING_SYSTEM_MACOS)

  // sysconf
  #include <unistd.h>

#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if (Core_OperatingSystem == DX_OPERATING_SYSTEM_LINUX)  || \
    (Core_OperatingSystem == DX_OPERATING_SYSTEM_CYGWIN) || \
    (Core_OperatingSystem == DX_OPERATING_SYSTEM_MACOS)

Core_Result Core_getPageSize(Core_Size* RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  long temporary = sysconf(_SC_PAGESIZE);
  if (temporary < 0 || temporary > Core_Size_Greatest) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

Core_Result Core_getNumberOfCores(Core_Size* RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  long temporary = sysconf(_SC_NPROCESSORS_ONLN);
  if (temporary < 0 || temporary > Core_Size_Greatest) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

Core_Result Core_getExecutablePathname(Core_String** RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  // w/o trailing zero
  char buffer[PATH_MAX + 1];
  // readlink will not append a zero terminator.
  ssize_t size = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (-1 == size) {
    Core_setErrror(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  buffer[size] = '\0';
  /// @todo `readlink` returns a string in some encoding.
  /// We assume(!) that this encoding is UTF-8 at the moment.
  Core_String* temporary = NULL;
  if (Core_String_create(&temporary, buffer, size)) {
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

#endif

#if Core_OperatingSystem == DX_OPERATING_SYSTEM_WINDOWS  

Core_Result Core_getPageSize(Core_Size* RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure; 
  }
  SYSTEM_INFO systemInfo;
  GetSystemInfo(&systemInfo);
  if (systemInfo.dwPageSize > Core_Size_Greatest) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = (Core_Size)systemInfo.dwPageSize;
  return Core_Success;
}

Core_Result Core_getNumberOfCores(Core_Size* RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SYSTEM_INFO systemInfo;
  GetSystemInfo(&systemInfo);
  if (systemInfo.dwNumberOfProcessors > Core_Size_Greatest) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  *RETURN = (Core_Size)systemInfo.dwNumberOfProcessors;
  return Core_Success;
}

Core_Result Core_getExecutablePath(Core_String** RETURN) {
  HMODULE module = GetModuleHandleA(NULL);
  if (!module) {
    Core_setError(Core_Error_EnvironmentFailed);
    return Core_Failure;
  }
  Core_Size n = MAX_PATH;
  char* p = NULL;
  if (Core_Memory_allocate(&p, n)) {
    return Core_Failure;
  }
  do {
    DWORD m = GetModuleFileNameA(module, p, n);
    if (m == 0) {
      Core_Memory_deallocate(p);
      p = NULL;
      Core_setError(Core_Error_EnvironmentFailed);
      return Core_Failure;
    }
    // If m is non-zero, then the function succeeded.
    // There are now two cases:
    // a) m == n: The buffer was too small (n is non-zero).
    // b) m < n: m denotes the number of characters copied without the zero terminator.
    if (m == n) {
      Core_Size overflow;
      Core_Size n_new;
      Core_safeAddSz(&n_new, n, MAX_PATH, &overflow);
      if (overflow) {
        n_new = SIZE_MAX;
      }
      if (n_new == n) {
        Core_Memory_deallocate(p);
        p = NULL;
        Core_setError(Core_Error_AllocationFailed);
        return Core_Failure;
      }
      if (Core_Memory_reallocate(&p, p, n_new)) {
        Core_Memory_deallocate(p);
        p = NULL;
        return Core_Failure;
      }
      n = n_new;
    } else {
      Core_String* s = NULL;
      if (Core_String_create(&s, p, m)) {
        Core_Memory_deallocate(p);
        p = NULL;
        return Core_Failure;
      }
      Core_Memory_deallocate(p);
      p = NULL;
      *RETURN = s;
      return Core_Success;
    }
  } while (true);
}

#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
