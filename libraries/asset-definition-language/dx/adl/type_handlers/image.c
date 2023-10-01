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

static int _read_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static int _read_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context);

static void on_object_added(dx_object** o);

static void on_object_removed(dx_object** o);

static dx_result _read_image(dx_asset_image** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result resolve(dx_adl_type_handlers_image* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_image* SELF, dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image",
                      dx_adl_type_handlers_image,
                      dx_adl_type_handler);

static int _read_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
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

  dx_asset_image_operation* operation = NULL;
  if (dx_adl_type_handler_read((dx_object**)&operation, type_handler, node, context)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return DX_FAILURE;
  }
  reader_symbol->asset = DX_OBJECT(operation);
  DX_REFERENCE(reader_symbol->asset);
  DX_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
  if (dx_inline_object_array_append(&image->operations, DX_OBJECT(operation))) {
    DX_UNREFERENCE(operation);
    operation = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(operation);
  operation = NULL;
  return DX_SUCCESS;
}

static void on_object_added(dx_object** o) {
  DX_REFERENCE(*o);
}

static void on_object_removed(dx_object** o) {
  DX_UNREFERENCE(*o);
}

static int _read_image_operations(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
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
    if (_read_image_operation(child_node, symbol, context)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static dx_result _read_image(dx_asset_image** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  // name
  {
    if (dx_adl_semantical_read_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  // either "width", "height", and "pixelFormat" or "path"
  if (dx_ddl_node_map_get_or_null(node, NAME(width_key)) &&
      dx_ddl_node_map_get_or_null(node, NAME(height_key)) &&
      dx_ddl_node_map_get_or_null(node, NAME(pixel_format_key)) &&
      !dx_ddl_node_map_get_or_null(node, NAME(path_key))) {
    // pixelFormat
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
    if (dx_asset_image_create(&image_value, name_value, pixel_format, width_value, height_value)) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  } else if (!dx_ddl_node_map_get_or_null(node, NAME(width_key)) &&
             !dx_ddl_node_map_get_or_null(node, NAME(height_key)) &&
             !dx_ddl_node_map_get_or_null(node, NAME(pixel_format_key)) &&
             dx_ddl_node_map_get_or_null(node, NAME(path_key))) {
    dx_string* path_value = dx_adl_semantical_read_string_field(node, NAME(path_key), context->names);
    if (!path_value) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
    if (dx_asset_image_create_path(&image_value, name_value, path_value)) {
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

static dx_result resolve(dx_adl_type_handlers_image* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
  // operations?
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(symbol->node, name);
    if (child_node) {
      if (_read_image_operations(child_node, symbol, context)) {
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
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_image* SELF, dx_ddl_node* node, dx_adl_context* context) {
  return _read_image((dx_asset_image**)RETURN, node, context);
}

dx_result dx_adl_type_handlers_image_construct(dx_adl_type_handlers_image* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_image_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_image_destruct(dx_adl_type_handlers_image* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_image_dispatch_construct(dx_adl_type_handlers_image_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
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
