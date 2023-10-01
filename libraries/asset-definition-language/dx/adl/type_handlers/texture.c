#include "dx/adl/type_handlers/texture.h"

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

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result _read_image(dx_asset_image** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _read_texture(dx_asset_texture** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result resolve(dx_adl_type_handlers_texture* SELF,
                         dx_adl_symbol* symbol,
                         dx_adl_context* context);

static dx_result read(dx_object** RETURN,
                      dx_adl_type_handlers_texture* SELF,
                      dx_ddl_node* node,
                      dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.texture",
                      dx_adl_type_handlers_texture,
                      dx_adl_type_handler);

static dx_result _read_image(dx_asset_image** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return DX_FAILURE;
  }
  if (dx_string_is_equal_to(received_type, NAME(image_type))) {
    dx_adl_type_handler* type_handler = NULL;
    if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, received_type)) {
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return dx_adl_type_handler_read((dx_object**)RETURN, type_handler, node, context);
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
}

static dx_result _read_texture(dx_asset_texture** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  // name
  dx_string* name_value = NULL;
  {
    if (dx_adl_semantical_read_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  // image
  dx_asset_reference* image_reference_value = NULL;
  {
    if (dx_asset_definition_language_parser_parse_image_instance_field(&image_reference_value, node, false, NAME(image_key), context)) {
      DX_UNREFERENCE(name_value);
      name_value = NULL;
      return DX_FAILURE;
    }
  }
  //
  dx_asset_texture* texture_value = NULL;
  if (dx_asset_texture_create(&texture_value, name_value, image_reference_value)) {
    DX_UNREFERENCE(image_reference_value);
    image_reference_value = NULL;
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(image_reference_value);
  image_reference_value = NULL;
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  *RETURN = texture_value;
  return DX_SUCCESS;
}

static dx_result resolve(dx_adl_type_handlers_texture* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_texture* texture = DX_ASSET_TEXTURE(symbol->asset);
  if (texture->image_reference->object) {
    symbol->resolved = true;
    return DX_SUCCESS;
  }
  dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, texture->image_reference->name);
  if (!referenced_symbol) {
    return DX_FAILURE;
  }
  texture->image_reference->object = referenced_symbol->asset;
  if (!texture->image_reference->object) {
    return DX_FAILURE;
  }
  DX_REFERENCE(texture->image_reference->object);
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_texture* SELF, dx_ddl_node* node, dx_adl_context* context) {
  return _read_texture((dx_asset_texture**)RETURN, node, context);
}

dx_result dx_adl_type_handlers_texture_construct(dx_adl_type_handlers_texture* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_texture_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_texture_destruct(dx_adl_type_handlers_texture* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_texture_dispatch_construct(dx_adl_type_handlers_texture_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_texture_create(dx_adl_type_handlers_texture** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_texture)
  if (dx_adl_type_handlers_texture_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
