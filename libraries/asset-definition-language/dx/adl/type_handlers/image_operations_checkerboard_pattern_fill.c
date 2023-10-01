#include "dx/adl/type_handlers/image_operations_checkerboard_pattern_fill.h"

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

static dx_result resolve(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image_operations.checkerboard_pattern_fill_reader",
                      dx_adl_type_handlers_image_operations_checkerboard_pattern_fill,
                      dx_adl_type_handler);

static dx_result _parse_checker_colors(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_asset_image_operations_checkerboard_pattern_fill *asset, dx_ddl_node* node, dx_adl_context* context) {
  // checkerColors
  dx_string* child_name = NULL;
  if (dx_string_create(&child_name, "checkerColors", sizeof("checkerColors") - 1)) {
    return DX_FAILURE;
  }
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, child_name);
  DX_UNREFERENCE(child_name);
  child_name = NULL;
  if (!child_node) {
    return DX_FAILURE;
  }
  // checkerColors.first
  if (dx_adl_semantical_read_color_instance_field(&asset->first_checker_color, child_node, false, NAME(first_key), context)) {
    return DX_FAILURE;
  }
  // checkerColors.second
  if (dx_adl_semantical_read_color_instance_field(&asset->second_checker_color, child_node, false, NAME(second_key), context)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result resolve(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* asset = DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(symbol->asset);
  // checkerColors
  {
    // checkerColors.first
    {
      dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, asset->first_checker_color->name));
      ///@todo Handle symbol not found situation.
      dx_assets_color_rgb_n8* color_asset = DX_ASSETS_COLOR_RGB_N8(color_symbol->asset);
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(asset, color_asset)) {
        return DX_FAILURE;
      }
    }
    // checkerColors.second
    {
      dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, asset->second_checker_color->name));
      ///@todo Handle symbol not found situation.
      dx_assets_color_rgb_n8* color_asset = DX_ASSETS_COLOR_RGB_N8(color_symbol->asset);
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(asset, color_asset)) {
        return DX_FAILURE;
      }
    }
  }
  //
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* image_operation = NULL;
  if (dx_asset_image_operations_checkerboard_pattern_fill_create(&image_operation)) {
    return DX_FAILURE;
  }
  // numberOfCheckers
  {
    dx_string* child_name = NULL;
    if (dx_string_create(&child_name, "numberOfCheckers", sizeof("numberOfCheckers") - 1)) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return DX_FAILURE;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return DX_FAILURE;
    }
    // numberOfCheckers.horizontal
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(horizontal_key))) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_horizontal(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
    }
    // numberOfCheckers.vertical
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(vertical_key))) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_vertical(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
    }
  }
  // checkerSize
  {
    dx_string* child_name = NULL;
    if (dx_string_create(&child_name, "checkerSize", sizeof("checkerSize") - 1)) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return DX_FAILURE;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return DX_FAILURE;
    }
    // checkerSize.horizontal
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(horizontal_key))) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_horizontal(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
    }
    // checkerSize.vertical
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(vertical_key))) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_vertical(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return DX_FAILURE;
      }
    }
  }
  // checkerColors
  if (_parse_checker_colors(SELF, image_operation, node, context)) {
    DX_UNREFERENCE(image_operation);
    image_operation = NULL;
    return DX_FAILURE;
  }
  *RETURN = DX_OBJECT(image_operation);
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_destruct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_dispatch_construct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_create(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill)
  if (dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
