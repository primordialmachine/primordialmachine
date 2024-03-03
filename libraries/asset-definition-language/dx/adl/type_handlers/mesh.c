#include "dx/adl/type_handlers/mesh.h"

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

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_mesh* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_mesh* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node);

static Core_Result _parse_mesh_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static Core_Result _parse_mesh_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static Core_Result _parse_vertex_format(Core_VertexFormat* RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse_mesh(Core_Assets_Mesh** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_mesh* SELF, dx_adl_symbol *symbol, dx_adl_context* context);

static Core_Result _enter(dx_adl_type_handlers_mesh* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_defineObjectType("dx.adl.type_handlers.mesh",
                      dx_adl_type_handlers_mesh,
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

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_mesh* SELF) {
  Core_InlineHashMapPP_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_mesh* SELF) {
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
  DEFINE("vertexFormat");
  DEFINE("generator");
  DEFINE("operations");
  DEFINE("material");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  CORE_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  CORE_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node) {
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

static Core_Result _parse_mesh_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (node->kind != dx_ddl_node_kind_map) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }

  dx_adl_symbol* reader_symbol = NULL;
  if (dx_adl_symbol_create(&reader_symbol, received_type, dx_adl_names_create_unique_name(context->names))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    CORE_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return Core_Failure;
  }

  dx_adl_type_handler* type_handler = NULL;
  if (Core_InlineHashMapPP_get(&type_handler, &context->type_handlers, received_type)) {
    if (Core_Error_NotFound != Core_getError()) {
      CORE_UNREFERENCE(reader_symbol);
      reader_symbol = NULL;
      return Core_Failure;
    } else {
      CORE_UNREFERENCE(reader_symbol);
      reader_symbol = NULL;
      Core_setError(Core_Error_SemanticalAnalysisFailed);
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  dx_asset_mesh_operation* operation = NULL;
  if (dx_adl_type_handler_read((Core_Object**)&operation, type_handler, node, context)) {
    CORE_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return Core_Failure;
  }
  reader_symbol->asset = CORE_OBJECT(operation);
  CORE_REFERENCE(reader_symbol->asset);
  CORE_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  Core_Assets_Mesh* mesh = CORE_ASSETS_MESH(symbol->asset);
  if (dx_inline_object_array_append(&mesh->operations, CORE_OBJECT(operation))) {
    CORE_UNREFERENCE(operation);
    operation = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(operation);
  operation = NULL;
  return Core_Success;
}

static Core_Result _parse_mesh_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_Size n;
  if (dx_ddl_node_list_get_size(&n, node)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_list_get(&child_node, node, i)) {
      return Core_Failure;
    }
    if (_parse_mesh_operation(child_node, symbol, context)) {
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
  }
  return Core_Success;
}

static Core_Result _parse_vertex_format(Core_VertexFormat* RETURN, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_VertexFormat vertex_format = 0;
  Core_Size n;
  if (dx_ddl_node_list_get_size(&n, node)) {
    return Core_Failure;
  }
  if (n > 3) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_StringBuffer * stringBuffer;
  Core_StringBuffer_create(&stringBuffer);
  typedef struct SupportedEntry {
    char const* name;
    size_t const numberOfBytes;
    Core_VertexFormat value;
  } SupportedEntry;
  SupportedEntry supported [] = {
    { "|position xyz", sizeof("|position xyz") - 1, Core_VertexFormat_PositionXyz },
    { "|position xyz|ambient rgba", sizeof("|position xyz|ambient rgba") - 1, Core_VertexFormat_PositionXyzAmbientRgba },
    { "|position xyz|ambient uv", sizeof("|position xyz|ambient uv") - 1, Core_VertexFormat_PositionXyzAmbientUv },
    { "|position xyz|ambient rgba|ambient uv", sizeof("|position xyz|ambient rgba|ambient uv") - 1, Core_VertexFormat_PositionXyzAmbientRgbaAmbientUv },
    { "|ambient rgba", sizeof("|ambient rgba") - 1, Core_VertexFormat_AmbientRgba },
    { "|ambient uv", sizeof("|ambient uv") - 1, Core_VertexFormat_AmbientUv },
  };
  const size_t supportedSize = sizeof(supported) / sizeof(SupportedEntry);
  for (Core_Size i = 0; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_list_get(&child_node, node, i)) {
      CORE_UNREFERENCE(stringBuffer);
      stringBuffer = NULL;
      return Core_Failure;
    }
    Core_String* received_value = NULL;
    if (dx_ddl_node_get_string(&received_value, child_node)) {
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      CORE_UNREFERENCE(stringBuffer);
      stringBuffer = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    Core_Boolean isEqualTo[3] = { Core_False, Core_False, Core_False };
    if (Core_String_isEqualTo(&isEqualTo[0], received_value, NAME(position_xyz_string)) ||
        Core_String_isEqualTo(&isEqualTo[1], received_value, NAME(ambient_rgba_string)) ||
        Core_String_isEqualTo(&isEqualTo[2], received_value, NAME(ambient_uv_string))) {
      CORE_UNREFERENCE(received_value);
      received_value = NULL;
      CORE_UNREFERENCE(stringBuffer);
      stringBuffer = NULL;
      return Core_Failure;
    }
    Core_StringBuffer_appendBytes(stringBuffer, "|", sizeof("|") - 1);
    Core_StringBuffer_appendString(stringBuffer, received_value);
    CORE_UNREFERENCE(received_value);
    received_value = NULL;
  }
  Core_String* string;
  Core_StringBuffer_getString(&string, stringBuffer);
  CORE_UNREFERENCE(stringBuffer);
  stringBuffer = NULL;
  for (Core_Size i = 0, n = supportedSize; i < n; ++i) {
    Core_Size numberOfBytes;
    Core_String_getNumberOfBytes(&numberOfBytes, string);
    if (numberOfBytes == supported[i].numberOfBytes) {
      const void* bytes;
      Core_String_getBytes(&bytes, string);
      Core_Integer8 areEqual;
      if (Core_Memory_compare(&areEqual, bytes, supported[i].name, numberOfBytes)) {
        CORE_UNREFERENCE(string);
        string = NULL;
        return Core_Failure;
      }
      if (!areEqual) {
        CORE_UNREFERENCE(string);
        string = NULL;
        *RETURN = supported[i].value;
        return Core_Success;
      }
    }
  }
  CORE_UNREFERENCE(string);
  string = NULL;
  return Core_Failure;
}

static Core_Result _parse_mesh(Core_Assets_Mesh** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* name_value = NULL;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return Core_Failure;
    }
  }
  // generator
  Core_String* generator_value = NULL;
  {
    Core_String* temporary = NULL;
    if (Core_String_create(&temporary, "generator", sizeof("generator") - 1)) {
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, temporary)) {
      CORE_UNREFERENCE(temporary);
      temporary = NULL;
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(temporary);
    temporary = NULL;
    if (dx_ddl_node_get_string(&generator_value, child_node)) {
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
  }
  // vertexFormat
  Core_VertexFormat vertex_format_value = Core_VertexFormat_PositionXyzAmbientRgbaAmbientUv;
  {
    Core_String* name = NAME(vertex_format_key);
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, name)) {
      CORE_UNREFERENCE(generator_value);
      generator_value = NULL;
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
    if (_parse_vertex_format(&vertex_format_value, child_node, context)) {
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      CORE_UNREFERENCE(generator_value);
      generator_value = NULL;
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
  }
  // material
  Core_Assets_Ref* material_reference_value = NULL;
  {
    if (dx_adl_semantical_read_material_instance_field(&material_reference_value, node, false, NAME(material_key), context)) {
      CORE_UNREFERENCE(generator_value);
      generator_value = NULL;
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
  }
  Core_Assets_Mesh* mesh_value = NULL;
  if (Core_Assets_Mesh_create(&mesh_value, name_value, generator_value, vertex_format_value, material_reference_value)) {
    CORE_UNREFERENCE(material_reference_value);
    material_reference_value = NULL;
    CORE_UNREFERENCE(generator_value);
    generator_value = NULL;
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(material_reference_value);
  material_reference_value = NULL;
  CORE_UNREFERENCE(generator_value);
  generator_value = NULL;
  CORE_UNREFERENCE(name_value);
  name_value = NULL;
  *RETURN = mesh_value;
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  return _parse_mesh((Core_Assets_Mesh**)RETURN, node, context);
}

static Core_Result _resolve(dx_adl_type_handlers_mesh* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  Core_Assets_Mesh* mesh = CORE_ASSETS_MESH(symbol->asset);
  // operations
  {
    Core_Error last_error = Core_getError();
    Core_String* name = NAME(operations_key);
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, symbol->node, name)) {
      if (Core_Error_NotFound != Core_getError()) {
        return Core_Failure;
      } else {
        Core_setError(last_error);
      }
    } else {
      if (_parse_mesh_operations(child_node, symbol, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  if (mesh->materialReference) {
    if (mesh->materialReference->object) {
      symbol->resolved = true;
      return Core_Success;
    }
    dx_adl_symbol* referenced_symbol = NULL;
    if (dx_asset_definitions_get(&referenced_symbol, context->definitions, mesh->materialReference->name)) {
      return Core_Failure;
    }
    if (!referenced_symbol->asset) {
      CORE_UNREFERENCE(referenced_symbol);
      referenced_symbol = NULL;
      return Core_Failure;
    }
    mesh->materialReference->object = referenced_symbol->asset;
    CORE_REFERENCE(mesh->materialReference->object);
    CORE_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
  }
  symbol->resolved = true;
  return Core_Success;
}

static Core_Result _enter(dx_adl_type_handlers_mesh* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  // enter
  dx_adl_symbol* symbol = NULL;
  if (dx_adl_symbol_create(&symbol, received_type, name)) {
    return Core_Failure;
  }
  if (source) {
    symbol->node = source;
    CORE_REFERENCE(symbol->node);
  }
  if (dx_asset_definitions_set(context->definitions, name, symbol)) {
    CORE_UNREFERENCE(symbol);
    symbol = NULL;
    if (Core_Error_Exists == Core_getError()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->numberOfBytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
      CORE_UNREFERENCE(name);
      name = NULL;
      CORE_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    } else {
      CORE_UNREFERENCE(name);
      name = NULL;
      CORE_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(symbol);
  symbol = NULL;
  CORE_UNREFERENCE(name);
  name = NULL;
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_mesh_construct(dx_adl_type_handlers_mesh* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_mesh);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_mesh_destruct(dx_adl_type_handlers_mesh* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_mesh_constructDispatch(dx_adl_type_handlers_mesh_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->enter = (Core_Result(*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _enter;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_mesh_create(dx_adl_type_handlers_mesh** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_mesh);
  if (dx_adl_type_handlers_mesh_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
