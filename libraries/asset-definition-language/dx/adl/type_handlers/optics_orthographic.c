#include "dx/adl/type_handlers/optics_orthographic.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static dx_result _on_hash_expected_key_key(dx_size* RETURN, void** a);

static dx_result _on_compare_expected_key_keys(dx_bool* RETURN, void** a, void** b);

static dx_result _initialize_expected_keys(dx_adl_type_handlers_optics_orthographic* SELF);

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_optics_orthographic* SELF);

static dx_result _check_keys(dx_adl_type_handlers_optics_orthographic* SELF, dx_ddl_node* node);

static dx_result _parse_optics_orthographic(dx_asset_optics_orthographic** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse(dx_object**, dx_adl_type_handlers_optics_orthographic*, dx_ddl_node* node, dx_adl_context* context);

static dx_result _resolve(dx_adl_type_handlers_optics_orthographic* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.optics",
                      dx_adl_type_handlers_optics_orthographic,
                      dx_adl_type_handler);

static void _on_expected_key_key_added(void** a) {
  DX_REFERENCE(*a);
}

static void _on_expected_key_key_removed(void** a) {
  DX_UNREFERENCE(*a);
}

static dx_result _on_hash_expected_key_key(dx_size* RETURN, void** a) {
  *RETURN = dx_string_get_hash_value(DX_STRING(*a));
  return DX_SUCCESS;
}

static dx_result _on_compare_expected_key_keys(dx_bool* RETURN, void** a, void** b) {
  *RETURN = dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
  return DX_SUCCESS;
}

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_optics_orthographic* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return DX_SUCCESS;
}

static dx_result _initialize_expected_keys(dx_adl_type_handlers_optics_orthographic* SELF) {
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION cfg = {
    .key_added_callback = &_on_expected_key_key_added,
    .key_removed_callback = &_on_expected_key_key_removed,
    .value_added_callback = NULL,
    .value_removed_callback = NULL,
    .hash_key_callback = &_on_hash_expected_key_key,
    .compare_keys_callback = &_on_compare_expected_key_keys,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->expected_keys, &cfg)) {
    return DX_FAILURE;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    dx_string* expected_key = NULL; \
    if (dx_string_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return DX_FAILURE; \
    } \
    if (dx_inline_pointer_hashmap_set(&SELF->expected_keys, expected_key, expected_key)) {\
      DX_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return DX_FAILURE; \
    } \
    DX_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("near");
  DEFINE("far");
#undef DEFINE
  return DX_SUCCESS;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static dx_result _check_keys(dx_adl_type_handlers_optics_orthographic* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  dx_inline_pointer_array received_keys;
  if (dx_inline_pointer_array_initialize(&received_keys, 0, &configuration)) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_get_keys(&node->map, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return DX_FAILURE;
  }
  dx_size number_of_received_keys = 0;
  if (dx_inline_pointer_array_get_size(&number_of_received_keys, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return DX_FAILURE;
  }
  for (dx_size i = 0, n = number_of_received_keys; i < n; ++i) {
    dx_string* received_key = NULL;
    if (dx_inline_pointer_array_get_at(&received_key, &received_keys, i)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return DX_FAILURE;
    }
    dx_string* expected_key = NULL;
    if (dx_inline_pointer_hashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return DX_FAILURE;
    }
  }
  dx_inline_pointer_array_uninitialize(&received_keys);
  return DX_SUCCESS;
}

static dx_result _parse_optics_orthographic(dx_asset_optics_orthographic** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  // Paranoia mode.
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(optics_orthographic_type))) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;

  dx_asset_optics_orthographic* optics_asset = NULL;
  if (dx_asset_optics_orthographic_create(&optics_asset)) {
    return DX_FAILURE;
  }
  // near?
  {
    dx_f32 value;
    dx_string* key = NAME(near_key);
    if (!dx_adl_semantical_read_f32(&value, node, key)) {
      optics_asset->near = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return DX_FAILURE;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // far?
  {
    dx_f32 value;
    dx_string* key = NAME(far_key);
    if (!dx_adl_semantical_read_f32(&value, node, key)) {
      optics_asset->far = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return DX_FAILURE;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  *RETURN = optics_asset;
  return DX_SUCCESS;
}

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_optics_orthographic* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (_check_keys(SELF, node)) {
    return DX_FAILURE;
  }
  return _parse_optics_orthographic((dx_asset_optics_orthographic**)RETURN, node,context);
}

static dx_result _resolve(dx_adl_type_handlers_optics_orthographic* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_optics_orthographic_construct(dx_adl_type_handlers_optics_orthographic* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_optics_orthographic_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  if (_initialize_expected_keys(SELF)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_optics_orthographic_destruct(dx_adl_type_handlers_optics_orthographic* SELF) {
  _initialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_optics_orthographic_dispatch_construct(dx_adl_type_handlers_optics_orthographic_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

dx_result dx_adl_type_handlers_optics_orthographic_create(dx_adl_type_handlers_optics_orthographic** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_optics_orthographic)
  if (dx_adl_type_handlers_optics_orthographic_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
