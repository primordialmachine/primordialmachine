#include "dx/adl/type_handlers/material.h"

#include "dx/assets.h"
#include "dx/adl/enter.h"
#include "dx/adl/parser.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

static dx_result _parse_material_controller(dx_asset_material_controller** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse_ambient_color(dx_asset_material* material, dx_ddl_node* node, dx_adl_context* context);

static dx_result _read_material(dx_asset_material** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.material",
                      dx_adl_type_handlers_material,
                      dx_adl_type_handler);

static dx_result _parse_material_controller(dx_asset_material_controller** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return DX_FAILURE;
  }
  if (dx_string_is_equal_to(received_type, NAME(material_controllers_ambient_color_type))) {
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

static dx_result _parse_ambient_color(dx_asset_material* material, dx_ddl_node* node, dx_adl_context* context) {
  if (dx_adl_semantical_read_color_instance_field(&material->ambient_color, node, true, NAME(ambient_color_key), context)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result _read_material(dx_asset_material** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_material* material_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    if (dx_adl_semantical_read_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  if (dx_asset_material_create(&material_value, name_value)) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  // ambientColor?
  {
    if (_parse_ambient_color(material_value, node, context)) {
      DX_UNREFERENCE(material_value);
      material_value = NULL;
      return DX_FAILURE;
    }
  }
  // ambientTexture?
  {
    dx_asset_reference* texture_reference = NULL;
    if (dx_asset_definition_language_parser_parse_texture_instance_field(&texture_reference, node, true, NAME(ambient_texture_key), context)) {
      DX_UNREFERENCE(material_value);
      material_value = NULL;
      return DX_FAILURE;
    } else {
      if (NULL != texture_reference) {
        if (dx_asset_material_set_ambient_texture(material_value, texture_reference)) {
          DX_UNREFERENCE(texture_reference);
          texture_reference = NULL;
          DX_UNREFERENCE(material_value);
          material_value = NULL;
          return DX_FAILURE;
        }
        DX_UNREFERENCE(texture_reference);
        texture_reference = NULL;
      }
    }
  }
  // controller?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(controller_key));
    if (!child_node) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(material_value);
        material_value = NULL;
        return DX_FAILURE;
      } else {
        dx_set_error(DX_NO_ERROR);
      }
    } else {
      dx_asset_material_controller* controller_value = NULL;
      if (_parse_material_controller(&controller_value, child_node, context)) {
        DX_UNREFERENCE(material_value);
        material_value = NULL;
        return DX_FAILURE;
      }
      material_value->controller = controller_value;
    }
  }
  *RETURN = material_value;
  return DX_SUCCESS;
}

static dx_result ensure_ambient_color_resolved(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_asset_material* material = DX_ASSET_MATERIAL(symbol->asset);
  // default to opaque white if no color is specified
  if (!material->ambient_color) {
    DX_RGB_N8 WHITE = { .r = 255, .g = 255, .b = 255 };
    dx_string* name = NULL;
    if (dx_string_create(&name, "<anonymous>", sizeof("<anonymous>") - 1)) {
      return DX_FAILURE;
    }
    if (dx_asset_reference_create(&material->ambient_color, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(name);
    name = NULL;
    if (dx_assets_color_rgb_n8_create((dx_assets_color_rgb_n8**) & material->ambient_color->object, &WHITE)) {
      DX_UNREFERENCE(material->ambient_color);
      material->ambient_color = NULL;
      return DX_FAILURE;
    }
  } else {
    dx_adl_symbol* color_symbol = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, material->ambient_color->name));
    ///@todo Handle symbol not found situation.
    dx_assets_color_rgb_n8* color_asset = DX_ASSETS_COLOR_RGB_N8(color_symbol->asset);
    if (dx_asset_material_set_ambient_color(material, color_asset)) {
      return DX_FAILURE;
    }
  }
  return DX_SUCCESS;
}

static dx_result ensure_ambient_texture_resolved(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_asset_material* material = DX_ASSET_MATERIAL(symbol->asset);

  // if there is no ambient textur or the ambient texture was resolved already, return successfully.
  if (!material->ambient_texture_reference) {
    return DX_SUCCESS;
  }
  if (material->ambient_texture_reference->object) {
    return DX_SUCCESS;
  }
  // otherwise resolve
  dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, material->ambient_texture_reference->name);
  if (!referenced_symbol) {
    return DX_FAILURE;
  }
  material->ambient_texture_reference->object = referenced_symbol->asset;
  if (!material->ambient_texture_reference->object) {
    return DX_FAILURE;
  }
  DX_REFERENCE(material->ambient_texture_reference->object);
  return DX_SUCCESS;
}

static dx_result resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_material* material = DX_ASSET_MATERIAL(symbol->asset);
  if (ensure_ambient_texture_resolved(SELF, symbol, context)) {
    return DX_FAILURE;
  }
  if (ensure_ambient_color_resolved(SELF, symbol, context)) {
    return DX_FAILURE;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context) {
  return _read_material((dx_asset_material**)RETURN, node, context);
}

dx_result dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_material_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_material_destruct(dx_adl_type_handlers_material* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_material_dispatch_construct(dx_adl_type_handlers_material_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_material_create(dx_adl_type_handlers_material** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_material)
  if (dx_adl_type_handlers_material_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
