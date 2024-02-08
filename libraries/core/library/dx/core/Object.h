#if !defined(CORE_OBJECT_H_INCLUDED)
#define CORE_OBJECT_H_INCLUDED

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
#if _DEBUG && 1 == Core_withObjectMagicBytes
  char magic_bytes[6];
#endif
};

// See Core_withObjectMagicBytes for more information.
#if _DEBUG && 1 == Core_withObjectDispatchMagicBytes
void Core_Debug_checkObjectMagicBytes(void* p);
#else
static inline void Core_Debug_checkObjectMagicBytes(void* p)
{/*Intentionally empty.*/}
#endif

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>Core_Object_Dispatch</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>Core_Object_Dispatch</code> pointer.
static inline Core_Object_Dispatch* CORE_OBJECT_DISPATCH(void* p) {
  return (Core_Object_Dispatch*)p;
}

struct Core_Object_Dispatch {
  int dummy;
#if _DEBUG && 1 == Core_withObjectDispatchMagicBytes
  char magic_bytes[8];
#endif
};

// See Core_withObjectDispatchMagicBytes for more information.
#if _DEBUG && 1 == Core_withObjectDispatchMagicBytes
void Core_Debug_checkObjectDispatchMagicBytes(void* p);
#else
static inline void Core_Debug_checkObjectDispatchMagicBytes(void* p)
{/*Intentionally empty.*/}
#endif

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

/* http://localhost/core#core-reference */
void Core_reference(Core_Object *object);

/* http://localhost/core#core-unreference */
void Core_unreference(Core_Object* object);

static inline void CORE_REFERENCE(void *p) {
  Core_reference(CORE_OBJECT(p));
}

static inline void CORE_UNREFERENCE(void *p) {
  Core_unreference(CORE_OBJECT(p));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* http://localhost/core#core-weakreference */
Core_declareObjectType("Core.WeakReference",
                       Core_WeakReference,
                       Core_Object);

static inline Core_WeakReference* CORE_WEAKREFERENCE(void* p) {
  return (Core_WeakReference*)p;
}

struct Core_WeakReference {
  Core_Object _parent;
  /// @brief A weak reference is linked into the singly-linked list of weak references of its referenced object if there is such an object.
  Core_WeakReference* next;
  Core_Object* object;
};

static inline Core_WeakReference_Dispatch* CORE_WEAKREFERENCE_DISPATCH(void* p) {
  return (Core_WeakReference_Dispatch*)p;
}

struct Core_WeakReference_Dispatch {
  Core_Object_Dispatch _parent;
};

/* http://localhost/core#core-weakreference-construct */
Core_Result Core_WeakReference_construct(Core_WeakReference* SELF, Core_Object* object);

/* http://localhost/core#core-weakreference-create */
Core_Result Core_WeakReference_create(Core_WeakReference** RETURN, Core_Object* object);

/* http://localhost/core#core-weakreference-set */
Core_Result Core_WeakReference_set(Core_WeakReference* SELF, Core_Object* object);

/* http://localhost/core#core-weakreference-acquire */
Core_Result Core_WeakReference_acquire(Core_Object** RETURN, Core_WeakReference* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Utility macro to facilate the definition of functions.
/// @code
/// struct dx_foo_Dispatch {
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
  _TYPE##_Dispatch* dispatch = NULL; \
  if (Core_Type_getDispatch(&dispatch, TYPE)) { \
    return Core_Failure; \
  } \
  return dispatch->_FUNCTION(__VA_ARGS__);

#define DX_CONSTRUCT_PREFIX(_TYPE) \
  Core_BeginConstructor(_TYPE);

/* Utility macro to shorten the definition of a constructor. */
#define Core_BeginConstructor(T) \
  if (!SELF) { \
    Core_setError(Core_Error_ArgumentInvalid); \
    return Core_Failure; \
  } \
  Core_Type* TYPE = NULL; \
  if (T##_getType(&TYPE)) { \
    return Core_Failure; \
  }

/* Utility macro to shorten the definition of a constructor. */
#define Core_EndConstructor(T) \
  CORE_OBJECT(SELF)->type = TYPE; \
  return Core_Success;

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

#endif // CORE_OBJECT_H_INCLUDED
