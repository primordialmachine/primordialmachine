#include "dx/adl/type_handlers/image_operations_checkerboard_pattern_fill.h"

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

static int resolve(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image_operations.checkerboard_pattern_fill_reader",
                      dx_adl_type_handlers_image_operations_checkerboard_pattern_fill,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* asset = DX_ASSET_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(symbol->asset);
  dx_ddl_node* node = symbol->node;
  // checkerColors
  {
    dx_string* child_name = NULL;
    if (dx_string_create(&child_name, "checkerColors", sizeof("checkerColors") - 1)) {
      return 1;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      return 1;
    }
    // checkerColors.first
    {
      dx_asset_reference* color_reference = dx_adl_semantical_read_color_instance_field(child_node, false, NAME(first_key), context);
      if (!color_reference) {
        return 1;
      }
      dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, color_reference->name));
      dx_asset_color_rgb_n8* color_asset = DX_ASSET_COLOR_RGB_N8(color_symbol->asset);
      DX_UNREFERENCE(color_reference);
      color_reference = NULL;
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_first_checker_color(asset, color_asset)) {
        return 1;
      }
    }
    // checkerColors.second
    {
      dx_asset_reference* color_reference = dx_adl_semantical_read_color_instance_field(child_node, false, NAME(second_key), context);
      if (!color_reference) {
        return 1;
      }
      dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, color_reference->name));
      dx_asset_color_rgb_n8* color_asset = DX_ASSET_COLOR_RGB_N8(color_symbol->asset);
      DX_UNREFERENCE(color_reference);
      color_reference = NULL;
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_second_checker_color(asset, color_asset)) {
        return 1;
      }
    }
  }
  //
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  dx_asset_image_operations_checkerboard_pattern_fill* image_operation = dx_asset_image_operations_checkerboard_pattern_fill_create();
  if (!image_operation) {
    return NULL;
  }
  // numberOfCheckers
  {
    dx_string* child_name = NULL;
    if (dx_string_create(&child_name, "numberOfCheckers", sizeof("numberOfCheckers") - 1)) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    // numberOfCheckers.horizontal
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(horizontal_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_horizontal(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
    // numberOfCheckers.vertical
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(vertical_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_number_of_checkers_vertical(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
  }
  // checkerSize
  {
    dx_string* child_name = NULL;
    if (dx_string_create(&child_name, "checkerSize", sizeof("checkerSize") - 1)) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, child_name);
    DX_UNREFERENCE(child_name);
    child_name = NULL;
    if (!child_node) {
      DX_UNREFERENCE(image_operation);
      image_operation = NULL;
      return NULL;
    }
    // checkerSize.horizontal
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(horizontal_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_horizontal(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
    // checkerSize.vertical
    {
      dx_size value;
      if (dx_adl_semantical_read_sz(child_node, NAME(vertical_key), &value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
      if (dx_asset_image_operations_checkerboard_pattern_fill_set_checker_size_vertical(image_operation, value)) {
        DX_UNREFERENCE(image_operation);
        image_operation = NULL;
        return NULL;
      }
    }
  }
  return DX_OBJECT(image_operation);
}

int dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self) {
  dx_rti_type* _type = dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(self))) {
    return 1;
  }
  DX_ADL_TYPE_HANDLER(self)->resolve = (int(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
  DX_ADL_TYPE_HANDLER(self)->read = (dx_object * (*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_destruct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_dispatch_construct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_create() {
  dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* self = DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_CHECKERBOARD_PATTERN_FILL(dx_object_alloc(sizeof(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
