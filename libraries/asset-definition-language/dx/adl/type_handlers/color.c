#include "dx/adl/type_handlers/color.h"

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

static dx_result _parse_color_rgb_u8(dx_ddl_node* node, dx_adl_context* context, DX_RGB_N8* target);

static dx_result resolve(dx_adl_type_handlers_color* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_color* SELF, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.color",
                      dx_adl_type_handlers_color,
                      dx_adl_type_handler);

static dx_result _parse_color_rgb_u8(dx_ddl_node* node, dx_adl_context* context, DX_RGB_N8* target) {
  dx_n8 r, g, b;
  if (dx_adl_semantical_read_n8(&r, node, NAME(red_key))) {
    return DX_FAILURE;
  }
  if (dx_adl_semantical_read_n8(&g, node, NAME(green_key))) {
    return DX_FAILURE;
  }
  if (dx_adl_semantical_read_n8(&b, node, NAME(blue_key))) {
    return DX_FAILURE;
  }
  // TODO: Check bounds.
  target->r = r;
  target->g = g;
  target->b = b;
  return DX_SUCCESS;
}

static dx_result resolve(dx_adl_type_handlers_color* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_color* self, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_RGB_N8 temporary;
  if (_parse_color_rgb_u8(node, context, &temporary)) {
    return DX_FAILURE;
  }
  dx_assets_color_rgb_n8* color = NULL;
  if (dx_assets_color_rgb_n8_create(&color, &temporary)) {
    return DX_FAILURE;
  }
  *RETURN  = DX_OBJECT(color);
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_color_construct(dx_adl_type_handlers_color* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_color_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_color_destruct(dx_adl_type_handlers_color* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_color_dispatch_construct(dx_adl_type_handlers_color_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_color_create(dx_adl_type_handlers_color** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_color)
  if (dx_adl_type_handlers_color_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
