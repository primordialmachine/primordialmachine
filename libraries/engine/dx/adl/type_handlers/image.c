#include "dx/adl/type_handlers/image.h"

#include "dx/assets.h"
#include "dx/adl/read.h"
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

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context);

static int resolve(dx_adl_type_handlers_image* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_image* self, dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image",
                      dx_adl_type_handlers_image,
                      dx_adl_type_handler);

static int _read_image_operation(dx_ddl_node* node, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return 1;
  }

  if (!dx_string_is_equal_to(received_type, NAME(image_operations_color_fill_type)) &&
      !dx_string_is_equal_to(received_type, NAME(image_operations_checkerboard_pattern_fill_type))) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return 1;
  }

  dx_adl_symbol* reader_symbol = dx_adl_symbol_create(received_type, dx_adl_names_create_unique_name(context->names));
  if (!reader_symbol) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return 1;
  }
  reader_symbol->node = node;
  DX_REFERENCE(reader_symbol->node);
  if (dx_asset_definitions_set(context->definitions, reader_symbol->name, reader_symbol)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 0;
  }

  dx_adl_type_handler* reader = NULL;
  if (dx_inline_pointer_hashmap_get(&reader, &context->readers, received_type)) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 1;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;

  dx_asset_image_operation* operation = DX_ASSET_IMAGE_OPERATION(dx_adl_type_handler_read(reader, node, context));
  if (!operation) {
    DX_UNREFERENCE(reader_symbol);
    reader_symbol = NULL;
    return 1;
  }
  reader_symbol->asset = DX_OBJECT(operation);
  DX_REFERENCE(reader_symbol->asset);
  DX_UNREFERENCE(reader_symbol);
  reader_symbol = NULL;
  dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
  if (dx_inline_object_array_append(&image->operations, DX_OBJECT(operation))) {
    DX_UNREFERENCE(operation);
    operation = NULL;
    return 1;
  }
  DX_UNREFERENCE(operation);
  operation = NULL;
  return 0;
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
    return 1;
  }
  if (node->kind != dx_ddl_node_kind_list) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return 1;
  }
  for (dx_size i = 0, n = dx_ddl_node_list_get_size(node); i < n; ++i) {
    dx_ddl_node* child_node = dx_ddl_node_list_get(node, i);
    if (!child_node) {
      return 1;
    }
    if (_read_image_operation(child_node, symbol, context)) {
      return 1;
    }
  }
  return 0;
}

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_image* image_value = NULL;
  dx_string* name_value = NULL;
  dx_size width_value, height_value;
  dx_asset_color_rgb_n8* color_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
    if (!name_value) {
      goto END;
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
      goto END;
    }
    DX_PIXEL_FORMAT pixel_format;
    if (dx_string_is_equal_to(pixel_format_string, NAME(pixel_format_rn8_gn8_bn8_string))) {
      pixel_format = DX_PIXEL_FORMAT_RN8_GN8_BN8;
    } else if (dx_string_is_equal_to(pixel_format_string, NAME(pixel_format_bn8_gn8_rn8_string))) {
      pixel_format = DX_PIXEL_FORMAT_BN8_GN8_RN8;
    } else {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
      goto END;
    }
    // width
    if (dx_adl_semantical_read_sz(node, NAME(width_key), &width_value)) {
      goto END;
    }
    // height
    if (dx_adl_semantical_read_sz(node, NAME(height_key), &height_value)) {
      goto END;
    }
    if (dx_asset_image_create(&image_value, name_value, pixel_format, width_value, height_value)) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      goto END;
    }
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  } else if (!dx_ddl_node_map_get_or_null(node, NAME(width_key)) &&
             !dx_ddl_node_map_get_or_null(node, NAME(height_key)) &&
             !dx_ddl_node_map_get_or_null(node, NAME(pixel_format_key)) &&
             dx_ddl_node_map_get_or_null(node, NAME(path_key))) {
    dx_string* path_value = dx_adl_semantical_read_string_field(node, NAME(path_key), context->names);
    if (!path_value) {
      goto END;
    }
    if (dx_asset_image_create_path(&image_value, name_value, path_value)) {
      DX_UNREFERENCE(path_value);
      path_value = NULL;
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      goto END;
    }
    DX_UNREFERENCE(path_value);
    path_value = NULL;
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  } else {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    goto END;
  }

END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (color_value) {
    DX_UNREFERENCE(color_value);
    color_value = NULL;
  }
  return image_value;
}

static int resolve(dx_adl_type_handlers_image* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
  // operations?
  {
    dx_error last_error = dx_get_error();
    dx_string* name = NAME(operations_key);
    dx_ddl_node* child_node = dx_ddl_node_map_get(symbol->node, name);
    if (child_node) {
      if (_read_image_operations(child_node, symbol, context)) {
        return 1;
      }
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return 1;
      } else {
        dx_set_error(last_error);
      }
    }
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_type_handlers_image* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_image(node, context));
}

int dx_adl_type_handlers_image_construct(dx_adl_type_handlers_image* self) {
  dx_rti_type* _type = dx_adl_type_handlers_image_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(self))) {
    return 1;
  }
  DX_ADL_TYPE_HANDLER(self)->resolve = (int(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*))&resolve;
  DX_ADL_TYPE_HANDLER(self)->read = (dx_object*(*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_type_handlers_image_destruct(dx_adl_type_handlers_image* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_image_dispatch_construct(dx_adl_type_handlers_image_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_image* dx_adl_type_handlers_image_create() {
  dx_adl_type_handlers_image* self = DX_ADL_SEMANTICAL_IMAGE_READER(dx_object_alloc(sizeof(dx_adl_type_handlers_image)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_image_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
