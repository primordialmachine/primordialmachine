#include "dx/adl/type_handlers/image_operations_color_fill.h"

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

static int resolve(dx_adl_type_handlers_image_operations_color_fill* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_image_operations_color_fill* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.image_operations.color_fill_reader",
                      dx_adl_type_handlers_image_operations_color_fill,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_image_operations_color_fill* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_image_operations_color_fill* asset = DX_ASSET_IMAGE_OPERATIONS_COLOR_FILL(symbol->asset);
  dx_ddl_node* node = symbol->node;
  // color
  {
    dx_asset_reference* color_reference = dx_adl_semantical_read_color_instance_field(node, false, NAME(color_key), context);
    if (!color_reference) {
      return DX_FAILURE;
    }
    dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, color_reference->name));
    dx_asset_color_rgb_n8* color_asset = DX_ASSET_COLOR_RGB_N8(color_symbol->asset);
    DX_UNREFERENCE(color_reference);
    color_reference = NULL;
    //
    if (dx_asset_image_operations_color_fill_set_color(asset, color_asset)) {
      return DX_FAILURE;
    }
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_object* read(dx_adl_type_handlers_image_operations_color_fill* self, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return NULL;
  }
  dx_asset_image_operations_color_fill* image_operation = dx_asset_image_operations_color_fill_create();
  if (!image_operation) {
    return NULL;
  }
  return DX_OBJECT(image_operation);
}

int dx_adl_type_handlers_image_operations_color_fill_construct(dx_adl_type_handlers_image_operations_color_fill* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_image_operations_color_fill_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_ADL_TYPE_HANDLER(SELF)->resolve = (int(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
  DX_ADL_TYPE_HANDLER(SELF)->read = (dx_object * (*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_image_operations_color_fill_destruct(dx_adl_type_handlers_image_operations_color_fill* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_image_operations_color_fill_dispatch_construct(dx_adl_type_handlers_image_operations_color_fill_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_image_operations_color_fill* dx_adl_type_handlers_image_operations_color_fill_create() {
  dx_adl_type_handlers_image_operations_color_fill* self = DX_ADL_TYPE_HANDLERS_IMAGE_OPERATIONS_COLOR_FILL(dx_object_alloc(sizeof(dx_adl_type_handlers_image_operations_color_fill)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_image_operations_color_fill_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
