#include "Core/Configuration.h"

#include "Core/Convert.h"

Core_defineObjectType("Core.Configuration",
                      Core_Configuration,
                      Core_Object);

static void Core_Configuration_destruct(Core_Configuration* SELF) {
  Core_InlineHashMapPP_uninitialize(&SELF->backend);
}

static void Core_Configuration_constructDispatch(Core_Configuration_Dispatch* SELF)
{/*Intentionally empty.*/}

static void keyAdded(Core_String** x) {
  CORE_REFERENCE(*x);
}

static void keyRemoved(Core_String** x) {
  CORE_UNREFERENCE(*x);
}

static void valueAdded(Core_String** x) {
  CORE_REFERENCE(*x);
}

static void valueRemoved(Core_String** x) {
  CORE_UNREFERENCE(*x);
}

static Core_Result compareKeys(Core_Boolean* RETURN, Core_String** x, Core_String** y) {
  return Core_String_isEqualTo(RETURN, *x, *y);
}

static Core_Result hashKey(Core_Size* RETURN, Core_String** x) {
  return Core_String_getHashValue(RETURN, *x);
}

Core_Result Core_Configuration_construct(Core_Configuration* SELF) {
  Core_BeginConstructor(Core_Configuration);
  Core_InlineHashMapPP_Configuration configuration = {
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&compareKeys,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&hashKey,
    .keyAddedCallback = (Core_InlineHashMapPP_KeyAddedCallback*)&keyAdded,
    .keyRemovedCallback = (Core_InlineHashMapPP_KeyRemovedCallback*)&keyRemoved,
    .valueAddedCallback = (Core_InlineHashMapPP_ValueAddedCallback*)&valueAdded,
    .valueRemovedCallback = (Core_InlineHashMapPP_ValueRemovedCallback*)&valueRemoved,
  };
  if (Core_InlineHashMapPP_initialize(&SELF->backend, &configuration)) {
    return Core_Failure;
  }
  Core_EndConstructor(Core_Configuration);
}

Core_Result Core_Configuration_create(Core_Configuration** RETURN) {
  DX_CREATE_PREFIX(Core_Configuration);
  if (Core_Configuration_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Configuration_setN8(Core_Configuration* SELF, Core_String* key, Core_Natural8 value) {
  Core_String* f;
  if (Core_String_create(&f, "${n8}", sizeof("${n8}") - 1)) {
    return Core_Failure;
  }
  Core_String* t;
  if (Core_String_printf(&t, f, value)) {
    CORE_UNREFERENCE(f);
    f = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(f);
  f = NULL;
  if (Core_Configuration_setS(SELF, key, t)) {
    CORE_UNREFERENCE(t);
    t = NULL;
  }
  CORE_UNREFERENCE(t);
  t = NULL;
  return Core_Success;
}

Core_Result Core_Configuration_setS(Core_Configuration* SELF, Core_String* key, Core_String* value) {
  return Core_InlineHashMapPP_set(&SELF->backend, key, value);
}

Core_Result Core_Configuration_getN8(Core_Natural8* RETURN, Core_Configuration* SELF, Core_String* key) {
  Core_String* value1;
  if (Core_InlineHashMapPP_get(&value1, &SELF->backend, key)) {
    return Core_Failure;
  }
  Core_Natural8 value2;
  if (Core_convertStringToN8(&value2, value1->bytes, value1->numberOfBytes)) {
    return Core_Failure;
  }
  *RETURN = value2;
  return Core_Success;
}

Core_Result Core_Configuration_getS(Core_String** RETURN, Core_Configuration* SELF, Core_String* key) {
  return Core_InlineHashMapPP_get(RETURN, &SELF->backend, key);
}

Core_defineObjectType("Core.ConfigurationIterator",
                      Core_ConfigurationIterator,
                      Core_Object);

static void Core_ConfigurationIterator_destruct(Core_ConfigurationIterator* SELF) {
  Core_InlineHashMapPPIterator_uninitialize(&SELF->backend);
  CORE_UNREFERENCE(SELF->target);
  SELF->target = NULL;
}

static void Core_ConfigurationIterator_constructDispatch(Core_ConfigurationIterator_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result Core_ConfigurationIterator_construct(Core_ConfigurationIterator* SELF, Core_Configuration* target) {
  Core_BeginConstructor(Core_ConfigurationIterator);
  SELF->target = target;
  CORE_REFERENCE(target);
  if (Core_InlineHashMapPPIterator_initialize(&SELF->backend, &SELF->target->backend)) {
    CORE_UNREFERENCE(SELF->target);
    SELF->target = NULL;
    return Core_Failure;
  }
  Core_EndConstructor(Core_ConfigurationIterator);
}

Core_Result Core_ConfigurationIterator_create(Core_ConfigurationIterator** RETURN, Core_Configuration* target) {
  DX_CREATE_PREFIX(Core_ConfigurationIterator);
  if (Core_ConfigurationIterator_construct(SELF, target)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_ConfigurationIterator_hasEntry(Core_Boolean* RETURN, Core_ConfigurationIterator* SELF) {
  return Core_InlineHashMapPPIterator_hasEntry(RETURN, &SELF->backend);
}

Core_Result Core_ConfigurationIterator_getValue(Core_String** RETURN, Core_ConfigurationIterator* SELF) {
  return Core_InlineHashMapPPIterator_getValue(RETURN, &SELF->backend);
}

Core_Result Core_ConfigurationIterator_getKey(Core_String** RETURN, Core_ConfigurationIterator* SELF) {
  return Core_InlineHashMapPPIterator_getKey(RETURN, &SELF->backend);
}
