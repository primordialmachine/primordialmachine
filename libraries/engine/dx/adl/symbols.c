#include "dx/adl/symbols.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.symbol",
                      dx_adl_symbol,
                      dx_object);

static void dx_adl_symbol_destruct(dx_adl_symbol* self) {
  if (self->node) {
    DX_UNREFERENCE(self->node);
    self->node = NULL;
  }
  if (self->asset) {
    DX_UNREFERENCE(self->asset);
    self->asset = NULL;
  }
  DX_UNREFERENCE(self->type);
  self->type = NULL;
  DX_UNREFERENCE(self->name);
  self->name = NULL;
}

static void dx_adl_symbol_dispatch_construct(dx_adl_symbol_dispatch* self)
{/*Intentionally empty.*/}

int dx_adl_symbol_construct(dx_adl_symbol* self, dx_string* type, dx_string* name) {
  dx_rti_type* _type = dx_adl_symbol_get_type();
  if (!_type) {
    return 1;
  }
  self->type = type;
  DX_REFERENCE(self->type);
  self->name = name;
  DX_REFERENCE(self->name);
  self->asset = NULL;
  self->node = NULL;
  self->resolved = false;
  return 0;
}

dx_adl_symbol* dx_adl_symbol_create(dx_string* type, dx_string* name) {
  dx_rti_type* _type = dx_adl_symbol_get_type();
  if (!_type) {
    return NULL;
  }
  dx_adl_symbol* self = DX_ADL_SYMBOL(dx_object_alloc(sizeof(dx_adl_symbol)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_symbol_construct(self, type, name)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
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

static void dx_asset_definitions_destruct(dx_asset_definitions* self) {
  dx_inline_pointer_hashmap_uninitialize(&self->map);
}

static void dx_asset_definitions_dispatch_construct(dx_asset_definitions_dispatch* self)
{/*Intentionally empty.*/}

int dx_asset_definitions_construct(dx_asset_definitions* self) {
  dx_rti_type* _type = dx_asset_definitions_get_type();
  if (!_type) {
    return 1;
  }
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (dx_result(*)(dx_bool*, void**,void**)) & on_compare_keys,
    .hash_key_callback = (dx_result(*)(dx_size*,void**)) & on_hash_key,
    .key_added_callback = (void(*)(void**)) & on_added,
    .key_removed_callback = (void(*)(void**)) & on_removed,
    .value_added_callback = (void(*)(void**)) & on_added,
    .value_removed_callback = (void(*)(void**)) & on_removed,
  };
  if (dx_inline_pointer_hashmap_initialize(&self->map, &configuration)) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_definitions* dx_asset_definitions_create() {
  dx_asset_definitions* self = DX_ASSET_PALETTE(dx_object_alloc(sizeof(dx_asset_definitions)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_definitions_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_adl_symbol* dx_asset_definitions_get(dx_asset_definitions const* self, dx_string* name) {
  if (!self || !name) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  dx_adl_symbol* temporary = NULL;
  if (dx_inline_pointer_hashmap_get(&temporary, &self->map, name)) {
    return NULL;
  }
  return temporary;
}

int dx_asset_definitions_set(dx_asset_definitions* self, dx_string* name, dx_adl_symbol* value) {
  if (!self || !name || !value) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_inline_pointer_hashmap_set(&self->map, name, value)) {
    return 1;
  }
  return 0;
}

int dx_asset_definitions_dump(dx_asset_definitions* self) {
  dx_log("{\n", sizeof("{\n") - 1);
  dx_inline_pointer_hashmap_iterator iterator;
  dx_inline_pointer_hashmap_iterator_initialize(&iterator, &self->map);
  dx_bool has_entry = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
    return 1;
  }
  while (has_entry) {
    dx_string* key = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_key(&key, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return 1;
    }
    dx_adl_symbol* value = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&value, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return 1;
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
      return 1;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  dx_log("}\n", sizeof("}\n") - 1);
  return 0;
}