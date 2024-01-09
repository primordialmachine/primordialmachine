#include "dx/adl/type_handlers/material.h"

#include "dx/assets.h"
#include "dx/adl/enter.h"
#include "dx/adl/parser.h"

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, void** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, void** a, void** b);

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_material* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_material* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_material* SELF, dx_ddl_node* node);

static Core_Result _parse_material_controller(dx_assets_material_controller** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse_ambient_color(dx_assets_material* material, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse_material(dx_assets_material** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve_ambient_color(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static Core_Result _resolve_ambient_texture(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

Core_defineObjectType("dx.adl.type_handlers.material",
                      dx_adl_type_handlers_material,
                      dx_adl_type_handler);

static void _on_expected_key_key_added(void** a) {
  CORE_REFERENCE(*a);
}

static void _on_expected_key_key_removed(void** a) {
  CORE_UNREFERENCE(*a);
}

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_material* SELF) {
  Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_material* SELF) {
  Core_InlinePointerHashMap_Configuration cfg = {
    .keyAddedCallback = &_on_expected_key_key_added,
    .keyRemovedCallback = &_on_expected_key_key_removed,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .hashKeyCallback = (Core_InlinePointerHashmap_hash_key_callback*) &_on_hash_expected_key_key,
    .compareKeysCallback = (Core_InlinePointerHashmap_compare_keys_callback*)&_on_compare_expected_key_keys,
  };
  if (Core_InlinePointerHashmap_initialize(&SELF->expected_keys, &cfg)) {
    return Core_Failure;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    Core_String* expected_key = NULL; \
    if (Core_String_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    if (Core_InlinePointerHashmap_set(&SELF->expected_keys, expected_key, expected_key)) {\
      CORE_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    CORE_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("name");
  DEFINE("ambientColor");
  DEFINE("ambientTexture");
  DEFINE("controller");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  CORE_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  CORE_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_material* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  dx_inline_pointer_array received_keys;
  if (dx_inline_pointer_array_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (Core_InlinePointerHashmap_getKeys(&node->map, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return Core_Failure;
  }
  Core_Size number_of_received_keys = 0;
  if (dx_inline_pointer_array_get_size(&number_of_received_keys, &received_keys)) {
    dx_inline_pointer_array_uninitialize(&received_keys);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = number_of_received_keys; i < n; ++i) {
    Core_String* received_key = NULL;
    if (dx_inline_pointer_array_get_at(&received_key, &received_keys, i)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return Core_Failure;
    }
    Core_String* expected_key = NULL;
    if (Core_InlinePointerHashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  dx_inline_pointer_array_uninitialize(&received_keys);
  return Core_Success;
}

static Core_Result _parse_material_controller(dx_assets_material_controller** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(material_controllers_ambient_color_type))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (isEqualTo) {
    dx_adl_type_handler* type_handler = NULL;
    if (Core_InlinePointerHashmap_get(&type_handler, &context->type_handlers, received_type)) {
      CORE_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return dx_adl_type_handler_read((Core_Object**)RETURN, type_handler, node, context);
  } else {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
}

static Core_Result _parse_ambient_color(dx_assets_material* material, dx_ddl_node* node, dx_adl_context* context) {
  if (dx_adl_semantical_read_color_instance_field(&material->ambient_color, node, true, NAME(ambient_color_key), context)) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result _parse_material(dx_assets_material** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* name_value = NULL;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return Core_Failure;
    }
  }
  dx_assets_material* material_value = NULL;
  if (dx_assets_material_create(&material_value, name_value)) {
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(name_value);
  name_value = NULL;
  // ambientColor?
  {
    if (_parse_ambient_color(material_value, node, context)) {
      CORE_UNREFERENCE(material_value);
      material_value = NULL;
      return Core_Failure;
    }
  }
  // ambientTexture?
  {
    dx_asset_reference* texture_reference = NULL;
    if (dx_asset_definition_language_parser_parse_texture_instance_field(&texture_reference, node, true, NAME(ambient_texture_key), context)) {
      CORE_UNREFERENCE(material_value);
      material_value = NULL;
      return Core_Failure;
    } else {
      if (NULL != texture_reference) {
        if (dx_assets_material_set_ambient_texture(material_value, texture_reference)) {
          CORE_UNREFERENCE(texture_reference);
          texture_reference = NULL;
          CORE_UNREFERENCE(material_value);
          material_value = NULL;
          return Core_Failure;
        }
        CORE_UNREFERENCE(texture_reference);
        texture_reference = NULL;
      }
    }
  }
  // controller?
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(controller_key))) {
      if (Core_Error_NotFound != Core_getError()) {
        CORE_UNREFERENCE(material_value);
        material_value = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
    } else {
      dx_assets_material_controller* controller_asset = NULL;
      if (_parse_material_controller(&controller_asset, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(material_value);
        material_value = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      material_value->controller = controller_asset;
    }
  }
  *RETURN = material_value;
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  return _parse_material((dx_assets_material**)RETURN, node, context);
}

static Core_Result _resolve_ambient_color(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_assets_material* material = DX_ASSETS_MATERIAL(symbol->asset);
  // default to opaque white if no color is specified
  if (!material->ambient_color) {
    DX_RGB_N8 WHITE = { .r = 255, .g = 255, .b = 255 };
    Core_String* name = NULL;
    if (Core_String_create(&name, "<anonymous>", sizeof("<anonymous>") - 1)) {
      return Core_Failure;
    }
    if (dx_asset_reference_create(&material->ambient_color, name)) {
      CORE_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(name);
    name = NULL;
    if (dx_assets_color_rgb_n8_create((dx_assets_color_rgb_n8**)&material->ambient_color->object, &WHITE)) {
      CORE_UNREFERENCE(material->ambient_color);
      material->ambient_color = NULL;
      return Core_Failure;
    }
  } else {
    dx_adl_symbol* color_symbol = NULL;
    if (dx_asset_definitions_get(&color_symbol, context->definitions, material->ambient_color->name)) {
      return Core_Failure;
    }
    dx_assets_color_rgb_n8* color_asset = DX_ASSETS_COLOR_RGB_N8(color_symbol->asset);
    if (dx_assets_material_set_ambient_color(material, color_asset)) {
      CORE_UNREFERENCE(color_symbol);
      color_symbol = NULL;
      return Core_Failure;
    }
    CORE_UNREFERENCE(color_symbol);
    color_symbol = NULL;
  }
  return Core_Success;
}

static Core_Result _resolve_ambient_texture(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_assets_material* material = DX_ASSETS_MATERIAL(symbol->asset);

  // if there is no ambient textur or the ambient texture was resolved already, return successfully.
  if (!material->ambient_texture_reference) {
    return Core_Success;
  }
  if (material->ambient_texture_reference->object) {
    return Core_Success;
  }
  // otherwise resolve
  dx_adl_symbol* referenced_symbol = NULL;
  if (dx_asset_definitions_get(&referenced_symbol, context->definitions, material->ambient_texture_reference->name)) {
    return Core_Failure;
  }
  material->ambient_texture_reference->object = referenced_symbol->asset;
  if (!material->ambient_texture_reference->object) {
    CORE_UNREFERENCE(referenced_symbol);
    referenced_symbol = NULL;
    return Core_Failure;
  }
  CORE_REFERENCE(material->ambient_texture_reference->object);
  CORE_UNREFERENCE(referenced_symbol);
  referenced_symbol = NULL;
  return Core_Success;
}

static Core_Result _resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  dx_assets_material* material = DX_ASSETS_MATERIAL(symbol->asset);
  if (_resolve_ambient_texture(SELF, symbol, context)) {
    return Core_Failure;
  }
  if (_resolve_ambient_color(SELF, symbol, context)) {
    return Core_Failure;
  }
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_material);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_material_destruct(dx_adl_type_handlers_material* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_material_constructDispatch(dx_adl_type_handlers_material_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_material_create(dx_adl_type_handlers_material** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_material);
  if (dx_adl_type_handlers_material_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
