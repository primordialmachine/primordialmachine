#if !defined(DX_CORE_OBJECT_H_INCLUDED)
#define DX_CORE_OBJECT_H_INCLUDED

#include "dx/core/core.h"
#include "dx/core/TypeSystem.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// The base of non-POD objects.
typedef struct Core_Object Core_Object;

/// @brief
/// The base of the dispatch of non-POD objects.
typedef struct Core_Object_Dispatch Core_Object_Dispatch;

/// @brief
/// The opaque C representation of a weak reference.
typedef struct Core_WeakReference Core_WeakReference;

/// @brief Get the "Core.Object" type.
/// @return A pointer to the "Core.Object" type on success. The null pointer on failure.
Core_Result Core_Object_getType(Core_Type** RETURN);

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>Core_Object</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>Core_Object</code> pointer.
static inline Core_Object* CORE_OBJECT(void* p) {
  return (Core_Object*)p;
}

struct Core_Object {
  /// @brief The reference count of the object.
  dx_reference_counter reference_count;
  /// @brief A pointer to the type of this object or the null pointer.
  Core_Type* type;
  /// @brief Singly-linked list of weak references.
  Core_WeakReference* weak_references;
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  char magic_bytes[6];
#endif
};

void DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(void* p);

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>Core_Object_Dispatch</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>Core_Object_Dispatch</code> pointer.
static inline Core_Object_Dispatch* CORE_OBJECT_DISPATCH(void* p) {
  return (Core_Object_Dispatch*)p;
}

struct Core_Object_Dispatch {
  int dummy;
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  char magic_bytes[8];
#endif
};

void DX_DEBUG_CHECK_OBJECT_DISPATCH_MAGIC_BYTES(void* p);

/// @brief
/// Allocate a Core_Object.
/// @param [out] RETURN
/// A pointer to a <code>Core_Object*</code> variable.
/// @param size
/// The size, in Bytes, of the object. Must be greater than or equal to <code>sizeof(Core_Object)</code>.
/// @return
/// #Core_Success on success. #Core_Failure on failure.
/// @failure
/// This function has set the by-thread error variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the allocated object.
/// @remarks
/// An object created by this function has the following values:
/// - Core_Object::destructor is null
/// - Core_Object::referenceCount is 1
/// - Core_Object::type is a pointer to the Core.Object type
Core_Result Core_Object_allocate(Core_Object** RETURN, Core_Size size);

#define DX_CONSTRUCT_PREFIX(_TYPE) \
  if (!SELF) { \
    Core_setError(Core_Error_ArgumentInvalid); \
    return Core_Failure; \
  } \
  Core_Type* TYPE = NULL; \
  if (_TYPE##_getType(&TYPE)) { \
    return Core_Failure; \
  } \

#define DX_CREATE_PREFIX(_TYPE) \
  if (!RETURN) { \
    Core_setError(Core_Error_ArgumentInvalid); \
    return Core_Failure; \
  } \
  Core_Type* TYPE = NULL; \
  if (_TYPE##_getType(&TYPE)) { \
    return Core_Failure; \
  } \
  _TYPE* SELF = NULL; \
  if (Core_Object_allocate((Core_Object**)&SELF, sizeof(_TYPE))) { \
    return Core_Failure; \
  }

/// @brief
/// Increment the reference count of a Core_Object object by @a 1.
/// @param object
/// A pointer to the Core_Object object.
void Core_Object_reference(Core_Object *object);

/// @brief
/// Increment the reference count of a Core_Object object by @a 1.
/// @param object
/// A pointer to the Core_Object object.
void Core_Object_unreference(Core_Object* object);

static inline void DX_REFERENCE(void *p) {
  Core_Object_reference(CORE_OBJECT(p));
}

static inline void DX_UNREFERENCE(void *p) {
  Core_Object_unreference(CORE_OBJECT(p));
}

/// @brief Utility macro to facilate the definition of functions.
/// @code
/// struct dx_foo_dispatch {
///   dx_bar* (*create_bar)(dx_foo*, dx_quu*);
/// };
/// 
/// static inline dx_bar* dx_foo_create_bar(dx_foo* SELF, dx_quu* quu) {
///   DX_OBJECT_VIRTUALCALL(dx_foo, create_bar, SELF, quu);
/// }
/// 
/// @endcode
#define DX_OBJECT_VIRTUALCALL(_TYPE, _FUNCTION, ...) \
  Core_Type* TYPE = CORE_OBJECT(SELF)->type; \
  _TYPE##_dispatch* dispatch = (_TYPE##_dispatch*)dx_rti_type_get_dispatch(TYPE); \
  return dispatch->_FUNCTION(__VA_ARGS__);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// A weak reference.
DX_DECLARE_OBJECT_TYPE("Core.WeakReference",
                       Core_WeakReference,
                       Core_Object);

static inline Core_WeakReference* DX_WEAK_REFERENCE(void* p) {
  return (Core_WeakReference*)p;
}

struct Core_WeakReference {
  Core_Object _parent;
  /// @brief A weak reference is linked into the singly-linked list of weak references of its referenced object if there is such an object.
  Core_WeakReference* next;
  Core_Object* object;
};

static inline Core_WeakReference_dispatch* DX_WEAK_REFERENCE_DISPATCH(void* p) {
  return (Core_WeakReference_dispatch*)p;
}

struct Core_WeakReference_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_WeakReference_construct(Core_WeakReference* SELF, Core_Object* object);

Core_Result Core_WeakReference_create(Core_WeakReference** RETURN, Core_Object* object);

Core_Result Core_WeakReference_set(Core_WeakReference* SELF, Core_Object* object);

Core_Result Core_WeakReference_acquire(Core_Object** RETURN, Core_WeakReference* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_OBJECT_H_INCLUDED
