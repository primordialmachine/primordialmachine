#include "dx/adl/type_handlers/image_operations_color_fill.h"

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

static dx_result resolve(dx_adl_type_handlers_image_operations_color_fill* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_image_operations_color_fill* SELF, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image_operations.color_fill_reader",
                      dx_adl_type_handlers_image_operations_color_fill,
                      dx_adl_type_handler);

static dx_result resolve(dx_adl_type_handlers_image_operations_color_fill* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_image_operations_color_fill* asset = DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(symbol->asset);
  dx_ddl_node* node = symbol->node;
  // color
  {
    dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, asset->color->name));
    ///@todo Handle symbol not found situation.
    dx_assets_color_rgb_n8* color_asset = DX_ASSETS_COLOR_RGB_N8(color_symbol->asset);
    if (dx_asset_image_operations_color_fill_set_color(asset, color_asset)) {
      return DX_FAILURE;
    }
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_image_operations_color_fill* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_asset_image_operations_color_fill* asset = NULL;
  if (dx_asset_image_operations_color_fill_create(&asset)) {
    return DX_FAILURE;
  }
  // color
  if (dx_adl_semantical_read_color_instance_field(&asset->color, node, false, NAME(color_key), context)) {
    return DX_FAILURE;
  }
  *RETURN = DX_OBJECT(asset);
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_image_operations_color_fill_construct(dx_adl_type_handlers_image_operations_color_fill* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_image_operations_color_fill_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_image_operations_color_fill_destruct(dx_adl_type_handlers_image_operations_color_fill* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_image_operations_color_fill_dispatch_construct(dx_adl_type_handlers_image_operations_color_fill_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_image_operations_color_fill_create(dx_adl_type_handlers_image_operations_color_fill** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_image_operations_color_fill)
  if (dx_adl_type_handlers_image_operations_color_fill_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
