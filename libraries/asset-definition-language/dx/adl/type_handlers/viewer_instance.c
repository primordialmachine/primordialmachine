#include "dx/adl/type_handlers/viewer_instance.h"

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

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_viewer_instance* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_viewer_instance* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node);

static Core_Result _parse_viewer_instance(dx_assets_viewer_instance** RETURN, dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_viewer_instance* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

Core_defineObjectType("dx.adl.type_handlers.viewer_instance",
                      dx_adl_type_handlers_viewer_instance,
                      dx_adl_type_handler);

static void _on_expected_key_key_added(void** a) {
  CORE_REFERENCE(*a);
}

static void _on_expected_key_key_removed(void** a) {
  CORE_UNREFERENCE(*a);
}

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_viewer_instance* SELF) {
  Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_viewer_instance* SELF) {
  Core_InlinePointerHashMap_Configuration cfg = {
    .keyAddedCallback = &_on_expected_key_key_added,
    .keyRemovedCallback = &_on_expected_key_key_removed,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .hashKeyCallback = (Core_InlinePointerHashmap_hash_key_callback*)&_on_hash_expected_key_key,
    .compareKeysCallback = (Core_InlinePointerHashmap_compare_keys_callback*)&_on_compare_expected_key_keys,
  };
  if (Core_InlinePointerHashmap_initialize(&SELF->expected_keys, &cfg)) {
    return Core_Failure;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    Core_String* expected_key = NULL; \
    if (Core_String_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    if (Core_InlinePointerHashmap_set(&SELF->expected_keys, expected_key, expected_key)) {\
      CORE_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    CORE_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("name");
  DEFINE("reference");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  CORE_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  CORE_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  dx_inline_pointer_array received_keys;
  if (dx_inline_pointer_array_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (Core_InlinePointerHashmap_getKeys(&node->map, &received_keys)) {
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
    if (Core_InlinePointerHashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  dx_inline_pointer_array_uninitialize(&received_keys);
  return Core_Success;
}

static Core_Result _parse_viewer_instance(dx_assets_viewer_instance** RETURN, dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_reference* viewer_reference = NULL;
  // reference
  {
    Core_String* name = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
    if (!name) {
      return Core_Failure;
    }
    if (dx_asset_reference_create(&viewer_reference, name)) {
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
  }
  dx_assets_viewer_instance* viewer_instance_value = NULL;
  if (dx_assets_viewer_instance_create(&viewer_instance_value, viewer_reference)) {
    CORE_UNREFERENCE(viewer_reference);
    viewer_reference = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(viewer_reference);
  viewer_reference = NULL;
  *RETURN = viewer_instance_value;
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  return _parse_viewer_instance((dx_assets_viewer_instance**)RETURN, SELF, node, context);
}

static Core_Result _resolve(dx_adl_type_handlers_viewer_instance* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  dx_assets_viewer_instance* viewer_instance = DX_ASSETS_VIEWER_INSTANCE(symbol->asset);
  if (viewer_instance->viewer_reference) {
    dx_adl_symbol* referenced_symbol = NULL;
    if (dx_asset_definitions_get(&referenced_symbol, context->definitions, viewer_instance->viewer_reference->name)) {
      return Core_Failure;
    }
    viewer_instance->viewer_reference->object = referenced_symbol->asset;
    CORE_REFERENCE(viewer_instance->viewer_reference->object);
    CORE_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
  }
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_viewer_instance_construct(dx_adl_type_handlers_viewer_instance* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_viewer_instance);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_viewer_instance_destruct(dx_adl_type_handlers_viewer_instance* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_viewer_instance_constructDispatch(dx_adl_type_handlers_viewer_instance_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_viewer_instance_create(dx_adl_type_handlers_viewer_instance** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_viewer_instance);
  if (dx_adl_type_handlers_viewer_instance_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
