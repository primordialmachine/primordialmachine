#include "dx/adl/symbols.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.symbol",
                      dx_adl_symbol,
                      dx_object);

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

static void dx_adl_symbol_dispatch_construct(dx_adl_symbol_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_adl_symbol_construct(dx_adl_symbol* SELF, dx_string* type, dx_string* name) {
  dx_rti_type* TYPE = dx_adl_symbol_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->type = type;
  DX_REFERENCE(SELF->type);
  SELF->name = name;
  DX_REFERENCE(SELF->name);
  SELF->asset = NULL;
  SELF->node = NULL;
  SELF->resolved = false;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_adl_symbol_create(dx_adl_symbol** RETURN, dx_string* type, dx_string* name) {
  DX_CREATE_PREFIX(dx_adl_symbol)
  if (dx_adl_symbol_construct(SELF, type, name)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.definitions",
                      dx_asset_definitions,
                      dx_object);

static void on_added(dx_object** a);

static void on_removed(dx_object** a);

static dx_result on_hash_key(dx_size* RETURN, dx_object** a);

static dx_result on_compare_keys(dx_bool* RETURN, dx_object** a, dx_object** b);

static void on_added(dx_object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  DX_REFERENCE(*a);
}

static void on_removed(dx_object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  DX_UNREFERENCE(*a);
}

static dx_result on_hash_key(dx_size* RETURN, dx_object** a) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  *RETURN = dx_string_get_hash_value(DX_STRING(*a));
  return DX_SUCCESS;
}

static dx_result on_compare_keys(dx_bool* RETURN, dx_object** a, dx_object** b) {
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*a);
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(*b);
  *RETURN = dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
  return DX_SUCCESS;
}

static void dx_asset_definitions_destruct(dx_asset_definitions* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->map);
}

static void dx_asset_definitions_dispatch_construct(dx_asset_definitions_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_definitions_construct(dx_asset_definitions* SELF) {
  dx_rti_type* TYPE = dx_asset_definitions_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (dx_result(*)(dx_bool*, void**,void**)) & on_compare_keys,
    .hash_key_callback = (dx_result(*)(dx_size*,void**)) & on_hash_key,
    .key_added_callback = (void(*)(void**)) & on_added,
    .key_removed_callback = (void(*)(void**)) & on_removed,
    .value_added_callback = (void(*)(void**)) & on_added,
    .value_removed_callback = (void(*)(void**)) & on_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->map, &configuration)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_definitions_create(dx_asset_definitions** RETURN) {
  DX_CREATE_PREFIX(dx_asset_definitions)
  if (dx_asset_definitions_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_asset_definitions_get(dx_adl_symbol** RETURN, dx_asset_definitions const* SELF, dx_string* name) {
  if (!RETURN || !SELF || !name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_adl_symbol* temporary = NULL;
  if (dx_inline_pointer_hashmap_get(&temporary, &SELF->map, name)) {
    return DX_FAILURE;
  }
  DX_REFERENCE(temporary);
  *RETURN = temporary;
  return DX_SUCCESS;
}


dx_result dx_asset_definitions_set(dx_asset_definitions* SELF, dx_string* name, dx_adl_symbol* value) {
  if (!SELF || !name || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->map, name, value)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_asset_definitions_dump(dx_asset_definitions* SELF) {
  dx_log("{\n", sizeof("{\n") - 1);
  dx_inline_pointer_hashmap_iterator iterator;
  dx_inline_pointer_hashmap_iterator_initialize(&iterator, &SELF->map);
  dx_bool has_entry = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
    return DX_FAILURE;
  }
  while (has_entry) {
    dx_string* key = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_key(&key, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return DX_FAILURE;
    }
    dx_adl_symbol* value = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&value, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return DX_FAILURE;
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
      return DX_FAILURE;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  dx_log("}\n", sizeof("}\n") - 1);
  return DX_SUCCESS;
}
