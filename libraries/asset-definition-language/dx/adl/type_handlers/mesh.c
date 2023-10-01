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

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context);

static void on_object_added(dx_object** o);

static void on_object_removed(dx_object** o);

static int _read_mesh_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static int _read_mesh_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static int _read_vertex_format(dx_ddl_node* node, dx_adl_context* context, dx_vertex_format* vertex_format);

static dx_asset_mesh* _read_mesh(dx_ddl_node* node, dx_adl_context* context);

static dx_result resolve(dx_adl_type_handlers_mesh* SELF,
                         dx_adl_symbol *symbol,
                         dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_mesh* SELF,
                      dx_ddl_node* node,
                      dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh",
                      dx_adl_type_handlers_mesh,
                      dx_adl_type_handler);

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_material* asset = NULL;
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(material_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_type_handler* type_handler = NULL;
    if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, NAME(material_type))) {
      return NULL;
    }
    if (dx_adl_type_handler_read((dx_object**)&asset, type_handler, node, context)) {
      goto END;
    }
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

static void on_object_added(dx_object** o) {
  DX_REFERENCE(*o);
}

static void on_object_removed(dx_object** o) {
  DX_UNREFERENCE(*o);
}

static int _read_mesh_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return 1;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return 1;
  }

  dx_adl_symbol* reader_symbol = NULL;
  if (dx_adl_symbol_create(&reader_symbol, received_type, dx_adl_names_create_unique_name(context->names))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return 1;
  }
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 0;
  }

  dx_adl_type_handler* type_handler = NULL;
  if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, received_type)) {
    if (dx_get_error() != DX_ERROR_NOT_FOUND) {
      DX_UNREFERENCE(reader_symbol);
      reader_symbol = NULL;
      return 1;
    } else {
      DX_UNREFERENCE(reader_symbol);
      reader_symbol = NULL;
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
      return 1;
    }
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_asset_mesh_operation* operation = NULL;
  if (dx_adl_type_handler_read((dx_object**)&operation, type_handler, node, context)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 1;
  }
  reader_symbol->asset = DX_OBJECT(operation);
  DX_REFERENCE(reader_symbol->asset);
  DX_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_asset_mesh* mesh = DX_ASSET_MESH(symbol->asset);
  if (dx_inline_object_array_append(&mesh->operations, DX_OBJECT(operation))) {
    DX_UNREFERENCE(operation);
    operation = NULL;
    return 1;
  }
  DX_UNREFERENCE(operation);
  operation = NULL;
  return 0;
}

static int _read_mesh_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return 1;
  }
  dx_size n;
  if (dx_ddl_node_list_get_size(&n, node)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_list_get(&child_node, node, i)) {
      return 1;
    }
    if (_read_mesh_operation(child_node, symbol, context)) {
      return 1;
    }
  }
  return 0;
}

static int _read_vertex_format(dx_ddl_node* node, dx_adl_context* context, dx_vertex_format* vertex_format) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return 1;
  }
  dx_vertex_format vertex_format_1 = 0;
  dx_size n;
  if (dx_ddl_node_list_get_size(&n, node)) {
    return 1;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_list_get(&child_node, node, i)) {
      return 1;
    }
    dx_string* received_value = dx_ddl_node_get_string(child_node);
    if (dx_string_is_equal_to(received_value, NAME(position_xyz_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format_1 |= dx_vertex_format_position_xyz;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_rgba_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format_1 |= dx_vertex_format_ambient_rgba;
    } else if (dx_string_is_equal_to(received_value, NAME(ambient_uv_string))) {
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      vertex_format_1 |= dx_vertex_format_ambient_uv;
    } else {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
      DX_UNREFERENCE(received_value);
      received_value = NULL;
      return 1;
    }
  }
  (*vertex_format) = vertex_format_1;
  return 0;
}

static dx_asset_mesh* _read_mesh(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_mesh* mesh_value = NULL;
  dx_string* name_value = NULL;
  dx_string* generator_value = NULL;
  dx_asset_reference* material_reference_value = NULL;
  // name
  {
    if (dx_adl_semantical_read_name(&name_value, node, context)) {
      goto END;
    }
  }
  // generator
  {
    dx_string* name = NULL;
    if (dx_string_create(&name, "generator", sizeof("generator") - 1)) {
      goto END;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      goto END;
    }
    if (dx_ddl_node_get_string_2(&generator_value, child_node)) {
      goto END;
    }
  }
  // vertexFormat
  dx_vertex_format vertex_format_value = dx_vertex_format_position_xyz_ambient_rgba_ambient_uv;
  {
    dx_string* name = NAME(vertex_format_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    if (dx_get_error()) {
      goto END;
    }
    if (child_node) {
      if (_read_vertex_format(child_node, context, &vertex_format_value)) {
        goto END;
      }
    }
  }
  // material
  {
    if (dx_adl_semantical_read_material_instance_field(&material_reference_value, node, false, NAME(material_key), context)) {
      goto END;
    }
  }
  if (dx_asset_mesh_create(&mesh_value, name_value, generator_value, vertex_format_value, material_reference_value)) {
    DX_UNREFERENCE(material_reference_value);
    material_reference_value = NULL;
    DX_UNREFERENCE(generator_value);
    generator_value = NULL;
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    goto END;
  }
  DX_UNREFERENCE(material_reference_value);
  material_reference_value = NULL;
  DX_UNREFERENCE(generator_value);
  generator_value = NULL;
  DX_UNREFERENCE(name_value);
  name_value = NULL;
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (generator_value) {
    DX_UNREFERENCE(generator_value);
    generator_value = NULL;
  }
  if (material_reference_value) {
    DX_UNREFERENCE(material_reference_value);
    material_reference_value = NULL;
  }
  return mesh_value;
}

static dx_result resolve(dx_adl_type_handlers_mesh* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_mesh* mesh = DX_ASSET_MESH(symbol->asset);
  // operations
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(symbol->node, name);
    if (child_node) {
      if (_read_mesh_operations(child_node, symbol, context)) {
        return DX_FAILURE;
      }
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return DX_FAILURE;
      } else {
        dx_set_error(last_error);
      }
    }
  }
  if (mesh->material_reference) {
    if (mesh->material_reference->object) {
      symbol->resolved = true;
      return DX_SUCCESS;
    }
    dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, mesh->material_reference->name);
    if (!referenced_symbol) {
      return DX_FAILURE;
    }
    if (!referenced_symbol->asset) {
      return DX_FAILURE;
    }
    mesh->material_reference->object = referenced_symbol->asset;
    DX_REFERENCE(mesh->material_reference->object);
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_mesh* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_object* temporary = DX_OBJECT(_read_mesh(node, context));
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
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
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_mesh_destruct(dx_adl_type_handlers_mesh* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_mesh_dispatch_construct(dx_adl_type_handlers_mesh_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  // @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
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
