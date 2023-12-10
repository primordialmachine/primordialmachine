#include "dx/adl/symbols.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.symbol",
                      dx_adl_symbol,
                      Core_Object);

static void dx_adl_symbol_destruct(dx_adl_symbol* SELF) {
  if (SELF->node) {
    DX_UNREFERENCE(SELF->node);
    SELF->node = NULL;
  }
  if (SELF->asset) {
    DX_UNREFERENCE(SELF->asset);
    SELF->asset = NULL;
  }
  DX_UNREFERENCE(SELF->type);
  SELF->type = NULL;
  DX_UNREFERENCE(SELF->name);
  SELF->name = NULL;
}

static void dx_adl_symbol_constructDispatch(dx_adl_symbol_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_adl_symbol_construct(dx_adl_symbol* SELF, Core_String* type, Core_String* name) {
  DX_CONSTRUCT_PREFIX(dx_adl_symbol);
  SELF->type = type;
  DX_REFERENCE(SELF->type);
  SELF->name = name;
  DX_REFERENCE(SELF->name);
  SELF->asset = NULL;
  SELF->node = NULL;
  SELF->resolved = false;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_adl_symbol_create(dx_adl_symbol** RETURN, Core_String* type, Core_String* name) {
  DX_CREATE_PREFIX(dx_adl_symbol);
  if (dx_adl_symbol_construct(SELF, type, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.definitions",
                      dx_asset_definitions,
                      Core_Object);

static void on_added(Core_Object** a);

static void on_removed(Core_Object** a);

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a);

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static void on_added(Core_Object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  DX_REFERENCE(*a);
}

static void on_removed(Core_Object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  DX_UNREFERENCE(*a);
}

static Core_Result on_hash_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result on_compare_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static void dx_asset_definitions_destruct(dx_asset_definitions* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->map);
}

static void dx_asset_definitions_constructDispatch(dx_asset_definitions_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_definitions_construct(dx_asset_definitions* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_definitions);
  static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&on_compare_keys,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&on_hash_key,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&on_added,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&on_removed,
    .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&on_added,
    .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&on_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->map, &configuration)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_definitions_create(dx_asset_definitions** RETURN) {
  DX_CREATE_PREFIX(dx_asset_definitions);
  if (dx_asset_definitions_construct(SELF)) {
    DX_UNREFERENCE(SELF);
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
  if (dx_inline_pointer_hashmap_get(&temporary, &SELF->map, name)) {
    return Core_Failure;
  }
  DX_REFERENCE(temporary);
  *RETURN = temporary;
  return Core_Success;
}


Core_Result dx_asset_definitions_set(dx_asset_definitions* SELF, Core_String* name, dx_adl_symbol* value) {
  if (!SELF || !name || !value) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->map, name, value)) {
    return Core_Failure;
  }
  return Core_Success;
}

Core_Result dx_asset_definitions_dump(dx_asset_definitions* SELF) {
  dx_log("{\n", sizeof("{\n") - 1);
  dx_inline_pointer_hashmap_iterator iterator;
  dx_inline_pointer_hashmap_iterator_initialize(&iterator, &SELF->map);
  Core_Boolean has_entry = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
    return Core_Failure;
  }
  while (has_entry) {
    Core_String* key = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_key(&key, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return Core_Failure;
    }
    dx_adl_symbol* value = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&value, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return Core_Failure;
    }
    dx_log("  ", sizeof("  ") - 1);
    dx_log(key->bytes, key->number_of_bytes);
    dx_log(" -> ", sizeof(" -> ") - 1);
    dx_log(value->name->bytes, value->name->number_of_bytes);
    dx_log(" / ", sizeof(" / ") - 1);
    dx_log(value->type->bytes, value->type->number_of_bytes);
    dx_log("\n", sizeof("\n") - 1);
    dx_inline_pointer_hashmap_iterator_next(&iterator);
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return Core_Failure;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  dx_log("}\n", sizeof("}\n") - 1);
  return Core_Success;
}
