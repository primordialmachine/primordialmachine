#include "dx/adl/symbols.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.adl.symbol",
                      dx_adl_symbol,
                      Core_Object);

static void dx_adl_symbol_destruct(dx_adl_symbol* SELF) {
  if (SELF->node) {
    CORE_UNREFERENCE(SELF->node);
    SELF->node = NULL;
  }
  if (SELF->asset) {
    CORE_UNREFERENCE(SELF->asset);
    SELF->asset = NULL;
  }
  CORE_UNREFERENCE(SELF->type);
  SELF->type = NULL;
  CORE_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_adl_symbol_constructDispatch(dx_adl_symbol_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_adl_symbol_construct(dx_adl_symbol* SELF, Core_String* type, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_adl_symbol);
  SELF->type = type;
  CORE_REFERENCE(SELF->type);
  SELF->name = name;
  CORE_REFERENCE(SELF->name);
  SELF->asset = NULL;
  SELF->node = NULL;
  SELF->resolved = false;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_adl_symbol_create(dx_adl_symbol** RETURN, Core_String* type, Core_String* name) {
  DX_CREATE_PREFIX(dx_adl_symbol);
  if (dx_adl_symbol_construct(SELF, type, name)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.adl.definitions",
                      dx_asset_definitions,
                      Core_Object);

static void on_added(Core_Object** a);

static void on_removed(Core_Object** a);

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a);

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static void on_added(Core_Object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  CORE_REFERENCE(*a);
}

static void on_removed(Core_Object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  CORE_UNREFERENCE(*a);
}

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static void dx_asset_definitions_destruct(dx_asset_definitions* SELF) {
  Core_InlinePointerHashmap_uninitialize(&SELF->map);
}

static void dx_asset_definitions_constructDispatch(dx_asset_definitions_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_definitions_construct(dx_asset_definitions* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_definitions);
  static Core_InlinePointerHashMap_Configuration const configuration = {
    .compareKeysCallback = (Core_InlinePointerHashmap_CompareKeysCallback*)&on_compare_keys,
    .hashKeyCallback = (Core_InlinePointerHashmap_HashKeyCallback*)&on_hash_key,
    .keyAddedCallback = (Core_InlinePointerHashMap_KeyAddedCallback*)&on_added,
    .keyRemovedCallback = (Core_InlinePointerHashMap_KeyRemovedCallback*)&on_removed,
    .valueAddedCallback = (Core_InlinePointerHashmap_ValueAddedCallback*)&on_added,
    .valueRemovedCallback = (Core_InlinePointerHashMap_ValueRemovedCallback*)&on_removed,
  };
  if (Core_InlinePointerHashmap_initialize(&SELF->map, &configuration)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_definitions_create(dx_asset_definitions** RETURN) {
  DX_CREATE_PREFIX(dx_asset_definitions);
  if (dx_asset_definitions_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_asset_definitions_get(dx_adl_symbol** RETURN, dx_asset_definitions const* SELF, Core_String* name) {
  if (!RETURN || !SELF || !name) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  dx_adl_symbol* temporary = NULL;
  if (Core_InlinePointerHashmap_get(&temporary, &SELF->map, name)) {
    return Core_Failure;
  }
  CORE_REFERENCE(temporary);
  *RETURN = temporary;
  return Core_Success;
}


Core_Result dx_asset_definitions_set(dx_asset_definitions* SELF, Core_String* name, dx_adl_symbol* value) {
  if (!SELF || !name || !value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (Core_InlinePointerHashmap_set(&SELF->map, name, value)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_asset_definitions_dump(dx_asset_definitions* SELF) {
  dx_log("{\n", sizeof("{\n") - 1);
  Core_InlinePointerHashmapIterator iterator;
  Core_InlinePointerHashmapIterator_initialize(&iterator, &SELF->map);
  Core_Boolean has_entry = false;
  if (Core_InlinePointerHashmapIterator_hasEntry(&has_entry, &iterator)) {
    Core_InlinePointerHashmapIterator_uninitialize(&iterator);
    return Core_Failure;
  }
  while (has_entry) {
    Core_String* key = NULL;
    if (Core_InlinePointerHashmapIterator_getKey(&key, &iterator)) {
      Core_InlinePointerHashmapIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    dx_adl_symbol* value = NULL;
    if (Core_InlinePointerHashmapIterator_getValue(&value, &iterator)) {
      Core_InlinePointerHashmapIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    dx_log("  ", sizeof("  ") - 1);
    dx_log(key->bytes, key->number_of_bytes);
    dx_log(" -> ", sizeof(" -> ") - 1);
    dx_log(value->name->bytes, value->name->number_of_bytes);
    dx_log(" / ", sizeof(" / ") - 1);
    dx_log(value->type->bytes, value->type->number_of_bytes);
    dx_log("\n", sizeof("\n") - 1);
    Core_InlinePointerHashmapIterator_next(&iterator);
    if (Core_InlinePointerHashmapIterator_hasEntry(&has_entry, &iterator)) {
      Core_InlinePointerHashmapIterator_uninitialize(&iterator);
      return Core_Failure;
    }
  }
  Core_InlinePointerHashmapIterator_uninitialize(&iterator);
  dx_log("}\n", sizeof("}\n") - 1);
  return Core_Success;
}
