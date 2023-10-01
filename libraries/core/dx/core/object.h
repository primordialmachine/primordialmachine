#if !defined(DX_CORE_OBJECT_H_INCLUDED)
#define DX_CORE_OBJECT_H_INCLUDED

#include "dx/core/core.h"

/// @brief Macro providing the name of the "self" variable.
#define SELF _self
/// @brief Macro providing the name of the "return" variable.
#define RETURN _return
/// @brief Macro providing the name of the "type" variable.
#define TYPE _type

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief
/// The base of non-POD objects.
typedef struct dx_object dx_object;

/// @brief
/// The base of the dispatch of non-POD objects.
typedef struct dx_object_dispatch dx_object_dispatch;

/// @brief
/// The opaque C representation of runtime information on a type.
typedef struct dx_rti_type dx_rti_type;

/// @brief
/// The opaque C representation of a weak reference.
typedef struct dx_weak_reference dx_weak_reference;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Initialize the runtime type system.
/// @return #DX_SUCCESS on success. #DX_FAILURE on failure.
/// @failure This function sets the by-thread error variable.
dx_result dx_rti_initialize();

/// @brief Uninitialize the runtime type system.
void dx_rti_unintialize();

/// @brief Get if a type is a fundemental type.
/// @param type A pointer to the type.
/// @return @a true if the type is a fundamental type. @a false otherwise.
/// @undefined @a type does not point to a type.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_fundamental(dx_rti_type* type);

/// @brief Get if a type is an enumeration type.
/// @param type A pointer to the type.
/// @return @a true if the type is an enumeration type. @a false otherwise.
/// @undefined @a type does not point to a type.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_enumeration(dx_rti_type* type);

/// @brief Get if a type is an object type.
/// @param type A pointer to the type.
/// @return @a true if the type is an object type. @a false otherwise.
/// @undefined @a type does not point to a type.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_object(dx_rti_type* type);

/// @brief Used to register types like ee_(i|n)(8|16|32|64), ee_f(32|64), ee_size, and ee_boolean.
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @param value_size The size of a value of that type.
/// @return A pointer to the dx_rti_type object representing the type on success. The null pointer on failure.
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// @default-failure
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @error #DX_ERROR_EXISTS a type of the same name already exists
dx_result dx_rti_create_fundamental(dx_rti_type** RETURN, char const *p, dx_size n, void(*on_type_destroyed)(), dx_size value_size);

#define DX_DECLARE_FUNDAMENTAL_TYPE(NAME, C_NAME) \
  dx_rti_type* C_NAME##_get_type(); \

#define DX_DEFINE_FUNDAMENTAL_TYPE(NAME, C_NAME) \
  static dx_rti_type* _##C_NAME##_type = NULL; \
\
  static void C_NAME##_on_type_destroyed() { \
    _##C_NAME##_type = NULL; \
  } \
\
  dx_rti_type* C_NAME##_get_type() { \
    if (!_##C_NAME##_type) { \
      if (dx_rti_create_fundamental(&_##C_NAME##_type, NAME, sizeof(NAME) - 1, &C_NAME##_on_type_destroyed, sizeof(C_NAME))) { \
        return NULL; \
      } \
    } \
    return _##C_NAME##_type; \
  }

/// @brief Used to register enumeration types.
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @return A pointer to the dx_rti_type object representing the type on success. The null pointer on failure.
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// @default-failure
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @error #DX_ERROR_EXISTS a type of the same name already exists
dx_result dx_rti_create_enumeration(dx_rti_type** RETURN, char const* p, dx_size n, void(*on_type_destroyed)());

#define DX_DECLARE_ENUMERATION_TYPE(NAME, C_NAME) \
  dx_rti_type* C_NAME##_get_type(); \
  typedef enum C_NAME C_NAME;

#define DX_DEFINE_ENUMERATION_TYPE(NAME, C_NAME) \
  static dx_rti_type* _##C_NAME##_type = NULL; \
\
  static void C_NAME##_on_type_destroyed() { \
    _##C_NAME##_type = NULL; \
  } \
\
  dx_rti_type* C_NAME##_get_type() { \
    if (!_##C_NAME##_type) { \
      if (dx_rti_create_enumeration(&_##C_NAME##_type, NAME, sizeof(NAME) - 1, &C_NAME##_on_type_destroyed)) { \
        return NULL; \
      } \
    } \
    return _##C_NAME##_type; \
  }

/// @brief Used to register object types.
///
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @param parent A pointer to the parent type or a null pointer.
///
/// @param object_size The object size.
/// @param object_destruct A pointer to the object destructor or a null pointer.
///
/// @param dispatch_size The dispatch size.
/// @param dispatch_construct A pointer to the dispatch constructor or a null pointer.
/// 
/// @return A pointer to the dx_rti_type object representing the type on success. The null pointer on failure.
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// @default-failure
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed
/// @error #DX_ERROR_EXISTS a type of the same name already exists
dx_result dx_rti_create_object(dx_rti_type** RETURN, char const* p, dx_size n, void (*on_type_destroyed)(), dx_rti_type* parent,
                               dx_size object_size, void (*object_destruct)(dx_object*), \
                               dx_size dispatch_size, void (*dispatch_construct)(dx_object_dispatch*));

#define DX_DECLARE_OBJECT_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  dx_rti_type* C_NAME##_get_type(); \
  typedef struct C_NAME##_dispatch C_NAME##_dispatch; \
  typedef struct C_NAME C_NAME;

#define DX_DEFINE_OBJECT_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  static dx_rti_type* _##C_NAME##_type = NULL; \
\
  static void C_NAME##_destruct(C_NAME* SELF); \
\
  static void C_NAME##_dispatch_construct(C_NAME##_dispatch* SELF); \
\
  static void C_NAME##_on_type_destroyed() { \
    _##C_NAME##_type = NULL; \
  } \
\
  dx_rti_type* C_NAME##_get_type() { \
    if (!_##C_NAME##_type) { \
      dx_rti_type* parent = C_PARENT_NAME##_get_type(); \
      if (!parent) { \
        return NULL; \
      } \
      if (dx_rti_create_object(&_##C_NAME##_type, NAME, sizeof(NAME) - 1, &C_NAME##_on_type_destroyed, parent, \
                               sizeof(C_NAME), (void(*)(dx_object*))&C_NAME##_destruct, \
                               sizeof(C_NAME##_dispatch), (void(*)(dx_object_dispatch*))&C_NAME##_dispatch_construct)) { \
        return NULL; \
      } \
    } \
    return _##C_NAME##_type; \
  }

/// @brief Get if a type is a lower than or equal to another type.
/// @param x The first type.
/// @param y The second type.
/// @return
/// @a true if the first type is lower than or equal to the second type.
/// @a false otherwise.
/// 
/// @undefined @a x does not pointed to a dx_type object.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_leq(dx_rti_type* x, dx_rti_type* y);

/// @brief Get a pointer to the dispatch of an object type.
/// @param x The type.
/// @return A pointer to the dispatch.
/// 
/// @undefined @a x does not pointed to a dx_type object.
/// @undefined The runtime type system is not initialized.
void* dx_rti_type_get_dispatch(dx_rti_type* x);

/// @brief Get the parent type of a type.
/// @param x The type.
/// @return The parent type of the type pointed to by @a x if a parent type exists.
/// The null pointer otherwise. The null pointer is also returned on failure.
/// 
/// @undefined @a x does not pointed to a dx_type object.
/// @undefined The runtime type system is not initialized.
dx_rti_type* dx_rti_type_get_parent(dx_rti_type* x);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Get the "DX.Object" type.
/// @return A pointer to the "DX.Object" type on success. The null pointer on failure.
dx_rti_type* dx_object_get_type();

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>dx_object</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>dx_object</code> pointer.
static inline dx_object* DX_OBJECT(void *p) {
  return (dx_object*)p;
}

struct dx_object {
  /// @brief The reference count of the object.
  dx_reference_counter reference_count;
  /// @brief A pointer to the type of this object or the null pointer.
  dx_rti_type* type;
  /// @brief Singly-linked list of weak references.
  dx_weak_reference* weak_references;
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  char magic_bytes[6];
#endif
};

void DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(void* p);

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>dx_object_dispatch</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>dx_object_dispatch</code> pointer.
static inline dx_object_dispatch* DX_OBJECT_DISPATCH(void* p) {
  return (dx_object_dispatch*)p;
}

struct dx_object_dispatch {
  int dummy;
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  char magic_bytes[8];
#endif
};

void DX_DEBUG_CHECK_OBJECT_DISPATCH_MAGIC_BYTES(void* p);

/// @brief
/// Allocate a dx_object.
/// @param size
/// The size, in Bytes, of the object. Must be greater than or equal to <code>sizeof(dx_object)</code>.
/// @return
/// A pointer to the object on success. A null pointer on failure.
/// @post
/// On success, a pointer to dx_object object is returned.
/// The object is initialized such that <code>dx_object::destructor</code> is assigned a null pointer and <code>dx_object::reference_count</code> is assigned @a 1.
dx_object* dx_object_alloc(dx_size size);

#define DX_CREATE_PREFIX(_TYPE) \
  dx_rti_type* TYPE = _TYPE##_get_type(); \
  if (!TYPE) { \
    return DX_FAILURE; \
  } \
  _TYPE* SELF = (_TYPE*)dx_object_alloc(sizeof(_TYPE)); \
  if (!SELF) { \
    return DX_FAILURE; \
  }

/// @brief
/// Increment the reference count of a dx_object object by @a 1.
/// @param object
/// A pointer to the dx_object object.
void dx_object_reference(dx_object *object);

/// @brief
/// Increment the reference count of a dx_object object by @a 1.
/// @param object
/// A pointer to the dx_object object.
void dx_object_unreference(dx_object* object);

static inline void DX_REFERENCE(void *p) {
  dx_object_reference(DX_OBJECT(p));
}

static inline void DX_UNREFERENCE(void *p) {
  dx_object_unreference(DX_OBJECT(p));
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
  dx_rti_type* TYPE = DX_OBJECT(SELF)->type; \
  _TYPE##_dispatch* dispatch = (_TYPE##_dispatch*)dx_rti_type_get_dispatch(TYPE); \
  return dispatch->_FUNCTION(__VA_ARGS__);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// A weak reference.
DX_DECLARE_OBJECT_TYPE("dx.weak_reference",
                       dx_weak_reference,
                       dx_object);

static inline dx_weak_reference* DX_WEAK_REFERENCE(void* p) {
  return (dx_weak_reference*)p;
}

struct dx_weak_reference {
  dx_object _parent;
  /// @brief A weak reference is linked into the singly-linked list of weak references of its referenced object if there is such an object.
  dx_weak_reference* next;
  dx_object* object;
};

static inline dx_weak_reference_dispatch* DX_WEAK_REFERENCE_DISPATCH(void* p) {
  return (dx_weak_reference_dispatch*)p;
}

struct dx_weak_reference_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_weak_reference_construct(dx_weak_reference* SELF, dx_object* object);

dx_result dx_weak_reference_create(dx_weak_reference** RETURN, dx_object* object);

dx_result dx_weak_reference_set(dx_weak_reference* SELF, dx_object* object);

dx_result dx_weak_reference_acquire(dx_object** RETURN, dx_weak_reference* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_FUNDAMENTAL_TYPE("dx.n8",
                            dx_n8);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.n16",
                            dx_n16);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.n32",
                            dx_n32);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.n64",
                            dx_n64);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.i8",
                            dx_i8);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.i16",
                            dx_i16);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.i32",
                            dx_i32);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.i64",
                            dx_i64);

/// @todo Should be "r32".
DX_DECLARE_FUNDAMENTAL_TYPE("dx.f32",
                            dx_f32);

/// @todo Should be "r64".
DX_DECLARE_FUNDAMENTAL_TYPE("dx.f64",
                            dx_f64);

DX_DECLARE_FUNDAMENTAL_TYPE("dx.bool",
                            dx_bool);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_OBJECT_H_INCLUDED
