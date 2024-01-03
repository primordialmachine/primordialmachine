#include "dx/adl/type_handlers/mesh_operations.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, void** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, void** a, void** b);

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_mesh_operations* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_mesh_operations* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_mesh_operations* SELF, dx_ddl_node* node);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_mesh_operations* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_mesh_operations* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh_operations",
                      dx_adl_type_handlers_mesh_operations,
                      dx_adl_type_handler);

static void _on_expected_key_key_added(void** a) {
  DX_REFERENCE(*a);
}

static void _on_expected_key_key_removed(void** a) {
  DX_UNREFERENCE(*a);
}

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_mesh_operations* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_mesh_operations* SELF) {
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION cfg = {
    .key_added_callback = &_on_expected_key_key_added,
    .key_removed_callback = &_on_expected_key_key_removed,
    .value_added_callback = NULL,
    .value_removed_callback = NULL,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&_on_hash_expected_key_key,
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&_on_compare_expected_key_keys,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->expected_keys, &cfg)) {
    return Core_Failure;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    Core_String* expected_key = NULL; \
    if (Core_String_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    if (dx_inline_pointer_hashmap_set(&SELF->expected_keys, expected_key, expected_key)) {\
      DX_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    DX_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_mesh_operations* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  dx_inline_pointer_array received_keys;
  if (dx_inline_pointer_array_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_get_keys(&node->map, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return Core_Failure;
  }
  Core_Size number_of_received_keys = 0;
  if (dx_inline_pointer_array_get_size(&number_of_received_keys, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = number_of_received_keys; i < n; ++i) {
    Core_String* received_key = NULL;
    if (dx_inline_pointer_array_get_at(&received_key, &received_keys, i)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return Core_Failure;
    }
    Core_String* expected_key = NULL;
    if (dx_inline_pointer_hashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  dx_inline_pointer_array_uninitialize(&received_keys);
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_mesh_operations* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(mesh_operations_set_vertex_colors_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (isEqualTo) {
    dx_assets_mesh_operations_set_vertex_colors* asset = NULL;
    if (dx_assets_mesh_operations_set_vertex_colors_create(&asset)) {
      return Core_Failure;
    }
    *RETURN = CORE_OBJECT(asset);
    return Core_Success;
  } else {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
}

static Core_Result _resolve(dx_adl_type_handlers_mesh_operations* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_mesh_operations_construct(dx_adl_type_handlers_mesh_operations* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_mesh_operations);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_mesh_operations_destruct(dx_adl_type_handlers_mesh_operations* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_mesh_operations_constructDispatch(dx_adl_type_handlers_mesh_operations_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_mesh_operations_create(dx_adl_type_handlers_mesh_operations** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_mesh_operations);
  if (dx_adl_type_handlers_mesh_operations_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
