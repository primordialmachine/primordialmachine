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

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static dx_result _on_hash_expected_key_key(dx_size* RETURN, void** a);

static dx_result _on_compare_expected_key_keys(dx_bool* RETURN, void** a, void** b);

static dx_result _initialize_expected_keys(dx_adl_type_handlers_material* SELF);

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_material* SELF);

static dx_result _check_keys(dx_adl_type_handlers_material* SELF, dx_ddl_node* node);

static dx_result _parse_material_controller(dx_assets_material_controller** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse_ambient_color(dx_assets_material* material, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse_material(dx_assets_material** RETURN, dx_ddl_node* node, dx_adl_context* context);

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context);

static dx_result _resolve_ambient_color(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result _resolve_ambient_texture(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result _resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.material",
                      dx_adl_type_handlers_material,
                      dx_adl_type_handler);

static void _on_expected_key_key_added(void** a) {
  DX_REFERENCE(*a);
}

static void _on_expected_key_key_removed(void** a) {
  DX_UNREFERENCE(*a);
}

static dx_result _on_hash_expected_key_key(dx_size* RETURN, void** a) {
  *RETURN = dx_string_get_hash_value(DX_STRING(*a));
  return DX_SUCCESS;
}

static dx_result _on_compare_expected_key_keys(dx_bool* RETURN, void** a, void** b) {
  *RETURN = dx_string_is_equal_to(DX_STRING(*a), DX_STRING(*b));
  return DX_SUCCESS;
}

static dx_result _uninitialize_expected_keys(dx_adl_type_handlers_material* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return DX_SUCCESS;
}

static dx_result _initialize_expected_keys(dx_adl_type_handlers_material* SELF) {
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION cfg = {
    .key_added_callback = &_on_expected_key_key_added,
    .key_removed_callback = &_on_expected_key_key_removed,
    .value_added_callback = NULL,
    .value_removed_callback = NULL,
    .hash_key_callback = &_on_hash_expected_key_key,
    .compare_keys_callback = &_on_compare_expected_key_keys,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->expected_keys, &cfg)) {
    return DX_FAILURE;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    dx_string* expected_key = NULL; \
    if (dx_string_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return DX_FAILURE; \
    } \
    if (dx_inline_pointer_hashmap_set(&SELF->expected_keys, expected_key, expected_key)) {\
      DX_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return DX_FAILURE; \
    } \
    DX_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("name");
  DEFINE("ambientColor");
  DEFINE("ambientTexture");
  DEFINE("controller");
#undef DEFINE
  return DX_SUCCESS;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static dx_result _check_keys(dx_adl_type_handlers_material* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  dx_inline_pointer_array received_keys;
  if (dx_inline_pointer_array_initialize(&received_keys, 0, &configuration)) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_get_keys(&node->map, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return DX_FAILURE;
  }
  dx_size number_of_received_keys = 0;
  if (dx_inline_pointer_array_get_size(&number_of_received_keys, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return DX_FAILURE;
  }
  for (dx_size i = 0, n = number_of_received_keys; i < n; ++i) {
    dx_string* received_key = NULL;
    if (dx_inline_pointer_array_get_at(&received_key, &received_keys, i)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return DX_FAILURE;
    }
    dx_string* expected_key = NULL;
    if (dx_inline_pointer_hashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return DX_FAILURE;
    }
  }
  dx_inline_pointer_array_uninitialize(&received_keys);
  return DX_SUCCESS;
}

static dx_result _parse_material_controller(dx_assets_material_controller** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
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

static dx_result _parse_ambient_color(dx_assets_material* material, dx_ddl_node* node, dx_adl_context* context) {
  if (dx_adl_semantical_read_color_instance_field(&material->ambient_color, node, true, NAME(ambient_color_key), context)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result _parse_material(dx_assets_material** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* name_value = NULL;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  dx_assets_material* material_value = NULL;
  if (dx_assets_material_create(&material_value, name_value)) {
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
        if (dx_assets_material_set_ambient_texture(material_value, texture_reference)) {
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
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(controller_key))) {
      if (DX_ERROR_NOT_FOUND != dx_get_error()) {
        DX_UNREFERENCE(material_value);
        material_value = NULL;
        return DX_FAILURE;
      } else {
        dx_set_error(DX_NO_ERROR);
      }
    } else {
      dx_assets_material_controller* controller_asset = NULL;
      if (_parse_material_controller(&controller_asset, child_node, context)) {
        DX_UNREFERENCE(child_node);
        child_node = NULL;
        DX_UNREFERENCE(material_value);
        material_value = NULL;
        return DX_FAILURE;
      }
      DX_UNREFERENCE(child_node);
      child_node = NULL;
      material_value->controller = controller_asset;
    }
  }
  *RETURN = material_value;
  return DX_SUCCESS;
}

static dx_result _parse(dx_object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (_check_keys(SELF, node)) {
    return DX_FAILURE;
  }
  return _parse_material((dx_assets_material**)RETURN, node, context);
}

static dx_result _resolve_ambient_color(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_assets_material* material = DX_ASSETS_MATERIAL(symbol->asset);
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
    if (dx_assets_color_rgb_n8_create((dx_assets_color_rgb_n8**)&material->ambient_color->object, &WHITE)) {
      DX_UNREFERENCE(material->ambient_color);
      material->ambient_color = NULL;
      return DX_FAILURE;
    }
  } else {
    dx_adl_symbol* color_symbol = NULL;
    if (dx_asset_definitions_get(&color_symbol, context->definitions, material->ambient_color->name)) {
      return DX_FAILURE;
    }
    dx_assets_color_rgb_n8* color_asset = DX_ASSETS_COLOR_RGB_N8(color_symbol->asset);
    if (dx_assets_material_set_ambient_color(material, color_asset)) {
      DX_UNREFERENCE(color_symbol);
      color_symbol = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(color_symbol);
    color_symbol = NULL;
  }
  return DX_SUCCESS;
}

static dx_result _resolve_ambient_texture(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_assets_material* material = DX_ASSETS_MATERIAL(symbol->asset);

  // if there is no ambient textur or the ambient texture was resolved already, return successfully.
  if (!material->ambient_texture_reference) {
    return DX_SUCCESS;
  }
  if (material->ambient_texture_reference->object) {
    return DX_SUCCESS;
  }
  // otherwise resolve
  dx_adl_symbol* referenced_symbol = NULL;
  if (dx_asset_definitions_get(&referenced_symbol, context->definitions, material->ambient_texture_reference->name)) {
    return DX_FAILURE;
  }
  material->ambient_texture_reference->object = referenced_symbol->asset;
  if (!material->ambient_texture_reference->object) {
    DX_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
    return DX_FAILURE;
  }
  DX_REFERENCE(material->ambient_texture_reference->object);
  DX_UNREFERENCE(referenced_symbol);
  referenced_symbol = NULL;
  return DX_SUCCESS;
}

static dx_result _resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_assets_material* material = DX_ASSETS_MATERIAL(symbol->asset);
  if (_resolve_ambient_texture(SELF, symbol, context)) {
    return DX_FAILURE;
  }
  if (_resolve_ambient_color(SELF, symbol, context)) {
    return DX_FAILURE;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_material_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  if (_initialize_expected_keys(SELF)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_material_destruct(dx_adl_type_handlers_material* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_material_dispatch_construct(dx_adl_type_handlers_material_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
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
