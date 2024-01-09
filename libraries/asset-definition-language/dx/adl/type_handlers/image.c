#include "dx/adl/type_handlers/image.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"
#include "dx/adl/enter.h"
// strlen
#include <string.h>

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, void** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, void** a, void** b);

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_image* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_image* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_image* SELF, dx_ddl_node* node);

static Core_Result _parse_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static Core_Result _parse_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static Core_Result _parse_image(dx_assets_image** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_image* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_image* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.adl.type_handlers.image",
                      dx_adl_type_handlers_image,
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

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_image* SELF) {
  Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_image* SELF) {
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
  DEFINE("width");
  DEFINE("height");
  DEFINE("pixelFormat");
  DEFINE("path");
  DEFINE("operations");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  CORE_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  CORE_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_image* SELF, dx_ddl_node* node) {
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

static Core_Result _parse_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo[2] = { Core_False, Core_False };
  if (Core_String_isEqualTo(&isEqualTo[0], received_type, NAME(image_operations_color_fill_type)) ||
      Core_String_isEqualTo(&isEqualTo[1], received_type, NAME(image_operations_checkerboard_pattern_fill_type))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo[0] && !isEqualTo[1]) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  dx_adl_symbol* reader_symbol = NULL;
  if (dx_adl_symbol_create(&reader_symbol, received_type, dx_adl_names_create_unique_name(context->names))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  reader_symbol->node = node;
  CORE_REFERENCE(reader_symbol->node);
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    CORE_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return Core_Failure;
  }

  dx_adl_type_handler* type_handler = NULL;
  if (Core_InlinePointerHashmap_get(&type_handler, &context->type_handlers, received_type)) {
    CORE_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    CORE_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;

  dx_assets_image_operation* operation = NULL;
  if (dx_adl_type_handler_read((Core_Object**)&operation, type_handler, node, context)) {
    CORE_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return Core_Failure;
  }
  reader_symbol->asset = CORE_OBJECT(operation);
  CORE_REFERENCE(reader_symbol->asset);
  CORE_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_assets_image* image = DX_ASSETS_IMAGE(symbol->asset);
  if (dx_inline_object_array_append(&image->operations, CORE_OBJECT(operation))) {
    CORE_UNREFERENCE(operation);
    operation = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(operation);
  operation = NULL;
  return Core_Success;
}

static Core_Result _parse_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
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
    if (_parse_image_operation(child_node, symbol, context)) {
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
  }
  return Core_Success;
}

static Core_Result _parse_image_with_path(dx_assets_image** RETURN, Core_String* name_value, dx_ddl_node* node, dx_adl_context* context) {
  // Case of "path".
  Core_String* path_value = dx_adl_semantical_read_string_field(node, NAME(path_key), context->names);
  if (!path_value) {
    return Core_Failure;
  }
  if (dx_assets_image_create_path(RETURN, name_value, path_value)) {
    CORE_UNREFERENCE(path_value);
    path_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(path_value);
  path_value = NULL;
  return Core_Success;
}

static Core_Result _parse_image_with_size_and_format(dx_assets_image** RETURN, Core_String* name_value, dx_ddl_node* node, dx_adl_context* context) {
  // Case of "width", "height", and "pixelFormat".
  Core_Size width_value, height_value;
  Core_String* pixel_format_string = dx_adl_semantical_read_string_field(node, NAME(pixel_format_key), context->names);
  if (!pixel_format_string) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo[2] = { Core_False, Core_False };
  if (Core_String_isEqualTo(&isEqualTo[0], pixel_format_string, NAME(pixel_format_rn8_gn8_bn8_string)) ||
      Core_String_isEqualTo(&isEqualTo[1], pixel_format_string, NAME(pixel_format_bn8_gn8_rn8_string))) {
    CORE_UNREFERENCE(pixel_format_string);
    pixel_format_string = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(pixel_format_string);
  pixel_format_string = NULL;
  Core_PixelFormat pixel_format_value;
  if (isEqualTo[0]) {
    pixel_format_value = Core_PixelFormat_Rgb8;
  } else if (isEqualTo[1]) {
    pixel_format_value = Core_PixelFormat_Bgr8;
  } else {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  // width
  if (dx_adl_semantical_read_sz(&width_value, node, NAME(width_key))) {
    return Core_Failure;
  }
  // height
  if (dx_adl_semantical_read_sz(&height_value, node, NAME(height_key))) {
    return Core_Failure;
  }
  if (dx_assets_image_create(RETURN, name_value, pixel_format_value, width_value, height_value)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result _parse_image(dx_assets_image** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_assets_image* image_value = NULL;
  Core_String* name_value = NULL;
  Core_Size width_value, height_value;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return Core_Failure;
    }
  }
  Core_String* CHILD_NODE_NAMES[4] = { NULL };
  CHILD_NODE_NAMES[0] = NAME(width_key);
  CHILD_NODE_NAMES[1] = NAME(height_key);
  CHILD_NODE_NAMES[2] = NAME(pixel_format_key);
  CHILD_NODE_NAMES[3] = NAME(path_key);
  Core_Boolean child_nodes[4] = { Core_False };

  for (Core_Size i = 0, n = 4; i < n; ++i) {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, CHILD_NODE_NAMES[i])) {
      if (Core_Error_NotFound != Core_getError()) {
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
      child_nodes[i] = Core_False;
    } else {
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      child_nodes[i] = Core_True;
    }
  }
  if (child_nodes[0] && child_nodes[1] && child_nodes[2] && !child_nodes[3]) {
    // Case of "width", "height", and "pixelFormat".
    if (_parse_image_with_size_and_format(&image_value, name_value, node, context)) {
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
  } else if (!child_nodes[0] && !child_nodes[1] && !child_nodes[2] && child_nodes[3]) {
    // Case of "path".
    if (_parse_image_with_path(&image_value, name_value, node, context)) {
      CORE_UNREFERENCE(name_value);
      name_value = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
  } else {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
    return Core_Failure;
  }
  *RETURN = image_value;
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_image* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  return _parse_image((dx_assets_image**)RETURN, node, context);
}

static Core_Result _resolve(dx_adl_type_handlers_image* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  dx_assets_image* image = DX_ASSETS_IMAGE(symbol->asset);
  // operations?
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
      if (_parse_image_operations(child_node, symbol, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_image_construct(dx_adl_type_handlers_image* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_image);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_image_destruct(dx_adl_type_handlers_image* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_image_constructDispatch(dx_adl_type_handlers_image_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_image_create(dx_adl_type_handlers_image** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_image);
  if (dx_adl_type_handlers_image_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
