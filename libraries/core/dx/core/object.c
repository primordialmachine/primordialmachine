#include "dx/core/object.h"

#include "dx/core/memory.h"
#include "dx/core/inline_pointer_hashmap.h"


/// @dummy
static uint32_t weak_references_lock = 0;

/// @dummy
static void _acquire_weak_references_lock();

/// @dummy
static void _relinquish_weak_references_lock();

/// @brief Detach a weak reference from an object.
/// @param object a pointer to the object.
/// @param weak_reference A pointer to the weak reference.
/// @undefined The weak reference is not attached to this object.
/// @undefuned The weak references lock was not acquired.
static void _detach_weak_reference(dx_object* object, dx_weak_reference* weak_reference);

/// @brief Attach a weak reference to an object.
/// @param object a pointer to the object.
/// @param weak_reference A pointer to the weak reference.
/// @undefined The weak reference is attached to an object object.
/// @undefuned The weak references lock was not acquired.
static void _attach_weak_reference(dx_object* object, dx_weak_reference* weak_reference);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_rti_type_name _dx_rti_type_name;

struct _dx_rti_type_name {
  dx_reference_counter reference_count;
  dx_size hash_value;
  dx_size number_of_bytes;
  char bytes[];
};

static dx_result _dx_rti_type_name_create(_dx_rti_type_name** RETURN, char const* bytes, dx_size number_of_bytes);

static dx_result _dx_rti_type_name_hash_key_callback(dx_size* RETURN, _dx_rti_type_name** a);

static dx_result _dx_rti_type_name_compare_keys_callback(dx_bool* RETURN, _dx_rti_type_name** a, _dx_rti_type_name** b);

static void _dx_rti_type_name_reference(_dx_rti_type_name* a);

static void _dx_rti_type_name_unreference(_dx_rti_type_name* a);

static void _dx_rti_type_name_reference_callback(_dx_rti_type_name** a);

static void _dx_rti_type_name_unreference_callback(_dx_rti_type_name** a);

#define _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL (1)
#define _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION (2)
#define _DX_RTI_TYPE_NODE_FLAGS_OBJECT (4)

#if defined(_DEBUG)
static dx_size g_debug_number_of_types = 0;
#endif

typedef struct _dx_rti_type _dx_rti_type;

static inline _dx_rti_type* _DX_RTI_TYPE(void* p) {
  return (_dx_rti_type*)p;
}

struct _dx_rti_type {
  dx_reference_counter reference_count;
  dx_n8 flags;
  _dx_rti_type_name* name;
  void (*on_type_destroyed)();
  union {
    // _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL
    struct {
      dx_size value_size;
    } fundamental;
    // _DX_RTI_TYPE_NODE_FLAGS_OBJECT
    struct {
      _dx_rti_type* parent;

      dx_size object_size;
      void (*destruct_object)(dx_object*);

      dx_size dispatch_size;
      void (*construct_dispatch)(dx_object_dispatch*);

      /// @brief Pointer to the dispatch or a null pointer.
      void* dispatch;

    } object;
  };
};

/// @internal
/// @brief Get if a type is a fundamental type.
/// @param ty A pointer to the type.
/// @return @a true if the type is a fundamental type. @a false if it is not a fundamental type.
/// @a false is also returned on failure.
/// @default-failure
static dx_bool _dx_rti_type_is_fundamental(_dx_rti_type* ty);

/// @internal
/// @brief Get if a type is an enumeration type.
/// @param ty A pointer to the type.
/// @return @a true if the type is an enumeration type. @a false if it is not an enumeration type.
/// @a false is also returned on failure.
/// @default-failure
static dx_bool _dx_rti_type_is_enumeration(_dx_rti_type* ty);

/// @internal
/// @brief Get if a type is an object type.
/// @param ty A pointer to the type.
/// @return @a true if the type is an objec type. @a false if it is not an object type.
/// @a false is also returned on failure.
/// @default-failure
static dx_bool _dx_rti_type_is_object(_dx_rti_type* ty);

/// @internal
/// @brief Ensure the dispatches of a type are created.
/// @param ty A pointer to the type.
/// @default-return
static int _dx_rti_type_ensure_dispatches_created(_dx_rti_type* ty);

/// @internal
/// @brief Ensure the dispatches of a type are destroyed.
/// @param ty A pointer to the type.
/// @return
/// #DX_SUCCESS on success. #DX_FAILURE on failure.
/// Always succeeds.
/// @undefined
/// @a ty does not point to a type.
static dx_result _dx_rti_type_ensure_dispatches_destroyed(_dx_rti_type* ty);

static dx_bool _dx_rti_type_is_fundamental(_dx_rti_type* ty) {
  return _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL == (ty->flags & _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL);
}

static dx_bool _dx_rti_type_is_enumeration(_dx_rti_type* ty) {
  return _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION == (ty->flags & _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION);
}

static dx_bool _dx_rti_type_is_object(_dx_rti_type* ty) {
  return _DX_RTI_TYPE_NODE_FLAGS_OBJECT == (ty->flags & _DX_RTI_TYPE_NODE_FLAGS_OBJECT);
}

static int _dx_rti_type_ensure_dispatches_created(_dx_rti_type* ty) {
  if (!_dx_rti_type_is_object(ty)) {
    return 0;
  }
  if (ty->object.dispatch) {
    return 0;
  }
  if (ty->object.parent) {
    if (_dx_rti_type_ensure_dispatches_created(ty->object.parent)) {
      return 1;
    }
  }
  ty->object.dispatch = NULL;
  if (dx_memory_allocate(&ty->object.dispatch, ty->object.dispatch_size)) {
    return 1;
  }
  dx_memory_zero(ty->object.dispatch, ty->object.dispatch_size);
  if (ty->object.parent) {
    dx_memory_copy(ty->object.dispatch, ty->object.parent->object.dispatch, ty->object.parent->object.dispatch_size);
  }
  if (ty->object.construct_dispatch) {
    ty->object.construct_dispatch(ty->object.dispatch);
  }
  return 0;
}

static dx_result _dx_rti_type_ensure_dispatches_destroyed(_dx_rti_type* ty) {
  if (!_dx_rti_type_is_object(ty)) {
    return DX_SUCCESS;
  }
  if (!ty->object.dispatch) {
    return DX_SUCCESS;
  }
  dx_memory_deallocate(ty->object.dispatch);
  ty->object.dispatch = NULL;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result _dx_rti_type_name_create(_dx_rti_type_name** RETURN, char const* bytes, dx_size number_of_bytes) {
  if (!RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  _dx_rti_type_name* SELF = NULL;
  if (dx_memory_allocate(&SELF, sizeof(_dx_rti_type_name) + number_of_bytes)) {
    return DX_FAILURE;
  }
  SELF->hash_value = dx_hash_bytes(bytes, number_of_bytes);
  dx_memory_copy(SELF->bytes, bytes, number_of_bytes);
  SELF->number_of_bytes = number_of_bytes;
  SELF->reference_count = 1;
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result _dx_rti_type_name_hash_key_callback(dx_size* RETURN, _dx_rti_type_name** a) {
  *RETURN = (*a)->hash_value;
  return DX_SUCCESS;
}

static dx_result _dx_rti_type_name_compare_keys_callback(dx_bool* RETURN, _dx_rti_type_name** a, _dx_rti_type_name** b) {
  if (*a == *b) {
    *RETURN = true;
  } else if ((*a)->number_of_bytes == (*b)->number_of_bytes) {
    *RETURN = !dx_memory_compare((*a)->bytes, (*b)->bytes, (*a)->number_of_bytes);
  } else {
    *RETURN = false;
  }
  return DX_SUCCESS;
}

static void _dx_rti_type_name_reference(_dx_rti_type_name* a) {
  dx_reference_counter_increment(&a->reference_count);
}

static void _dx_rti_type_name_unreference(_dx_rti_type_name* a) {
  if (0 == dx_reference_counter_decrement(&a->reference_count)) {
    dx_memory_deallocate(a);
  }
}

static void _dx_rti_type_name_reference_callback(_dx_rti_type_name** a) {
  _dx_rti_type_name_reference(*a);
}

static void _dx_rti_type_name_unreference_callback(_dx_rti_type_name** a) {
  _dx_rti_type_name_unreference(*a);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void _dx_rti_type_reference(_dx_rti_type* a) {
  dx_reference_counter_increment(&a->reference_count);
}

static void _dx_rti_type_unreference(_dx_rti_type* a) {
  if (0 == dx_reference_counter_decrement(&a->reference_count)) {
    if (a->on_type_destroyed) {
      a->on_type_destroyed();
    }
    if (_dx_rti_type_is_object(a)) {
      if (a->object.parent) {
        _dx_rti_type_unreference(a->object.parent);
        a->object.parent = NULL;
      }
      _dx_rti_type_ensure_dispatches_destroyed(a);
    }
    _dx_rti_type_name_unreference(a->name);
    a->name = NULL;
    dx_memory_deallocate(a);
  #if defined(_DEBUG)
    g_debug_number_of_types--;
  #endif
  }
}

static void _dx_rti_type_reference_callback(_dx_rti_type** a) {
  _dx_rti_type_reference(*a);
}

static void _dx_rti_type_unreference_callback(_dx_rti_type** a) {
  _dx_rti_type_unreference(*a);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_inline_pointer_hashmap* g_types = NULL;

dx_result dx_rti_initialize() {
  static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
   .compare_keys_callback = (dx_result(*)(dx_bool*,dx_inline_pointer_hashmap_key*,dx_inline_pointer_hashmap_key*)) & _dx_rti_type_name_compare_keys_callback,
   .hash_key_callback = (dx_result(*)(dx_size*,dx_inline_pointer_hashmap_key*)) &_dx_rti_type_name_hash_key_callback,
   .key_added_callback = (void(*)(dx_inline_pointer_hashmap_key*)) &_dx_rti_type_name_reference_callback,
   .key_removed_callback = (void(*)(dx_inline_pointer_hashmap_key*)) &_dx_rti_type_name_unreference_callback,
   .value_added_callback = (void(*)(dx_inline_pointer_hashmap_value*)) &_dx_rti_type_reference_callback,
   .value_removed_callback = (void(*)(dx_inline_pointer_hashmap_value*)) &_dx_rti_type_unreference_callback,
  };
  if (dx_memory_allocate(&g_types, sizeof(dx_inline_pointer_hashmap))) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_initialize(g_types, &configuration)) {
    dx_memory_deallocate(g_types);
    g_types = NULL;
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

void dx_rti_unintialize() {
  dx_inline_pointer_hashmap_uninitialize(g_types);
  dx_memory_deallocate(g_types);
  g_types = NULL;
}

dx_bool dx_rti_type_is_fundamental(dx_rti_type* type) {
  return _dx_rti_type_is_fundamental(_DX_RTI_TYPE(type));
}

dx_bool dx_rti_type_is_enumeration(dx_rti_type* type) {
  return _dx_rti_type_is_enumeration(_DX_RTI_TYPE(type));
}

dx_bool dx_rti_type_is_object(dx_rti_type* type) {
  return _dx_rti_type_is_object(_DX_RTI_TYPE(type));
}

dx_result dx_rti_create_fundamental(dx_rti_type** RETURN, char const* p, dx_size n, void (*on_type_destroyed)(), dx_size value_size) {
  _dx_rti_type_name* name = NULL;
  if (_dx_rti_type_name_create(&name, p, n)) {
    return DX_FAILURE;
  }
  _dx_rti_type* type = NULL;
  if (dx_inline_pointer_hashmap_get(&type, g_types, name)) {
    if (dx_get_error() != DX_ERROR_NOT_FOUND) {
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
    }
  } else {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    dx_set_error(DX_ERROR_EXISTS);
    _dx_rti_type_name_unreference(name);
    name = NULL;
    return DX_FAILURE;
  }
  if (dx_memory_allocate(&type, sizeof(_dx_rti_type))) {
    return DX_FAILURE;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL;
  type->name = name;
  //_dx_rti_type_name_reference(type->name);
  type->reference_count = 1;
  type->fundamental.value_size = value_size;
  // add the type
  if (dx_inline_pointer_hashmap_set(g_types, name, type)) {
    _dx_rti_type_unreference(type);
    type = NULL;
    return DX_FAILURE;
  }
  *RETURN = (dx_rti_type*)type;
  return DX_SUCCESS;
}

dx_result dx_rti_create_enumeration(dx_rti_type** RETURN, char const* p, dx_size n, void (*on_type_destroyed)()) {
  _dx_rti_type_name* name = NULL;
  if (_dx_rti_type_name_create(&name, p, n)) {
    return DX_FAILURE;
  }
  _dx_rti_type* type = NULL;
  if (dx_inline_pointer_hashmap_get(&type, g_types, name)) {
    if (dx_get_error() != DX_ERROR_NOT_FOUND) {
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
    }
  } else {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    dx_set_error(DX_ERROR_EXISTS);
    _dx_rti_type_name_unreference(name);
    name = NULL;
    return DX_FAILURE;
  }
  if (dx_memory_allocate(&type, sizeof(_dx_rti_type))) {
    return DX_FAILURE;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION;
  type->name = name;
  type->reference_count = 1;
  // add the type
  if (dx_inline_pointer_hashmap_set(g_types, name, type)) {
    _dx_rti_type_unreference(type);
    type = NULL;
    return DX_FAILURE;
  }
  _dx_rti_type_unreference(type);
#if defined(_DEBUG)
  g_debug_number_of_types++;
#endif
  *RETURN = (dx_rti_type*)type;
  return DX_SUCCESS;
}

dx_result dx_rti_create_object(dx_rti_type** RETURN, char const* p, dx_size n, void (*on_type_destroyed)(), dx_rti_type* parent,
                               dx_size object_size, void (*destruct_object)(dx_object*),
                               dx_size dispatch_size, void (*construct_dispatch)(dx_object_dispatch*)) {
  _dx_rti_type_name* name = NULL;
  if (_dx_rti_type_name_create(&name, p, n)) {
    return DX_FAILURE;
  }
  _dx_rti_type* type = NULL;
  if (dx_inline_pointer_hashmap_get(&type, g_types, name)) {
    if (dx_get_error() != DX_ERROR_NOT_FOUND) {
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
    }
  } else {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    _dx_rti_type_name_unreference(name);
    name = NULL;
    dx_set_error(DX_ERROR_EXISTS);
    return DX_FAILURE;
  }

  // begin sanity check
  if (parent) {
    // if a parent type is specified, then it must be an object type
    if (_DX_RTI_TYPE_NODE_FLAGS_OBJECT != (_DX_RTI_TYPE(parent)->flags & _DX_RTI_TYPE_NODE_FLAGS_OBJECT)) {
      // "parent type <parent> of child type <child> is not an object type"
      dx_log("parent type `", sizeof("parent type `") - 1);
      dx_log(_DX_RTI_TYPE(parent)->name->bytes, _DX_RTI_TYPE(parent)->name->number_of_bytes);
      dx_log("` of child type `", sizeof("` of child type `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` is not an object type\n", sizeof("` is not an object type\n") - 1);
      _dx_rti_type_name_unreference(name);
      name = NULL;
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    }
    // if a parent type is specified, its object size and its dispatch size must be smaller than or equal to the object size and the dispatch size of the of the child type.
    if (_DX_RTI_TYPE(parent)->object.object_size > object_size) {
      // "object size of parent type <parent> is greater than object size of child type <child>"
      dx_log("object size of parent type `", sizeof("object size of parent type `") - 1);
      dx_log(_DX_RTI_TYPE(parent)->name->bytes, _DX_RTI_TYPE(parent)->name->number_of_bytes);
      dx_log("` is greater than object size of child type `", sizeof("` is greater than object size of child type `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("`\n", sizeof("`\n") - 1);
      _dx_rti_type_name_unreference(name);
      name = NULL;
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    }
    if (_DX_RTI_TYPE(parent)->object.dispatch_size > dispatch_size) {
      // "dispatch size of parent type <parent> is greater than dispatch size of child type <child>"
      dx_log("dispatch size of parent type `", sizeof("dispatch size of parent type `") - 1);
      dx_log(_DX_RTI_TYPE(parent)->name->bytes, _DX_RTI_TYPE(parent)->name->number_of_bytes);
      dx_log("` is greater than dispatch size of child type `", sizeof("` is greater than dispatch size of child type `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("`\n", sizeof("`\n") - 1);
      _dx_rti_type_name_unreference(name);
      name = NULL;
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    }
  }
  // end sanity check

  if (dx_memory_allocate(&type, sizeof(_dx_rti_type))) {
    _dx_rti_type_name_unreference(name);
    name = NULL;
    return DX_FAILURE;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_OBJECT;
  type->name = name;
  type->reference_count = 1;
  
  type->object.object_size = object_size;
  type->object.destruct_object = destruct_object;

  type->object.dispatch_size = dispatch_size;
  type->object.construct_dispatch = construct_dispatch;
  type->object.dispatch = NULL;

  type->object.parent = _DX_RTI_TYPE(parent);
  if (type->object.parent) {
    _dx_rti_type_reference(type->object.parent);
  }
  // add the type
  if (dx_inline_pointer_hashmap_set(g_types, name, type)) {
    _dx_rti_type_unreference(type);
    type = NULL;
    return DX_FAILURE;
  }
  _dx_rti_type_unreference(type);
#if defined(_DEBUG)
  g_debug_number_of_types++;
#endif
  // Simply revert everything if this goes wrong.
  if (_dx_rti_type_ensure_dispatches_created(type)) {
    dx_inline_pointer_hashmap_remove(g_types, name);
    return DX_FAILURE;
  }
  *RETURN = (dx_rti_type*)type;
  return DX_SUCCESS;
}

static inline bool _dx_rti_type_is_leq(_dx_rti_type* x, _dx_rti_type* y) {
  if (!x || !y) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return false;
  }
  // If the x is an enumeration type or a fundamental type,
  // then it is lower than or equal to y only if x and y are the same type.
  if (_dx_rti_type_is_enumeration(x) || _dx_rti_type_is_fundamental(x)) {
    return x == y;
  }
  // Otherwise x is an object type.
  // It can be lower than or equal to y only if y is also an object type.
  if (!_dx_rti_type_is_object(y)) {
    return false;
  }
  _dx_rti_type* z = x;
  do {
    if (z == y) {
      return true;
    }
    z = _DX_RTI_TYPE(z->object.parent);
  } while (z != NULL);
  return false;
}

bool dx_rti_type_is_leq(dx_rti_type* x, dx_rti_type* y) {
  return _dx_rti_type_is_leq(_DX_RTI_TYPE(x), _DX_RTI_TYPE(y));
}

void* dx_rti_type_get_dispatch(dx_rti_type* x) {
  return _DX_RTI_TYPE(x)->object.dispatch;
}

dx_rti_type* dx_rti_type_get_parent(dx_rti_type* x) {
  _dx_rti_type* xx = _DX_RTI_TYPE(x);
  if (_dx_rti_type_is_object(xx)) {
    return (dx_rti_type*)xx->object.parent;
  }
  return NULL;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_rti_type* g_dx_object_type = NULL;

static void dx_object_on_type_destroyed() {
  g_dx_object_type = NULL;
}

dx_rti_type* dx_object_get_type() {
  if (!g_dx_object_type) {
    if (dx_rti_create_object(&g_dx_object_type, "dx.object", sizeof("dx.object") - 1, &dx_object_on_type_destroyed, NULL,
                             sizeof(dx_object), NULL,
                             sizeof(dx_object_dispatch), NULL)) {
      return g_dx_object_type;
    }
  }
  return g_dx_object_type;
}

void DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(void* p) {
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  DX_DEBUG_ASSERT(NULL != p);
  dx_object* q = DX_OBJECT(p);
  // 'O', 'B', 'J', 'E', 'T'' 
  DX_DEBUG_ASSERT(q->magic_bytes[0] == 'O');
  DX_DEBUG_ASSERT(q->magic_bytes[1] == 'B');
  DX_DEBUG_ASSERT(q->magic_bytes[2] == 'J');
  DX_DEBUG_ASSERT(q->magic_bytes[3] == 'E');
  DX_DEBUG_ASSERT(q->magic_bytes[4] == 'C');
  DX_DEBUG_ASSERT(q->magic_bytes[5] == 'T');
#endif
}

void DX_DEBUG_CHECK_OBJECT_DISPATCH_MAGIC_BYTES(void* p) {
#if _DEBUG && 1 == DX_OBJECT_DISPATCH_WITH_MAGIC_BYTES
  DX_DEBUG_ASSERT(NULL != p);
  dx_object_dispatch* q = DX_OBJECT_DISPATCH(p);
  // 'D', 'I', 'S', 'P', 'A', 'T', 'C', 'H'
  DX_DEBUG_ASSERT(q->magic_bytes[0] == 'D');
  DX_DEBUG_ASSERT(q->magic_bytes[1] == 'I');
  DX_DEBUG_ASSERT(q->magic_bytes[2] == 'S');
  DX_DEBUG_ASSERT(q->magic_bytes[3] == 'P');
  DX_DEBUG_ASSERT(q->magic_bytes[4] == 'A');
  DX_DEBUG_ASSERT(q->magic_bytes[5] == 'T');
  DX_DEBUG_ASSERT(q->magic_bytes[6] == 'C');
  DX_DEBUG_ASSERT(q->magic_bytes[7] == 'H');
#endif
}

dx_object* dx_object_alloc(dx_size size) {
  if (size < sizeof(dx_object)) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  dx_rti_type* type = dx_object_get_type();
  if (!type) {
    return NULL;
  }
  dx_object* object = NULL;
  if (dx_memory_allocate(&object, size)) {
    return NULL;
  }
  object->reference_count = 1;
  object->type = type;
  object->weak_references = NULL;
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  object->magic_bytes[0] = 'O';
  object->magic_bytes[1] = 'B';
  object->magic_bytes[2] = 'J';
  object->magic_bytes[3] = 'E';
  object->magic_bytes[4] = 'C';
  object->magic_bytes[5] = 'T';
#endif
  return object;
}

void dx_object_reference(dx_object *object) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(object);
  dx_reference_counter_increment(&object->reference_count);
}

void dx_object_unreference(dx_object* object) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(object);
  if (!dx_reference_counter_decrement(&object->reference_count)) {
    _acquire_weak_references_lock();
    while (object->weak_references) {
      dx_weak_reference* weak_reference = object->weak_references;
      object->weak_references = weak_reference->next;
      _detach_weak_reference(object, weak_reference);
    }
    _relinquish_weak_references_lock();
    while (object->type) {
      _dx_rti_type* type = (_dx_rti_type*)object->type;
      if (type->object.destruct_object) {
        type->object.destruct_object(object);
      }
      object->type = (dx_rti_type*)type->object.parent;
    }
    dx_memory_deallocate(object);
    object = NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


DX_DEFINE_OBJECT_TYPE("dx.weak_reference",
                      dx_weak_reference,
                      dx_object);

static void _acquire_weak_references_lock()
{/*Intentionally empty.*/}

static void _relinquish_weak_references_lock()
{/*Intentionally empty.*/}

static void _detach_weak_reference(dx_object* object, dx_weak_reference* weak_reference) {
  dx_weak_reference** previous = &(object->weak_references);
  dx_weak_reference* current = (object->weak_references);
  while (NULL != current) {
    if (current == weak_reference) {
      *previous = current->next;
      current->object = NULL;
      return;
    }
  }
  dx_log("warning: weak reference not found", sizeof("warning: weak reference not found"));
}

static void _attach_weak_reference(dx_object* object, dx_weak_reference* weak_reference) {
#if defined(_DEBUG)
  if (weak_reference->object) {
    dx_log("warning: weak reference already attached", sizeof("warning: weak reference already attached"));
  }
#endif
  weak_reference->next = object->weak_references;
  object->weak_references = weak_reference;
}

static void dx_weak_reference_destruct(dx_weak_reference* SELF) {
  _acquire_weak_references_lock();
  if (SELF->object) {
    _detach_weak_reference(SELF->object, SELF);
  }
  _relinquish_weak_references_lock();
}

static void dx_weak_reference_dispatch_construct(dx_weak_reference_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_weak_reference_construct(dx_weak_reference* SELF, dx_object* object) {
  dx_rti_type* TYPE = dx_weak_reference_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  _acquire_weak_references_lock();
  SELF->object = object;
  if (SELF->object) {
    _attach_weak_reference(SELF->object, SELF);
  }

  DX_OBJECT(SELF)->type = TYPE;
  _relinquish_weak_references_lock();

  return DX_SUCCESS;
}

dx_result dx_weak_reference_create(dx_weak_reference** RETURN, dx_object* object) {
  dx_weak_reference* SELF = DX_WEAK_REFERENCE(dx_object_alloc(sizeof(dx_weak_reference)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_weak_reference_construct(SELF, object)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_weak_reference_set(dx_weak_reference* SELF, dx_object* object) {
  _acquire_weak_references_lock();
  if (SELF->object) {
    _detach_weak_reference(SELF->object, SELF);
  }
  if (object) {
    _attach_weak_reference(object, SELF);
  }
  _relinquish_weak_references_lock();
  return DX_SUCCESS;
}

dx_result dx_weak_reference_acquire(dx_object** RETURN, dx_weak_reference* SELF) {
  if (SELF->object) {
    DX_REFERENCE(SELF->object);
  }
  *RETURN = SELF->object;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_FUNDAMENTAL_TYPE("dx.n8",
                           dx_n8);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.n16",
                           dx_n16);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.n32",
                           dx_n32);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.n64",
                           dx_n64);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.i8",
                           dx_i8);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.i16",
                           dx_i16);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.i32",
                           dx_i32);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.i64",
                           dx_i64);

/// @todo Should be "r32".
DX_DEFINE_FUNDAMENTAL_TYPE("dx.f32",
                           dx_f32);

/// @todo Should be "r64".
DX_DEFINE_FUNDAMENTAL_TYPE("dx.f64",
                           dx_f64);

DX_DEFINE_FUNDAMENTAL_TYPE("dx.bool",
                           dx_bool);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
