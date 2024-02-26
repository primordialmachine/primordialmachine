#if !defined(DX_CONFIGURATION_H_INCLUDED)
#define DX_CONFIGURATION_H_INCLUDED

#include "dx/core/Configure.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core#core-version-major */
#define Core_Version_Major (2)

/* http://localhost/core#core-version-minor */
#define Core_Version_Minor (0)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core#core-visualsbackend-direct3d12 */
#define Core_VisualsBackend_Direct3d12 (1)

/* http://localhost/core#core-visualsbackend-opengl4 */
#define Core_VisualsBackend_OpenGl4 (2)

/* http://localhost/core#core-visualsbackend-vulkan */
#define Core_VisualsBackend_Vulkan (3)

/* http://localhost/core#core-visualsbackend */
#define Core_VisualsBackend Core_VisualsBackend_OpenGl4

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core#core-audialsbackend-openal */
#define Core_AudialsBackend_OpenAl (1)

/* http://localhost/core#core-audialsbackend */
#define Core_AudialsBackend Core_AudialsBackend_OpenAl

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined an evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for Core_Object.
/// A function or macro Core_Debug_checkObjectMagicBytes is provided.
/// If this function is invoked on a pointer that does not point to a valid Core_Object (or derived type) object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define Core_withObjectMagicBytes (1)

/// If defined and evaluating to @a 1 and if compiled in debug mode,
/// then "magic bytes" memory corruption detection pattern is available for Core_Object_Dispatch.
/// A function or macro Core_Debug_checkObjectDispatchMagicBytes is provided.
/// If this function is invoked on a pointer that does not point to a valid Core_Object_Dispatch (or derived type) object,
/// then there is a good chance (but only a chance) that this function will cause a debug break.
#define Core_withObjectDispatchMagicBytes (1)

/// If defined and evaluating to @a 1,
/// then atomic reference counting is enabled.
/// Otherwise it is disabled, that is, non-atomic reference counting is performed.
#define DX_ATOMIC_REFERENCE_COUNTING_ENABLED (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1, then memory blocks allocated with Core_Memory_allocate are prefixed with their sizes.
#define Core_Memory_withTagging (1)

/// If defined and evaluating to @a 1 and if tagging is enabled (see Core_Memory_withTagging), then the allocator maintains statistics on allocations.
#define Core_Memory_withStatistics (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// then a function dx_math_tests() is provided
/// which performs tests on functionality provided by "dx/core/math.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_MATH_WITH_TESTS (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// If defined and evaluating to @a 1,
/// the a function dx_safe_mul_ix_tests() is provided
/// which performs tests on functionality provided by dx/core/safe_mul_ix.h".
/// This function succeeds if all such tests succeed, otherwise it fails.
#define DX_SAFE_MUL_IX_WITH_TESTS (1)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CONFIGURATION_H_INCLUDED

