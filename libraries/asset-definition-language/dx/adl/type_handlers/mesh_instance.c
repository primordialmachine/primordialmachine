#include "dx/adl/type_handlers/mesh_instance.h"

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

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_mesh_instance* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_mesh_instance* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_mesh_instance* SELF, dx_ddl_node* node);

static Core_Result _parse(Core_Object** RETURN,
                        dx_adl_type_handlers_mesh_instance* SELF,
                        dx_ddl_node* node,
                        dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_mesh_instance* SELF,
                          dx_adl_symbol* symbol,
                          dx_adl_context* context);

Core_defineObjectType("dx.adl.type_handlers.mesh_instance",
                      dx_adl_type_handlers_mesh_instance,
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

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_mesh_instance* SELF) {
  Core_InlineHashMapPP_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_mesh_instance* SELF) {
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
  DEFINE("transformation");
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

static Core_Result _check_keys(dx_adl_type_handlers_mesh_instance* SELF, dx_ddl_node* node) {
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

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_mesh_instance* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  dx_asset_mesh_instance* mesh_instance = NULL;
  Core_Assets_Ref* mesh_reference = NULL;
  Core_Assets_Matrix4x4R32* transformation = NULL;
  // transformation?
  {
    Core_String* name = NULL;
    if (Core_String_create(&name, "transformation", sizeof("transformation") - 1)) {
      return Core_Failure;
    }
    Core_Error old_error = Core_getError();
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, name)) {
      if (Core_Error_NotFound != Core_getError()) {
        Core_setError(old_error);
      } else {
        CORE_UNREFERENCE(name);
        name = NULL;
        return Core_Failure;
      }
    } else {
      CORE_UNREFERENCE(name);
      name = NULL;
      if (dx_asset_definition_language_parser_parse_translation(&transformation, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  // reference
  {
    Core_String* name = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
    if (!name) {
      if (transformation) {
        CORE_UNREFERENCE(transformation);
        transformation = NULL;
      }
      return Core_Failure;
    }
    if (Core_Assets_Ref_create(&mesh_reference, name)) {
      if (transformation) {
        CORE_UNREFERENCE(transformation);
        transformation = NULL;
      }
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
  }
  mesh_instance = NULL;
  if (dx_asset_mesh_instance_create(&mesh_instance, mesh_reference)) {
    CORE_UNREFERENCE(mesh_reference);
    mesh_reference = NULL;
    if (transformation) {
      CORE_UNREFERENCE(transformation);
      transformation = NULL;
    }
    return Core_Failure;
  }
  CORE_UNREFERENCE(mesh_reference);
  mesh_reference = NULL;
  if (transformation) {
    mesh_instance->world_matrix->value = transformation->value;
    CORE_UNREFERENCE(transformation);
    transformation = NULL;
  }
  *RETURN = CORE_OBJECT(mesh_instance);
  return Core_Success;
}

static Core_Result _resolve(dx_adl_type_handlers_mesh_instance* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  dx_asset_mesh_instance* mesh_instance = DX_ASSET_MESH_INSTANCE(symbol->asset);
  if (mesh_instance->mesh_reference) {
    dx_adl_symbol* referenced_symbol = NULL;
    if (dx_asset_definitions_get(&referenced_symbol, context->definitions, mesh_instance->mesh_reference->name)) {
      return Core_Failure;
    }
    mesh_instance->mesh_reference->object = referenced_symbol->asset;
    CORE_REFERENCE(mesh_instance->mesh_reference->object);
    CORE_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
  }
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_mesh_instance_construct(dx_adl_type_handlers_mesh_instance* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_mesh_instance);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_mesh_instance_destruct(dx_adl_type_handlers_mesh_instance* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_mesh_instance_constructDispatch(dx_adl_type_handlers_mesh_instance_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_mesh_instance_create(dx_adl_type_handlers_mesh_instance** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_mesh_instance);
  if (dx_adl_type_handlers_mesh_instance_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
