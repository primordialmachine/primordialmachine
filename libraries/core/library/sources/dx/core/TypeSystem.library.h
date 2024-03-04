#if !defined(CORE_TYPESYSTEM_LIBRARY_H_INCLUDED)
#define CORE_TYPESYSTEM_LIBRARY_H_INCLUDED

#include "dx/core/TypeSystem.h"
#include "Core/TypeSystem/TypeName.h"

typedef struct _Core_Type _Core_Type;

struct _Core_Type {
  Core_ReferenceCounter reference_count;
  Core_Natural8 flags;
  _Core_TypeName* name;
  void (*onTypeDestroyed)();
  union {
    // Core_TypeFlags_IsFundamental
    struct {
      Core_Size value_size;
    } fundamental;
    // Core_TypeFlags_IsObject
    struct {
      _Core_Type* parent;

      Core_Size object_size;
      void (*destructObject)(Core_Object*);

      Core_Size dispatch_size;
      void (*constructDispatch)(Core_Object_Dispatch*);

      /// @brief Pointer to the dispatch or a null pointer.
      void* dispatch;

    } object;
  };
};

/// @undefined
/// - Invoked from any other function other than "Core_Runtime_initialize".
/// - The "TypeSystem" module is initialized.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result Core_TypeSystem_initialize();

/// @undefined
/// - Invoked from any other function other than "Core_Runtime_uninitialize".
/// - The "TypeSystem" module is uninitialized.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result Core_TypeSystem_uninitialize();

#endif // CORE_TYPESYSTEM_LIBRARY_H_INCLUDED
