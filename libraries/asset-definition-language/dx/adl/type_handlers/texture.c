#include "dx/adl/type_handlers/texture.h"

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

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_texture* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_texture* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_texture* SELF, dx_ddl_node* node);

static Core_Result _parse_texture(dx_assets_texture** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN,
                        dx_adl_type_handlers_texture* SELF,
                        dx_ddl_node* node,
                        dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_texture* SELF,
                          dx_adl_symbol* symbol,
                          dx_adl_context* context);

Core_defineObjectType("dx.adl.type_handlers.texture",
                      dx_adl_type_handlers_texture,
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

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_texture* SELF) {
  Core_InlineHashMapPP_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_texture* SELF) {
  Core_InlineHashMapPP_Configuration cfg = {
    .keyAddedCallback = &_on_expected_key_key_added,
    .keyRemovedCallback = &_on_expected_key_key_removed,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&_on_hash_expected_key_key,
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&_on_compare_expected_key_keys,
  };
  if (Core_InlineHashMapPP_initialize(&SELF->expected_keys, &cfg)) {
    return Core_Failure;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    Core_String* expected_key = NULL; \
    if (Core_String_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      Core_InlineHashMapPP_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    if (Core_InlineHashMapPP_set(&SELF->expected_keys, expected_key, expected_key)) {\
      CORE_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      Core_InlineHashMapPP_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    CORE_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("name");
  DEFINE("image");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  CORE_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  CORE_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_texture* SELF, dx_ddl_node* node) {
  Core_InlineArrayListP_Configuration configuration = {
    .addedCallback = &on_received_key_added,
    .removedCallback = &on_received_key_removed,
  };
  Core_InlineArrayListP received_keys;
  if (Core_InlineArrayListP_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (Core_InlineHashMapPP_getKeys(&node->map, &received_keys)) {
    Core_InlineArrayListP_uninitialize(&received_keys);
    return Core_Failure;
  }
  Core_Size number_of_received_keys = 0;
  if (Core_InlineArrayListP_getSize(&number_of_received_keys, &received_keys)) {
    Core_InlineArrayListP_uninitialize(&received_keys);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = number_of_received_keys; i < n; ++i) {
    Core_String* received_key = NULL;
    if (Core_InlineArrayListP_get(&received_key, &received_keys, i)) {
      Core_InlineArrayListP_uninitialize(&received_keys);
      return Core_Failure;
    }
    Core_String* expected_key = NULL;
    if (Core_InlineHashMapPP_get(&expected_key, &SELF->expected_keys, received_key)) {
      Core_InlineArrayListP_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  Core_InlineArrayListP_uninitialize(&received_keys);
  return Core_Success;
}

static Core_Result _parse_texture(dx_assets_texture** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  // name
  Core_String* name_value = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
    return Core_Failure;
  }
  // image
  Core_Assets_Ref* image_reference_value = NULL;
  if (dx_asset_definition_language_parser_parse_image_instance_field(&image_reference_value, node, false, NAME(image_key), context)) {
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
    return Core_Failure;
  }
  //
  dx_assets_texture* texture_value = NULL;
  if (dx_assets_texture_create(&texture_value, name_value, image_reference_value)) {
    CORE_UNREFERENCE(image_reference_value);
    image_reference_value = NULL;
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(image_reference_value);
  image_reference_value = NULL;
  CORE_UNREFERENCE(name_value);
  name_value = NULL;
  //
  *RETURN = texture_value;
  //
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_texture* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  return _parse_texture((dx_assets_texture**)RETURN, node, context);
}

static Core_Result _resolve(dx_adl_type_handlers_texture* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  dx_assets_texture* texture = DX_ASSETS_TEXTURE(symbol->asset);
  if (texture->image_reference->object) {
    symbol->resolved = true;
    return Core_Success;
  }
  dx_adl_symbol* referenced_symbol = NULL;
  if (dx_asset_definitions_get(&referenced_symbol, context->definitions, texture->image_reference->name)) {
    return Core_Failure;
  }
  if (!referenced_symbol->asset) {
    CORE_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
    return Core_Failure;
  }
  texture->image_reference->object = referenced_symbol->asset;
  CORE_REFERENCE(texture->image_reference->object);
  CORE_UNREFERENCE(referenced_symbol);
  referenced_symbol = NULL;
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_texture_construct(dx_adl_type_handlers_texture* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_texture);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_texture_destruct(dx_adl_type_handlers_texture* SELF) {
  _initialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_texture_constructDispatch(dx_adl_type_handlers_texture_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_texture_create(dx_adl_type_handlers_texture** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_texture);
  if (dx_adl_type_handlers_texture_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
