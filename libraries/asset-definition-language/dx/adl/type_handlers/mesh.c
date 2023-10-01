#include "dx/adl/type_handlers/mesh.h"

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

static dx_result _initialize_expected_keys(dx_adl_type_handlers_mesh* SELF);

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_mesh* SELF);

static dx_result _check_keys(dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node);

static dx_result _parse_mesh_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result _parse_mesh_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result _parse_vertex_format(dx_vertex_format* RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse_mesh(dx_assets_mesh** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node, dx_adl_context* context);

static dx_result _resolve(dx_adl_type_handlers_mesh* SELF, dx_adl_symbol *symbol, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh",
                      dx_adl_type_handlers_mesh,
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

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_mesh* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return DX_SUCCESS;
}

static dx_result _initialize_expected_keys(dx_adl_type_handlers_mesh* SELF) {
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
  DEFINE("name");
  DEFINE("vertexFormat");
  DEFINE("generator");
  DEFINE("operations");
  DEFINE("material");
#undef DEFINE
  return DX_SUCCESS;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static dx_result _check_keys(dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node) {
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

static dx_result _parse_mesh_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (node->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return DX_FAILURE;
  }

  dx_adl_symbol* reader_symbol = NULL;
  if (dx_adl_symbol_create(&reader_symbol, received_type, dx_adl_names_create_unique_name(context->names))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return DX_FAILURE;
  }

  dx_adl_type_handler* type_handler = NULL;
  if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, received_type)) {
    if (dx_get_error() != DX_ERROR_NOT_FOUND) {
      DX_UNREFERENCE(reader_symbol);
      reader_symbol = NULL;
      return DX_FAILURE;
    } else {
      DX_UNREFERENCE(reader_symbol);
      reader_symbol = NULL;
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
      return DX_FAILURE;
    }
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_asset_mesh_operation* operation = NULL;
  if (dx_adl_type_handler_read((dx_object**)&operation, type_handler, node, context)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return DX_FAILURE;
  }
  reader_symbol->asset = DX_OBJECT(operation);
  DX_REFERENCE(reader_symbol->asset);
  DX_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_assets_mesh* mesh = DX_ASSETS_MESH(symbol->asset);
  if (dx_inline_object_array_append(&mesh->operations, DX_OBJECT(operation))) {
    DX_UNREFERENCE(operation);
    operation = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(operation);
  operation = NULL;
  return DX_SUCCESS;
}

static dx_result _parse_mesh_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  dx_size n;
  if (dx_ddl_node_list_get_size(&n, node)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_list_get(&child_node, node, i)) {
      return DX_FAILURE;
    }
    if (_parse_mesh_operation(child_node, symbol, context)) {
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(child_node);
    child_node = NULL;
  }
  return DX_SUCCESS;
}

static dx_result _parse_vertex_format(dx_vertex_format* RETURN, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  dx_vertex_format vertex_format = 0;
  dx_size n;
  if (dx_ddl_node_list_get_size(&n, node)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_list_get(&child_node, node, i)) {
      return DX_FAILURE;
    }
    dx_string* received_value = NULL;
    if (dx_ddl_node_get_string(&received_value, child_node)) {
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      return DX_FAILURE;
    }
    if (dx_string_is_equal_to(received_value, NAME(position_xyz_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format |= dx_vertex_format_position_xyz;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_rgba_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format |= dx_vertex_format_ambient_rgba;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_uv_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format |= dx_vertex_format_ambient_uv;
    } else {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(child_node);
    child_node = NULL;
  }
  *RETURN = vertex_format;
  return DX_SUCCESS;
}

static dx_result _parse_mesh(dx_assets_mesh** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* name_value = NULL;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  // generator
  dx_string* generator_value = NULL;
  {
    dx_string* temporary = NULL;
    if (dx_string_create(&temporary, "generator", sizeof("generator") - 1)) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, temporary)) {
      DX_UNREFERENCE(temporary);
      temporary = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(temporary);
    temporary = NULL;
    if (dx_ddl_node_get_string(&generator_value, child_node)) {
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(child_node);
    child_node = NULL;
  }
  // vertexFormat
  dx_vertex_format vertex_format_value = dx_vertex_format_position_xyz_ambient_rgba_ambient_uv;
  {
    dx_string* name = NAME(vertex_format_key);
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, name)) {
      DX_UNREFERENCE(generator_value);
      generator_value = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    if (_parse_vertex_format(&vertex_format_value, child_node, context)) {
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      DX_UNREFERENCE(generator_value);
      generator_value = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(child_node);
    child_node = NULL;
  }
  // material
  dx_asset_reference* material_reference_value = NULL;
  {
    if (dx_adl_semantical_read_material_instance_field(&material_reference_value, node, false, NAME(material_key), context)) {
      DX_UNREFERENCE(generator_value);
      generator_value = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
  }
  dx_assets_mesh* mesh_value = NULL;
  if (dx_assets_mesh_create(&mesh_value, name_value, generator_value, vertex_format_value, material_reference_value)) {
    DX_UNREFERENCE(material_reference_value);
    material_reference_value = NULL;
    DX_UNREFERENCE(generator_value);
    generator_value = NULL;
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(material_reference_value);
  material_reference_value = NULL;
  DX_UNREFERENCE(generator_value);
  generator_value = NULL;
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  *RETURN = mesh_value;
  return DX_SUCCESS;
}

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (_check_keys(SELF, node)) {
    return DX_FAILURE;
  }
  return _parse_mesh((dx_assets_mesh**)RETURN, node, context);
}

static dx_result _resolve(dx_adl_type_handlers_mesh* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_assets_mesh* mesh = DX_ASSETS_MESH(symbol->asset);
  // operations
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, symbol->node, name)) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return DX_FAILURE;
      } else {
        dx_set_error(last_error);
      }
    } else {
      if (_parse_mesh_operations(child_node, symbol, context)) {
        DX_UNREFERENCE(child_node);
        child_node = NULL;
        return DX_FAILURE;
      }
      DX_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  if (mesh->material_reference) {
    if (mesh->material_reference->object) {
      symbol->resolved = true;
      return DX_SUCCESS;
    }
    dx_adl_symbol* referenced_symbol = NULL;
    if (dx_asset_definitions_get(&referenced_symbol, context->definitions, mesh->material_reference->name)) {
      return DX_FAILURE;
    }
    if (!referenced_symbol->asset) {
      DX_UNREFERENCE(referenced_symbol);
      referenced_symbol = NULL;
      return DX_FAILURE;
    }
    mesh->material_reference->object = referenced_symbol->asset;
    DX_REFERENCE(mesh->material_reference->object);
    DX_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_mesh_construct(dx_adl_type_handlers_mesh* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_mesh_get_type();
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

static void dx_adl_type_handlers_mesh_destruct(dx_adl_type_handlers_mesh* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_mesh_dispatch_construct(dx_adl_type_handlers_mesh_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

dx_result dx_adl_type_handlers_mesh_create(dx_adl_type_handlers_mesh** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_mesh)
  if (dx_adl_type_handlers_mesh_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
