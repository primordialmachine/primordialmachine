#include "dx/core/signals.h"

#include "Core/Hash.h"
#include "dx/core/inline_pointer_array.h"
#include "Core/Memory.h"
#include "dx/core/inline_pointer_hashmap.h"
#include <string.h>

// key of a signal
typedef struct _SignalKey _SignalKey;

/// a connection to a signal.
typedef struct _SignalConnection _SignalConnection;

// a signal
typedef struct _Signal _Signal;

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

struct _SignalConnection {
  int x;
};

struct _Signal {
  dx_reference_counter referenceCount;
  _SignalKey key;
  /// null pointer or pointer to a (possibly empty) list of connections to this signal.
  dx_inline_pointer_array* connections;
};

static Core_Result _Signal_getConnections(dx_inline_pointer_array** connections, _Signal* signal) {
  return Core_Success;
}

#if 0
static Core_Result _Signal_create(_Signal** RETURN, Core_Type* type, char const* p, Core_Size n);
#endif

static Core_Result _Signal_destroy(_Signal* signal);

static void _Signal_valueAdded(_Signal** SELF);

static void _Signal_valueRemoved(_Signal** SELF);

static Core_Result _SignalKey_hashKey(Core_Size* RETURN, _SignalKey** SELF);

static Core_Result _SignalKey_compareKeys(Core_Boolean* RETURN, _SignalKey** SELF, _SignalKey** other);

static Core_Result _Signal_destroy(_Signal* signal) {
  if (signal->connections) {
    dx_inline_pointer_array_uninitialize(signal->connections);
    Core_Memory_deallocate(signal->connections);
    signal->connections = NULL;
  }
  Core_Memory_deallocate(signal->key.name.p);
  signal->key.name.p = NULL;
  Core_Memory_deallocate(signal);
  return Core_Success;
}

#if 0
static Core_Result _Signal_create(_Signal** RETURN, Core_Type* type, _SignalKey const* key) {
  char* q = NULL;
  if (dx_memory_allocate(&q, n)) {
    return Core_Failure;
  }
  dx_memory_copy(q, p, n); // all arguments valid => bound to succeed.
  Core_Size hashValue = dx_hash_bytes(q, n);
  _Signal* SELF = NULL;
  if (dx_memory_allocate(&SELF, sizeof(_Signal))) {
    Core_Memory_deallocate(q);
    q = NULL;
    return Core_Failure;
  }
  SELF->key.name.p = q;
  SELF->key.name.n = n;
  SELF->key.hashValue = hashValue;
  SELF->key.type = type;
  SELF->connections = NULL;
  SELF->referenceCount = 1;
  *RETURN = SELF;
  return Core_Success;
}
#endif

static void _Signal_valueAdded(_Signal** SELF) {
  dx_reference_counter_increment(&(*SELF)->referenceCount);
}

static void _Signal_valueRemoved(_Signal** SELF) {
  if (!dx_reference_counter_decrement(&(*SELF)->referenceCount)) {
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

static Core_InlinePointerHashmap* g_signals = NULL;

Core_Result Core_Signals_initialize() {
  if (Core_Memory_allocate(&g_signals, sizeof(Core_InlinePointerHashmap))) {
    return Core_Failure;
  }
  Core_InlinePointerHashMap_Configuration configuration = {
    .compareKeysCallback = (Core_InlinePointerHashmap_compare_keys_callback*) & _SignalKey_compareKeys,
    .hashKeyCallback = (Core_InlinePointerHashmap_hash_key_callback*)&_SignalKey_hashKey,
    .keyAddedCallback = NULL,
    .keyRemovedCallback = NULL,
    .valueAddedCallback = (Core_InlinePointerHashMap_KeyAddedCallback*)&_Signal_valueAdded,
    .valueRemovedCallback = (Core_InlinePointerHashMap_KeyRemovedCallback*)&_Signal_valueRemoved,
  };
  if (Core_InlinePointerHashmap_initialize(g_signals, &configuration)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Signals_uninitialize() {
  Core_InlinePointerHashmap_uninitialize(g_signals);
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
      if (Core_InlinePointerHashmap_get(&signal, g_signals, &key)) {
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
    if (Core_InlinePointerHashmap_get(&signal, g_signals, &key)) {
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
  if (Core_Memory_allocate(&signal, sizeof(_Signal))) {
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
  signal->connections = NULL;
  signal->referenceCount = 0;
  if (Core_InlinePointerHashmap_set(g_signals, &signal->key, signal)) {
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
      if (Core_InlinePointerHashmap_get(&signal, g_signals, &key)) {
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
    if (Core_InlinePointerHashmap_get(&signal, g_signals, &key)) {
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

Core_Result Core_Signals_connect(Core_Object* instance, char const* p, Core_Size n, Core_Object* context, Core_Callback* callback) {
  _Signal* signal = NULL;
  Core_Type* type = instance->type;
  if (get(&signal, type, p, n)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Signals_invoke(Core_Object* instance, char const* p, Core_Size n, Core_Object* argument) {
  _Signal* signal = NULL;
  Core_Type* type = instance->type;
  if (get(&signal, type, p, n)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result Core_Signals_disconnectAll(Core_Object* instance) {
  return Core_Success;
}
