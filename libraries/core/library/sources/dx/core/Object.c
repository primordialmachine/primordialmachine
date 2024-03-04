#include "dx/core/Object.h"

#include "Core/Hash.h"
#include "dx/core/inline_pointer_hashmap.h"
#include "Core/Memory.h"
#include "dx/core/signals.library.h"
#include "dx/core/TypeSystem.library.h"

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

static void Core_Object_constructDispatch(Core_Object_Dispatch* SELF) {
#if _DEBUG && 1 == Core_withObjectDispatchMagicBytes
  SELF->magic_bytes[0] = 'D';
  SELF->magic_bytes[1] = 'I';
  SELF->magic_bytes[2] = 'S';
  SELF->magic_bytes[3] = 'P';
  SELF->magic_bytes[4] = 'A';
  SELF->magic_bytes[5] = 'T';
  SELF->magic_bytes[6] = 'C';
  SELF->magic_bytes[7] = 'H';
#endif
}

Core_Result Core_Object_getType(Core_Type** RETURN) {
  if (!_Core_Object_type) {
  #if _DEBUG && 1 == Core_withObjectDispatchMagicBytes
    if (Core_TypeSystem_defineObjectType(&_Core_Object_type,
                                         "Core.Object", sizeof("Core.Object") - 1,
                                         &Core_Object_onTypeDestroyed,
                                         NULL,
                                         sizeof(Core_Object), NULL,
                                         sizeof(Core_Object_Dispatch), &Core_Object_constructDispatch)) {
  #else
    if (Core_TypeSystem_defineObjectType(&_Core_Object_type,
                                         "Core.Object", sizeof("Core.Object") - 1,
                                         &Core_Object_onTypeDestroyed,
                                         NULL,
                                         sizeof(Core_Object), NULL,
                                         sizeof(Core_Object_Dispatch), NULL)) {
  #endif
      return Core_Failure;
    }
  }
  *RETURN = _Core_Object_type;
  return Core_Success;
}

#if _DEBUG && 1 == Core_withObjectMagicBytes
void Core_Debug_checkObjectMagicBytes(void* p) {
  DX_DEBUG_ASSERT(NULL != p);
  Core_Object* q = CORE_OBJECT(p);
  // 'O', 'B', 'J', 'E', 'C', 'T' 
  DX_DEBUG_ASSERT(q->magic_bytes[0] == 'O');
  DX_DEBUG_ASSERT(q->magic_bytes[1] == 'B');
  DX_DEBUG_ASSERT(q->magic_bytes[2] == 'J');
  DX_DEBUG_ASSERT(q->magic_bytes[3] == 'E');
  DX_DEBUG_ASSERT(q->magic_bytes[4] == 'C');
  DX_DEBUG_ASSERT(q->magic_bytes[5] == 'T');
}
#endif

#if _DEBUG && 1 == Core_withObjectDispatchMagicBytes
void Core_Debug_checkObjectDispatchMagicBytes(void* p) {
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
}
#endif

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
#if _DEBUG && 1 == Core_withObjectMagicBytes
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

void Core_reference(Core_Object *object) {
  Core_Debug_checkObjectMagicBytes(object);
  Core_ReferenceCounter_increment(&object->reference_count);
}

void Core_unreference(Core_Object* object) {
  Core_Debug_checkObjectMagicBytes(object);
  if (!Core_ReferenceCounter_decrement(&object->reference_count)) {
    // weak references
    _acquire_weak_references_lock();
    while (object->weak_references) {
      Core_WeakReference* weak_reference = object->weak_references;
      object->weak_references = object->weak_references->next;
      // invalidate the weak reference.
      weak_reference->object = NULL;
    }
    _relinquish_weak_references_lock();
    // signals
    Core_Error oldError = Core_getError();
    Core_Signals_disconnectAll(object);
    Core_setError(oldError);
    // destruct
    while (object->type) {
      _Core_Type* type = (_Core_Type*)object->type;
      if (type->object.destructObject) {
        type->object.destructObject(object);
      }
      object->type = (Core_Type*)type->object.parent;
    }
    // deallocate
    Core_Memory_deallocate(object);
    object = NULL;
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("Core.WeakReference",
                       Core_WeakReference,
                       Core_Object);

static void _acquire_weak_references_lock()
{/*Intentionally empty.*/}

static void _relinquish_weak_references_lock()
{/*Intentionally empty.*/}

static void Core_WeakReference_destruct(Core_WeakReference* SELF) {
  _acquire_weak_references_lock();
  if (SELF->object) {
    // Detach the weak reference from the old object.
    Core_WeakReference** previous = &(SELF->object->weak_references);
    Core_WeakReference* current = (SELF->object->weak_references);
    while (NULL != current) {
      if (current == SELF) {
        *previous = current->next;
        current->object = NULL;
        break;
      } else {
        previous = &current->next;
        current = current->next;
      }
    }
  }
  _relinquish_weak_references_lock();
}

static void Core_WeakReference_constructDispatch(Core_WeakReference_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_WeakReference_construct(Core_WeakReference* SELF, Core_Object* object) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Type* TYPE = NULL;
  if (Core_WeakReference_getType(&TYPE)) {
    return Core_Failure;
  }
  _acquire_weak_references_lock();
  SELF->object = object;
  if (SELF->object) {
    SELF->next = object->weak_references;
    object->weak_references = SELF;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  _relinquish_weak_references_lock();
  return Core_Success;
}

Core_Result Core_WeakReference_create(Core_WeakReference** RETURN, Core_Object* object) {
  DX_CREATE_PREFIX(Core_WeakReference);
  if (Core_WeakReference_construct(SELF, object)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_WeakReference_set(Core_WeakReference* SELF, Core_Object* object) {
  _acquire_weak_references_lock();
  if (SELF->object) {
    // Detach the weak reference from the old object.
    Core_WeakReference** previous = &(object->weak_references);
    Core_WeakReference* current = (object->weak_references);
    while (NULL != current) {
      if (current == SELF) {
        *previous = current->next;
        current->object = NULL;
        break;
      } else {
        previous = &current->next;
        current = current->next;
      }
    }
  }
  if (object) {
    // Attach weak reference to the new object.
    SELF->next = object->weak_references;
    object->weak_references = SELF;
  }
  _relinquish_weak_references_lock();
  return Core_Success;
}

Core_Result Core_WeakReference_acquire(Core_Object** RETURN, Core_WeakReference* SELF) {
  if (SELF->object) {
    CORE_REFERENCE(SELF->object);
  }
  *RETURN = SELF->object;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
