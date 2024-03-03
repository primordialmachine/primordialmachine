#include "dx/core/signals.h"

#include "Core/Hash.h"
#include "Core/Collections/InlineArrayListP.h"
#include "Core/Memory.h"
#include "dx/core/inline_pointer_hashmap.h"
#include <string.h>

// key of a signal
typedef struct _SignalKey _SignalKey;

// a signal
typedef struct _Signal _Signal;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// A connection consists of a context, a callback and the signal.
typedef struct _SignalConnection {
  struct _SignalConnection* next;
  _Signal* signal;
  Core_WeakReference* contextWeakReference;
  Core_Callback* callback;
} _SignalConnection;

typedef struct _SignalConnectionList {
  _SignalConnection* head;
} _SignalConnectionList;

static void _SignalConnectionList_destroy(_SignalConnectionList* x) {
  while (x->head) {
    _SignalConnection* y = x->head;
    x->head = x->head->next;
    CORE_UNREFERENCE(y->contextWeakReference);
    y->callback = NULL;
    Core_Memory_deallocate(y);
  }
  Core_Memory_deallocate(x);
}

typedef struct _SignalConnectionManager {
  // hash map from instances to _SignalConnectionList.
  Core_InlineHashMapPP connections;
} _SignalConnectionManager;

static Core_Result _SignalConnectionManager_compareKeys(Core_Boolean* RETURN, Core_Object** x, Core_Object** y) {
  return *x == *y;
}

static Core_Result _SignalConnectionManager_hashKey(Core_Size* RETURN, Core_Object** x) {
  return Core_hashPointer(RETURN, *x);
}

static void _SignalConnectionManager_removedValue(_SignalConnectionList** x) {
  _SignalConnectionList_destroy(*x);
}

static Core_Result _SignalConnectionManager_initialize(_SignalConnectionManager* connectionManager) {
  Core_InlineHashMapPP_Configuration configuration = {
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*) & _SignalConnectionManager_compareKeys,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*) & _SignalConnectionManager_hashKey,
    .keyAddedCallback = NULL,
    .keyRemovedCallback = NULL,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = (Core_InlineHashMapPP_ValueRemovedCallback*) & _SignalConnectionManager_removedValue,
  };
  /* @todo Fix naming. */
  if (Core_InlineHashMapPP_initialize(&connectionManager->connections, &configuration)) {
    return Core_Failure;
  }
  return Core_Success;
}

static void _SignalConnectionManager_uninitialize(_SignalConnectionManager* connectionManager) {
  Core_InlineHashMapPP_uninitialize(&connectionManager->connections);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct _SignalKey {
  /// @brief The name of the signal.
  /// The bytes are not owned by this object.
  struct {
    char* p;
    Core_Size n;
  } name;
  /// @brief The type of the signal.
  /// @remarks Types are not destroyed *before* the signals are destroyed.
  Core_Type* type;
  /// @brief The hash value of the signal.
  Core_Size hashValue;
};

struct _Signal {
  Core_ReferenceCounter referenceCount;
  _SignalKey key;
};

static Core_Result _Signal_destroy(_Signal* signal);

static void _Signal_valueAdded(_Signal** SELF);

static void _Signal_valueRemoved(_Signal** SELF);

static Core_Result _SignalKey_hashKey(Core_Size* RETURN, _SignalKey** SELF);

static Core_Result _SignalKey_compareKeys(Core_Boolean* RETURN, _SignalKey** SELF, _SignalKey** other);

static Core_Result _Signal_destroy(_Signal* signal) {
  //_SignalConnectionManager_uninitialize(&signal->manager);
  Core_Memory_deallocate(signal->key.name.p);
  signal->key.name.p = NULL;
  Core_Memory_deallocate(signal);
  return Core_Success;
}

static void _Signal_valueAdded(_Signal** SELF) {
  Core_ReferenceCounter_increment(&(*SELF)->referenceCount);
}

static void _Signal_valueRemoved(_Signal** SELF) {
  if (!Core_ReferenceCounter_decrement(&(*SELF)->referenceCount)) {
    _Signal_destroy(*SELF);
  }
}

static Core_Result _SignalKey_hashKey(Core_Size* RETURN, _SignalKey** SELF) {
  *RETURN = (*SELF)->hashValue;
  return Core_Success;
}

static Core_Result _SignalKey_compareKeys(Core_Boolean* RETURN, _SignalKey** SELF, _SignalKey** other) {
  if (*SELF == *other) {
    *RETURN = true;
    return Core_Success;
  }
  if ((*SELF)->hashValue != (*other)->hashValue || (*SELF)->type != (*other)->type || (*SELF)->name.n != (*other)->name.n) {
    *RETURN = false;
    return Core_Success;
  }
  Core_Integer8 temporary;
  if (Core_Memory_compare(&temporary, (*SELF)->name.p, (*other)->name.p, (*SELF)->name.n)) {
    return Core_Failure;
  }
  *RETURN = 0 == temporary;
  return Core_Success;
}

static Core_Boolean g_initialized = false;

static Core_InlineHashMapPP* g_signals = NULL;

static _SignalConnectionManager g_signal_connection_manager = { 0 };

Core_Result Core_Signals_initialize() {
  if (Core_Memory_allocate((void**)&g_signals, sizeof(Core_InlineHashMapPP))) {
    return Core_Failure;
  }
  Core_InlineHashMapPP_Configuration configuration = {
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&_SignalKey_compareKeys,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&_SignalKey_hashKey,
    .keyAddedCallback = NULL,
    .keyRemovedCallback = NULL,
    .valueAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*)&_Signal_valueAdded,
    .valueRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*)&_Signal_valueRemoved,
  };
  if (Core_InlineHashMapPP_initialize(g_signals, &configuration)) {
    return Core_Failure;
  }
  if (_SignalConnectionManager_initialize(&g_signal_connection_manager)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Signals_uninitialize() {
  _SignalConnectionManager_uninitialize(&g_signal_connection_manager);
  Core_InlineHashMapPP_uninitialize(g_signals);
  Core_Memory_deallocate(g_signals);
  g_signals = NULL;
  return Core_Success;
}

static Core_Result create(Core_Type* type, char const* p, Core_Size n);

static Core_Result get(_Signal** RETURN, Core_Type* type, char const* p, Core_Size n);

static Core_Result create(Core_Type* type, char const* p, Core_Size n) {
  _SignalKey key;
  key.name.p = (char*)p;
  key.name.n = n;
  Core_Size nameHashValue = 0;
  Core_hashBytes(&nameHashValue, p, n); // must succeed
  nameHashValue *= 37;
  Core_Boolean isObjectType;
  if (Core_TypeSystem_isObjectType(&isObjectType, type)) {
    return Core_Failure;
  }
  if (isObjectType) {
    // object
    Core_Type* t = type;
    do {
      char const* typeNameBytes;
      Core_Size typeNameLength;
      Core_Type_getName(t, &typeNameBytes, &typeNameLength); // must succeed
      Core_Size typeHashValue;
      Core_hashBytes(&typeHashValue, typeNameBytes, typeNameLength); // must succeed
      key.type = t;
      key.hashValue = nameHashValue + typeHashValue;

      /*check*/
      _Signal* signal = NULL;
      if (Core_InlineHashMapPP_get((void**)&signal, g_signals, &key)) {
        if (Core_Error_NotFound != Core_getError()) {
          return Core_Failure;
        }
        Core_setError(Core_Error_NoError);
      } else {
        Core_setError(Core_Error_Exists);
        return Core_Failure;
      }
      Core_Type_getParent(&t, t); // must succeed
    } while (NULL != t);
  } else {
    // enumeration or fundamental
    char const* typeNameBytes;
    Core_Size typeNameLength;
    Core_Type_getName(type, &typeNameBytes, &typeNameLength); // must succeed
    Core_Size typeHashValue;
    Core_hashBytes(&typeHashValue, typeNameBytes, typeNameLength); // must succeed
    key.hashValue = nameHashValue + typeHashValue;

    /*check*/
    _Signal* signal = NULL;
    if (Core_InlineHashMapPP_get((void**)&signal, g_signals, &key)) {
      if (Core_Error_NotFound != Core_getError()) {
        return Core_Failure;
      }
      Core_setError(Core_Error_NoError);
    } else {
      Core_setError(Core_Error_Exists);
      return Core_Failure;
    }
  }
  //
  _Signal* signal = NULL;
  if (Core_Memory_allocate((void**)&signal, sizeof(_Signal))) {
    return Core_Failure;
  }
  if (Core_Memory_allocate((void**)&signal->key.name, n)) {
    Core_Memory_deallocate(signal);
    signal = NULL;
    return Core_Failure;
  }
  if (Core_Memory_copy(signal->key.name.p, p, n)) {
    Core_Memory_deallocate(signal->key.name.p);
    signal->key.name.p = NULL;
    Core_Memory_deallocate(signal);
    signal = NULL;
    return Core_Failure;
  }
  signal->key.name.n = n;
  char const* typeNameBytes = NULL;
  Core_Size typeNameLength = 0;
  Core_Type_getName(type, &typeNameBytes, &typeNameLength); // must succeed
  Core_Size typeHashValue = 0;
  Core_hashBytes(&typeHashValue, typeNameBytes, typeNameLength); // must succeed
  signal->key.hashValue = nameHashValue + typeHashValue;
  signal->key.type = type;
  signal->referenceCount = 0;
  if (Core_InlineHashMapPP_set(g_signals, &signal->key, signal)) {
    Core_Memory_deallocate(signal->key.name.p);
    signal->key.name.p = NULL;
    Core_Memory_deallocate(signal);
    signal = NULL;
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result get(_Signal** RETURN, Core_Type* type, char const* p, Core_Size n) {
  _SignalKey key;
  key.name.p = (char*)p;
  key.name.n = n;
  Core_Size nameHashValue;
  if (Core_hashBytes(&nameHashValue, p, n)) {
    return Core_Failure;
  }
  nameHashValue *= 37;
  Core_Boolean isObjectType;
  if (Core_TypeSystem_isObjectType(&isObjectType, type)) {
    return Core_Failure;
  }
  if (isObjectType) {
    // object
    Core_Type* t = type;
    do {
      char const* typeNameBytes = NULL;
      Core_Size typeNameLength = 0;
      Core_Type_getName(t, &typeNameBytes, &typeNameLength); // must succeed
      Core_Size typeHashValue = 0;
      Core_hashBytes(&typeHashValue, typeNameBytes, typeNameLength); // must succeed
      key.type = t;
      key.hashValue = nameHashValue + typeHashValue;

      /*check*/
      _Signal* signal = NULL;
      if (Core_InlineHashMapPP_get((void**)&signal, g_signals, &key)) {
        if (Core_Error_NotFound != Core_getError()) {
          return Core_Failure;
        }
        Core_setError(Core_Error_NoError);
      } else {
        *RETURN = signal;
        return Core_Success;
      }
      Core_Type_getParent(&t, t); // must succeed
    } while (NULL != t);
  } else {
    // enumeration or fundamental
    char const* typeNameBytes = NULL;
    Core_Size typeNameLength = 0;
    Core_Type_getName(type, &typeNameBytes, &typeNameLength); // must succeed
    Core_Size typeHashValue = 0;
    Core_hashBytes(&typeHashValue, typeNameBytes, typeNameLength); // must succeed
    key.hashValue = nameHashValue + typeHashValue;

    /*check*/
    _Signal* signal = NULL;
    if (Core_InlineHashMapPP_get((void**)&signal, g_signals, &key)) {
      if (Core_Error_NotFound != Core_getError()) {
        return Core_Failure;
      }
      Core_setError(Core_Error_NoError);
    } else {
      *RETURN = signal;
      return Core_Success;
    }
  }
  Core_setError(Core_Error_NotFound);
  return Core_Failure;
}

Core_Result Core_Signals_add(Core_Type* type, char const* p, Core_Size n) {
  return create(type, p, n);
}

Core_Result Core_Signals_connect(void** RETURN, Core_Object* instance, char const* p, Core_Size n, Core_Object* context, Core_Callback* callback) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _Signal* signal = NULL;
  Core_Type* type = instance->type;
  if (get(&signal, type, p, n)) {
    return Core_Failure;
  }
  _SignalConnection* connection = NULL;
  if (Core_Memory_allocate((void**)&connection, sizeof(_SignalConnection))) {
    return Core_Failure;
  }
  if (Core_WeakReference_create(&connection->contextWeakReference, context)) {
    Core_Memory_deallocate(signal);
    signal = NULL;
    return Core_Failure;
  }
  connection->callback = callback;
  connection->signal = signal;

  // Remove all connections from the specified instance.
  _SignalConnectionList* lst = NULL;
  if (Core_InlineHashMapPP_get((void**)&lst, &g_signal_connection_manager.connections, instance)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    }
    //
    if (Core_Memory_allocate((void**)&lst, sizeof(_SignalConnectionList))) {
      return Core_Failure;
    }
    lst->head = NULL;
    if (Core_InlineHashMapPP_set(&g_signal_connection_manager.connections, instance, lst)) {
      Core_Memory_deallocate(lst);
      lst = NULL;
      return Core_Failure;
    }
  }

  connection->next = lst->head;
  lst->head = connection;

  *RETURN = connection;
  return Core_Success;
}

Core_Result Core_Signals_invoke(Core_Object* instance, char const* p, Core_Size n, Core_Object* argument) {
  _Signal* signal = NULL;
  Core_Type* type = instance->type;
  if (get(&signal, type, p, n)) {
    return Core_Failure;
  }
  // Remove all connections from the specified instance.
  _SignalConnectionList* lst = NULL;
  if (Core_InlineHashMapPP_get((void**)&lst, &g_signal_connection_manager.connections, instance)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      return Core_Success;
    }
  }
  _SignalConnection* connection = lst->head;
  while (connection) {
    // ToDo: Use a sorted list for early abort.
    if (connection->signal == signal) {
      Core_Object* object = NULL;
      if (Core_WeakReference_acquire(&object, connection->contextWeakReference)) {
        return Core_Failure;
      }
      if (object) {
        if (connection->callback(object, argument)) {
          CORE_UNREFERENCE(object);
          object = NULL;
          return Core_Failure;
        } else {
          CORE_UNREFERENCE(object);
          object = NULL;
        }
      }
    }
    connection = connection->next;
  }
  return Core_Success;
}

Core_Result Core_Signals_disconnectAll(Core_Object* instance) {
  // Remove all connections from the specified instance.
  if (Core_InlineHashMapPP_remove(&g_signal_connection_manager.connections, instance)) {
    if (Core_Error_NotFound == Core_getError()) {
      Core_setError(Core_Error_NoError);
      return Core_Success;
    } else {
      return Core_Failure;
    }
  }
  return Core_Success;
}
