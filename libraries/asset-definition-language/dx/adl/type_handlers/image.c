#include "dx/adl/type_handlers/image.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"
#include "dx/adl/enter.h"
// strlen
#include <string.h>

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static dx_result _on_hash_expected_key_key(dx_size* RETURN, void** a);

static dx_result _on_compare_expected_key_keys(dx_bool* RETURN, void** a, void** b);

static dx_result _initialize_expected_keys(dx_adl_type_handlers_image* SELF);

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_image* SELF);

static dx_result _check_keys(dx_adl_type_handlers_image* SELF, dx_ddl_node* node);

static dx_result _parse_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result _parse_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result _parse_image(dx_assets_image** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_image* SELF, dx_ddl_node* node, dx_adl_context* context);

static dx_result _resolve(dx_adl_type_handlers_image* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image",
                      dx_adl_type_handlers_image,
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

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_image* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return DX_SUCCESS;
}

static dx_result _initialize_expected_keys(dx_adl_type_handlers_image* SELF) {
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
  DEFINE("width");
  DEFINE("height");
  DEFINE("pixelFormat");
  DEFINE("path");
  DEFINE("operations");
#undef DEFINE
  return DX_SUCCESS;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static dx_result _check_keys(dx_adl_type_handlers_image* SELF, dx_ddl_node* node) {
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

static dx_result _parse_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(image_operations_color_fill_type)) &&
      !dx_string_is_equal_to(received_type, NAME(image_operations_checkerboard_pattern_fill_type))) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }

  dx_adl_symbol* reader_symbol = NULL;
  if (dx_adl_symbol_create(&reader_symbol, received_type, dx_adl_names_create_unique_name(context->names))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  reader_symbol->node = node;
  DX_REFERENCE(reader_symbol->node);
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return DX_FAILURE;
  }

  dx_adl_type_handler* type_handler = NULL;
  if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, received_type)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;

  dx_assets_image_operation* operation = NULL;
  if (dx_adl_type_handler_read((dx_object**)&operation, type_handler, node, context)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return DX_FAILURE;
  }
  reader_symbol->asset = DX_OBJECT(operation);
  DX_REFERENCE(reader_symbol->asset);
  DX_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_assets_image* image = DX_ASSETS_IMAGE(symbol->asset);
  if (dx_inline_object_array_append(&image->operations, DX_OBJECT(operation))) {
    DX_UNREFERENCE(operation);
    operation = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(operation);
  operation = NULL;
  return DX_SUCCESS;
}

static dx_result _parse_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
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
    if (_parse_image_operation(child_node, symbol, context)) {
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(child_node);
    child_node = NULL;
  }
  return DX_SUCCESS;
}

static dx_result _parse_image(dx_assets_image** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_assets_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  dx_string* CHILD_NODE_NAMES[4] = { NULL };
  CHILD_NODE_NAMES[0] = NAME(width_key);
  CHILD_NODE_NAMES[1] = NAME(height_key);
  CHILD_NODE_NAMES[2] = NAME(pixel_format_key);
  CHILD_NODE_NAMES[3] = NAME(path_key);
  dx_bool child_nodes[4] = { DX_FALSE };

  for (dx_size i = 0, n = 4; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, CHILD_NODE_NAMES[i])) {
      if (DX_ERROR_NOT_FOUND != dx_get_error()) {
        return DX_FAILURE;
      } else {
        dx_set_error(DX_NO_ERROR);
      }
      child_nodes[i] = DX_FALSE;
    } else {
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      child_nodes[i] = DX_TRUE;
    }
  }
  if (child_nodes[0] && child_nodes[1] && child_nodes[2] && !child_nodes[3]) {
    // case of "width", "height", and "pixelFormat".
    dx_string* pixel_format_string = dx_adl_semantical_read_string_field(node, NAME(pixel_format_key), context->names);
    if (!pixel_format_string) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    dx_pixel_format pixel_format;
    if (dx_string_is_equal_to(pixel_format_string, NAME(pixel_format_rn8_gn8_bn8_string))) {
      pixel_format = dx_pixel_format_rn8_gn8_bn8;
    } else if (dx_string_is_equal_to(pixel_format_string, NAME(pixel_format_bn8_gn8_rn8_string))) {
      pixel_format = dx_pixel_format_bn8_gn8_rn8;
    } else {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
      DX_UNREFERENCE(pixel_format_string);
      pixel_format_string = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(pixel_format_string);
    pixel_format_string = NULL;
    // width
    if (dx_adl_semantical_read_sz(&width_value, node, NAME(width_key))) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    // height
    if (dx_adl_semantical_read_sz(&height_value, node, NAME(height_key))) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    if (dx_assets_image_create(&image_value, name_value, pixel_format, width_value, height_value)) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  } else if (!child_nodes[0] && !child_nodes[1] && !child_nodes[2] && child_nodes[3]) {
    // case of "path"
    dx_string* path_value = dx_adl_semantical_read_string_field(node, NAME(path_key), context->names);
    if (!path_value) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    if (dx_assets_image_create_path(&image_value, name_value, path_value)) {
      DX_UNREFERENCE(path_value);
      path_value = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(path_value);
    path_value = NULL;
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  } else {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    return DX_FAILURE;
  }
  *RETURN = image_value;
  return DX_SUCCESS;
}

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_image* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (_check_keys(SELF, node)) {
    return DX_FAILURE;
  }
  return _parse_image((dx_assets_image**)RETURN, node, context);
}

static dx_result _resolve(dx_adl_type_handlers_image* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_assets_image* image = DX_ASSETS_IMAGE(symbol->asset);
  // operations?
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, symbol->node, name)) {
      if (DX_ERROR_NOT_FOUND != dx_get_error()) {
        return DX_FAILURE;
      } else {
        dx_set_error(last_error);
      }
    } else {
      if (_parse_image_operations(child_node, symbol, context)) {
        DX_UNREFERENCE(child_node);
        child_node = NULL;
        return DX_FAILURE;
      }
      DX_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_image_construct(dx_adl_type_handlers_image* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_image_get_type();
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

static void dx_adl_type_handlers_image_destruct(dx_adl_type_handlers_image* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_image_dispatch_construct(dx_adl_type_handlers_image_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

dx_result dx_adl_type_handlers_image_create(dx_adl_type_handlers_image** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_image)
  if (dx_adl_type_handlers_image_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
