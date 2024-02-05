#include "dx/core/typesystem.h"

#include "Core/Hash.h"
#include "dx/core/typesystem.library.h"
#include "Core/Memory.h"
#include "dx/core/inline_pointer_hashmap.h"

static Core_Result _dx_rti_type_name_create(_dx_rti_type_name** RETURN, char const* bytes, Core_Size number_of_bytes);

static Core_Result _dx_rti_type_name_hash_key_callback(Core_Size* RETURN, _dx_rti_type_name** a);

static Core_Result _dx_rti_type_name_compare_keys_callback(Core_Boolean* RETURN, _dx_rti_type_name** a, _dx_rti_type_name** b);

static void _dx_rti_type_name_reference(_dx_rti_type_name* a);

static void _dx_rti_type_name_unreference(_dx_rti_type_name* a);

static void _dx_rti_type_name_reference_callback(_dx_rti_type_name** a);

static void _dx_rti_type_name_unreference_callback(_dx_rti_type_name** a);

#define _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL (1)
#define _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION (2)
#define _DX_RTI_TYPE_NODE_FLAGS_OBJECT (4)

#if defined(_DEBUG)
static Core_Size g_debug_number_of_types = 0;
#endif

static inline _dx_rti_type* _DX_RTI_TYPE(void* p) {
  return (_dx_rti_type*)p;
}

/// @internal
/// @brief Get if a type is a fundamental type.
/// @param ty A pointer to the type.
/// @return @a true if the type is a fundamental type. @a false if it is not a fundamental type.
/// @a false is also returned on failure.
/// @default-failure
static Core_Boolean _dx_rti_type_is_fundamental(_dx_rti_type* ty);

/// @internal
/// @brief Get if a type is an enumeration type.
/// @param ty A pointer to the type.
/// @return @a true if the type is an enumeration type. @a false if it is not an enumeration type.
/// @a false is also returned on failure.
/// @default-failure
static Core_Boolean _dx_rti_type_is_enumeration(_dx_rti_type* ty);

/// @internal
/// @brief Get if a type is an object type.
/// @param ty A pointer to the type.
/// @return @a true if the type is an objec type. @a false if it is not an object type.
/// @a false is also returned on failure.
/// @default-failure
static Core_Boolean _dx_rti_type_is_object(_dx_rti_type* ty);

/// @internal
/// @brief Ensure the dispatches of a type are created.
/// This function does nothing if the type is not an object type.
/// @param ty A pointer to the type.
/// @default-return
static Core_Result _dx_rti_type_ensure_Dispatches_created(_dx_rti_type* ty);

/// @internal
/// @brief Ensure the dispatches of a type are destroyed.
/// @param ty A pointer to the type.
/// @return
/// #DX_SUCCESS on success. #Core_Failure on failure.
/// Always succeeds.
/// @undefined
/// @a ty does not point to a type.
static Core_Result _dx_rti_type_ensure_Dispatches_destroyed(_dx_rti_type* ty);

static Core_Boolean _dx_rti_type_is_fundamental(_dx_rti_type* ty) {
  return _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL == (ty->flags & _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL);
}

static Core_Boolean _dx_rti_type_is_enumeration(_dx_rti_type* ty) {
  return _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION == (ty->flags & _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION);
}

static Core_Boolean _dx_rti_type_is_object(_dx_rti_type* ty) {
  return _DX_RTI_TYPE_NODE_FLAGS_OBJECT == (ty->flags & _DX_RTI_TYPE_NODE_FLAGS_OBJECT);
}

static Core_Result _dx_rti_type_ensure_Dispatches_created(_dx_rti_type* ty) {
  if (!_dx_rti_type_is_object(ty)) {
    return Core_Success;
  }
  if (ty->object.dispatch) {
    return Core_Success;
  }
  if (ty->object.parent) {
    if (_dx_rti_type_ensure_Dispatches_created(ty->object.parent)) {
      return Core_Failure;
    }
  }
  ty->object.dispatch = NULL;
  if (Core_Memory_allocate(&ty->object.dispatch, ty->object.dispatch_size)) {
    return Core_Failure;
  }
  Core_Memory_zero(ty->object.dispatch, ty->object.dispatch_size);
  if (ty->object.parent) {
    Core_Memory_copy(ty->object.dispatch, ty->object.parent->object.dispatch, ty->object.parent->object.dispatch_size);
  }
  if (ty->object.constructDispatch) {
    ty->object.constructDispatch(ty->object.dispatch);
  }
  return Core_Success;
}

static Core_Result _dx_rti_type_ensure_Dispatches_destroyed(_dx_rti_type* ty) {
  if (!_dx_rti_type_is_object(ty)) {
    return Core_Success;
  }
  if (!ty->object.dispatch) {
    return Core_Success;
  }
  Core_Memory_deallocate(ty->object.dispatch);
  ty->object.dispatch = NULL;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result _dx_rti_type_name_create(_dx_rti_type_name** RETURN, char const* bytes, Core_Size number_of_bytes) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _dx_rti_type_name* SELF = NULL;
  if (Core_Memory_allocate(&SELF, sizeof(_dx_rti_type_name) + number_of_bytes)) {
    return Core_Failure;
  }
  if (Core_hashBytes(&SELF->hash_value, bytes, number_of_bytes)) {
    Core_Memory_deallocate(SELF);
    return Core_Failure;
  }
  Core_Memory_copy(SELF->bytes, bytes, number_of_bytes);
  SELF->number_of_bytes = number_of_bytes;
  SELF->reference_count = 1;
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result _dx_rti_type_name_hash_key_callback(Core_Size* RETURN, _dx_rti_type_name** a) {
  *RETURN = (*a)->hash_value;
  return Core_Success;
}

static Core_Result _dx_rti_type_name_compare_keys_callback(Core_Boolean* RETURN, _dx_rti_type_name** a, _dx_rti_type_name** b) {
  if (*a == *b) {
    *RETURN = true;
  } else if ((*a)->number_of_bytes == (*b)->number_of_bytes) {
    Core_Natural8 temporary;
    Core_Memory_compare(&temporary, (*a)->bytes, (*b)->bytes, (*a)->number_of_bytes); // must succeed
    *RETURN = !temporary;
  } else {
    *RETURN = false;
  }
  return Core_Success;
}

static void _dx_rti_type_name_reference(_dx_rti_type_name* a) {
  dx_reference_counter_increment(&a->reference_count);
}

static void _dx_rti_type_name_unreference(_dx_rti_type_name* a) {
  if (0 == dx_reference_counter_decrement(&a->reference_count)) {
    Core_Memory_deallocate(a);
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
      _dx_rti_type_ensure_Dispatches_destroyed(a);
    }
    _dx_rti_type_name_unreference(a->name);
    a->name = NULL;
    Core_Memory_deallocate(a);
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

static Core_InlineHashMapPP* g_types = NULL;

Core_Result Core_TypeSystem_initialize() {
  static Core_InlineHashMapPP_Configuration const configuration = {
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*) & _dx_rti_type_name_compare_keys_callback,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*) & _dx_rti_type_name_hash_key_callback,
    .keyAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*) & _dx_rti_type_name_reference_callback,
    .keyRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*) & _dx_rti_type_name_unreference_callback,
    .valueAddedCallback = (Core_InlineHashMapPP_ValueAddedCallback*) & _dx_rti_type_reference_callback,
    .valueRemovedCallback = (Core_InlineHashMapPP_ValueRemovedCallback*) & _dx_rti_type_unreference_callback,
  };
  if (Core_Memory_allocate(&g_types, sizeof(Core_InlineHashMapPP))) {
    return Core_Failure;
  }
  if (Core_InlineHashMapPP_initialize(g_types, &configuration)) {
    Core_Memory_deallocate(g_types);
    g_types = NULL;
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_TypeSystem_uninitialize() {
  Core_InlineHashMapPP_uninitialize(g_types);
  Core_Memory_deallocate(g_types);
  g_types = NULL;
  return Core_Success;
}

Core_Result Core_TypeSystem_isFundamentalType(Core_Boolean* RETURN, Core_Type* type) {
  if (!RETURN || !type) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = _dx_rti_type_is_fundamental(_DX_RTI_TYPE(type));
  return Core_Success;
}

Core_Result Core_TypeSystem_isEnumerationType(Core_Boolean* RETURN, Core_Type* type) {
  if (!RETURN || !type) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = _dx_rti_type_is_enumeration(_DX_RTI_TYPE(type));
  return Core_Success;
}

Core_Result Core_TypeSystem_isObjectType(Core_Boolean* RETURN, Core_Type* type) {
  if (!RETURN || !type) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = _dx_rti_type_is_object(_DX_RTI_TYPE(type));
  return Core_Success;
}

Core_Result Core_TypeSystem_defineEnumerationType(Core_Type** RETURN, char const* p, Core_Size n, void (*on_type_destroyed)()) {
  _dx_rti_type_name* name = NULL;
  if (_dx_rti_type_name_create(&name, p, n)) {
    return Core_Failure;
  }
  _dx_rti_type* type = NULL;
  if (Core_InlineHashMapPP_get(&type, g_types, name)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  } else {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    Core_setError(Core_Error_Exists);
    _dx_rti_type_name_unreference(name);
    name = NULL;
    return Core_Failure;
  }
  if (Core_Memory_allocate(&type, sizeof(_dx_rti_type))) {
    return Core_Failure;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_ENUMERATION;
  type->name = name;
  type->reference_count = 1;
  // add the type
  if (Core_InlineHashMapPP_set(g_types, name, type)) {
    _dx_rti_type_unreference(type);
    type = NULL;
    return Core_Failure;
  }
  _dx_rti_type_unreference(type);
#if defined(_DEBUG)
  g_debug_number_of_types++;
#endif
  * RETURN = (Core_Type*)type;
  return Core_Success;
}

Core_Result Core_TypeSystem_defineFundamentalType(Core_Type** RETURN, char const* p, Core_Size n, void (*on_type_destroyed)(), Core_Size value_size) {
  _dx_rti_type_name* name = NULL;
  if (_dx_rti_type_name_create(&name, p, n)) {
    return Core_Failure;
  }
  _dx_rti_type* type = NULL;
  if (Core_InlineHashMapPP_get(&type, g_types, name)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  } else {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    Core_setError(Core_Error_Exists);
    _dx_rti_type_name_unreference(name);
    name = NULL;
    return Core_Failure;
  }
  if (Core_Memory_allocate(&type, sizeof(_dx_rti_type))) {
    return Core_Failure;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_FUNDAMENTAL;
  type->name = name;
  //_dx_rti_type_name_reference(type->name);
  type->reference_count = 1;
  type->fundamental.value_size = value_size;
  // add the type
  if (Core_InlineHashMapPP_set(g_types, name, type)) {
    _dx_rti_type_unreference(type);
    type = NULL;
    return Core_Failure;
  }
  *RETURN = (Core_Type*)type;
  return Core_Success;
}

Core_Result Core_TypeSystem_defineObjectType(Core_Type** RETURN, char const* p, Core_Size n, void (*on_type_destroyed)(), Core_Type* parent,
  Core_Size object_size, void (*destruct_object)(Core_Object*),
  Core_Size dispatch_size, void (*constructDispatch)(Core_Object_Dispatch*)) {
  _dx_rti_type_name* name = NULL;
  if (_dx_rti_type_name_create(&name, p, n)) {
    return Core_Failure;
  }
  _dx_rti_type* type = NULL;
  if (Core_InlineHashMapPP_get(&type, g_types, name)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  } else {
    dx_log("a type of the name `", sizeof("a type of the name `") - 1);
    dx_log(p, n);
    dx_log("` already exists", sizeof("` already exists") - 1);
    _dx_rti_type_name_unreference(name);
    name = NULL;
    Core_setError(Core_Error_Exists);
    return Core_Failure;
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
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
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
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
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
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
  }
  // end sanity check

  if (Core_Memory_allocate(&type, sizeof(_dx_rti_type))) {
    _dx_rti_type_name_unreference(name);
    name = NULL;
    return Core_Failure;
  }
  type->on_type_destroyed = on_type_destroyed;
  type->flags = _DX_RTI_TYPE_NODE_FLAGS_OBJECT;
  type->name = name;
  type->reference_count = 1;

  type->object.object_size = object_size;
  type->object.destruct_object = destruct_object;

  type->object.dispatch_size = dispatch_size;
  type->object.constructDispatch = constructDispatch;
  type->object.dispatch = NULL;

  type->object.parent = _DX_RTI_TYPE(parent);
  if (type->object.parent) {
    _dx_rti_type_reference(type->object.parent);
  }
  // add the type
  if (Core_InlineHashMapPP_set(g_types, name, type)) {
    _dx_rti_type_unreference(type);
    type = NULL;
    return Core_Failure;
  }
  _dx_rti_type_unreference(type);
#if defined(_DEBUG)
  g_debug_number_of_types++;
#endif
  // Simply revert everything if this goes wrong.
  if (_dx_rti_type_ensure_Dispatches_created(type)) {
    Core_InlineHashMapPP_remove(g_types, name);
    return Core_Failure;
  }
  *RETURN = (Core_Type*)type;
  return Core_Success;
}

static inline bool _dx_rti_type_is_leq(_dx_rti_type* x, _dx_rti_type* y) {
  if (!x || !y) {
    Core_setError(Core_Error_ArgumentInvalid);
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

Core_Result Core_Type_isLowerThanOrEqualTo(Core_Boolean* RETURN, Core_Type* x, Core_Type* y) {
  *RETURN = _dx_rti_type_is_leq(_DX_RTI_TYPE(x), _DX_RTI_TYPE(y));
  return Core_Success;
}

void* Core_Type_getDispatch(Core_Type* x) {
  return _DX_RTI_TYPE(x)->object.dispatch;
}

Core_Result Core_Type_getParent(Core_Type** RETURN, Core_Type* SELF) {
  _dx_rti_type* SELF_ = _DX_RTI_TYPE(SELF);
  if (_dx_rti_type_is_object(SELF_)) {
    *RETURN = (Core_Type*)SELF_->object.parent;
    return Core_Success;
  } else {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
}

Core_Result Core_Type_getName(Core_Type* SELF, char const** p, Core_Size* n) {
  _dx_rti_type* SELF_ = _DX_RTI_TYPE(SELF);
  *p = SELF_->name->bytes;
  *n = SELF_->name->number_of_bytes;
  return Core_Success;
}
