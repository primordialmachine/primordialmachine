#include "dx/core/object.h"

#include "Core/Hash.h"
#include "dx/core/inline_pointer_hashmap.h"
#include "Core/Memory.h"
#include "dx/core/signals.library.h"
#include "dx/core/typesystem.library.h"

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
static void _detach_weak_reference(Core_Object* object, Core_WeakReference* weak_reference);

/// @brief Attach a weak reference to an object.
/// @param object a pointer to the object.
/// @param weak_reference A pointer to the weak reference.
/// @undefined The weak reference is attached to an object object.
/// @undefuned The weak references lock was not acquired.
static void _attach_weak_reference(Core_Object* object, Core_WeakReference* weak_reference);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_Runtime_initialize() {
  if (Core_TypeSystem_initialize()) {
    return Core_Failure;
  }
  if (Core_Signals_initialize()) {
    Core_TypeSystem_uninitialize();
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Runtime_uninitialize() {
  Core_Signals_uninitialize();
  Core_TypeSystem_uninitialize();
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Type* _Core_Object_type = NULL;

static void Core_Object_onTypeDestroyed() {
  _Core_Object_type = NULL;
}

Core_Result Core_Object_getType(Core_Type** RETURN) {
  if (!_Core_Object_type) {
    if (Core_TypeSystem_defineObjectType(&_Core_Object_type,
                                         "Core.Object", sizeof("Core.Object") - 1,
                                         &Core_Object_onTypeDestroyed,
                                         NULL,
                                         sizeof(Core_Object), NULL,
                                         sizeof(Core_Object_Dispatch), NULL)) {
      return Core_Failure;
    }
  }
  *RETURN = _Core_Object_type;
  return Core_Success;
}

void DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(void* p) {
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  DX_DEBUG_ASSERT(NULL != p);
  Core_Object* q = CORE_OBJECT(p);
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
  Core_Object_Dispatch* q = CORE_OBJECT_DISPATCH(p);
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

Core_Result Core_Object_allocate(Core_Object** RETURN, Core_Size size) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (size < sizeof(Core_Object)) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (Core_Object_getType(&type)) {
    return Core_Failure;
  }
  Core_Object* object = NULL;
  if (Core_Memory_allocate(&object, size)) {
    return Core_Failure;
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
  *RETURN = object;
  return Core_Success;
}

void Core_Object_reference(Core_Object *object) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(object);
  dx_reference_counter_increment(&object->reference_count);
}

void Core_Object_unreference(Core_Object* object) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(object);
  if (!dx_reference_counter_decrement(&object->reference_count)) {
    _acquire_weak_references_lock();
    while (object->weak_references) {
      Core_WeakReference* weak_reference = object->weak_references;
      object->weak_references = weak_reference->next;
      _detach_weak_reference(object, weak_reference);
    }
    _relinquish_weak_references_lock();
    while (object->type) {
      _dx_rti_type* type = (_dx_rti_type*)object->type;
      if (type->object.destruct_object) {
        type->object.destruct_object(object);
      }
      object->type = (Core_Type*)type->object.parent;
    }
    Core_Memory_deallocate(object);
    object = NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("Core.WeakReference",
                       Core_WeakReference,
                       Core_Object);

static void _acquire_weak_references_lock()
{/*Intentionally empty.*/}

static void _relinquish_weak_references_lock()
{/*Intentionally empty.*/}

static void _detach_weak_reference(Core_Object* object, Core_WeakReference* weak_reference) {
  Core_WeakReference** previous = &(object->weak_references);
  Core_WeakReference* current = (object->weak_references);
  while (NULL != current) {
    if (current == weak_reference) {
      *previous = current->next;
      current->object = NULL;
      return;
    }
  }
  dx_log("warning: weak reference not found", sizeof("warning: weak reference not found"));
}

static void _attach_weak_reference(Core_Object* object, Core_WeakReference* weak_reference) {
#if defined(_DEBUG)
  if (weak_reference->object) {
    dx_log("warning: weak reference already attached", sizeof("warning: weak reference already attached"));
  }
#endif
  weak_reference->next = object->weak_references;
  object->weak_references = weak_reference;
}

static void Core_WeakReference_destruct(Core_WeakReference* SELF) {
  _acquire_weak_references_lock();
  if (SELF->object) {
    _detach_weak_reference(SELF->object, SELF);
  }
  _relinquish_weak_references_lock();
}

static void Core_WeakReference_constructDispatch(Core_WeakReference_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_WeakReference_construct(Core_WeakReference* SELF, Core_Object* object) {
  DX_CONSTRUCT_PREFIX(Core_WeakReference);

  _acquire_weak_references_lock();

  SELF->object = object;
  if (SELF->object) {
    _attach_weak_reference(SELF->object, SELF);
  }

  CORE_OBJECT(SELF)->type = TYPE;
  _relinquish_weak_references_lock();

  return Core_Success;
}

Core_Result Core_WeakReference_create(Core_WeakReference** RETURN, Core_Object* object) {
  DX_CREATE_PREFIX(Core_WeakReference);
  if (Core_WeakReference_construct(SELF, object)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_WeakReference_set(Core_WeakReference* SELF, Core_Object* object) {
  _acquire_weak_references_lock();
  if (SELF->object) {
    _detach_weak_reference(SELF->object, SELF);
  }
  if (object) {
    _attach_weak_reference(object, SELF);
  }
  _relinquish_weak_references_lock();
  return Core_Success;
}

Core_Result Core_WeakReference_acquire(Core_Object** RETURN, Core_WeakReference* SELF) {
  if (SELF->object) {
    DX_REFERENCE(SELF->object);
  }
  *RETURN = SELF->object;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Natural8",
    Core_Natural8
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Natural16",
    Core_Natural16
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Natural32",
    Core_Natural32
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Natural64",
    Core_Natural64
  );



DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Integer8",
    Core_Integer8
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Integer16",
    Core_Integer16
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Integer32",
    Core_Integer32
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Integer64",
    Core_Integer64
  );



DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Real32",
    Core_Real32
  );

DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Real64",
    Core_Real64
  );



DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Boolean",
    Core_Boolean
  );



DX_DEFINE_FUNDAMENTAL_TYPE
  (
    "Core.Size",
    Core_Size
  );



/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
