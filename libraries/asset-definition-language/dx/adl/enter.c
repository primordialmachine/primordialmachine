#include "dx/adl/enter.h"

#include "dx/adl/parser.h"
#include "dx/assets.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

#define CASEOF(_NAME, _DISPATCH) \
  if (dx_string_is_equal_to(received_type, NAME(_NAME))) { \
    if (dx_asset_definition_language_enter_on_##_DISPATCH(SELF, source, context)) { \
      DX_UNREFERENCE(received_type); \
      received_type = NULL; \
      return 1; \
    } \
  } else { \
    if (dx_get_error()) { \
      DX_UNREFERENCE(received_type); \
      received_type = NULL; \
      return 1; \
    } \
  }

DX_DEFINE_OBJECT_TYPE("dx.dx_asset_definition_language.enter",
                      dx_asset_definition_language_enter,
                      dx_object);

static dx_result add_symbol(dx_asset_definition_language_enter* SELF, dx_string* type, dx_string* name, dx_ddl_node* node) {
  dx_adl_symbol* symbol = NULL;
  if (dx_adl_symbol_create(&symbol, type, name)) {
    return DX_FAILURE;
  }
  if (node) {
    symbol->node = node;
    DX_REFERENCE(symbol->node);
  }
  if (dx_asset_definitions_set(SELF->context->definitions, name, symbol)) {
    DX_UNREFERENCE(symbol);
    symbol = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(symbol);
  symbol = NULL;
  return DX_SUCCESS;
}

static void dx_asset_definition_language_enter_destruct(dx_asset_definition_language_enter* SELF) {
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
}

static void dx_asset_definition_language_enter_dispatch_construct(dx_asset_definition_language_enter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_definition_language_enter_construct(dx_asset_definition_language_enter* SELF, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context) {
  dx_rti_type* TYPE = dx_asset_definition_language_enter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  
  SELF->context = context;

  if (!diagnostics) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->diagnostics = diagnostics;
  DX_REFERENCE(SELF->diagnostics);

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_create(dx_asset_definition_language_enter** RETURN, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context) {
  DX_CREATE_PREFIX(dx_asset_definition_language_enter)
  if (dx_asset_definition_language_enter_construct(SELF, diagnostics, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static bool is_of_type(dx_ddl_node* source, dx_string* expected_type, dx_adl_context* context) {
  if (dx_get_error()) {
    return false;
  }
  if (!source || dx_ddl_node_kind_map != source->kind) {
    return false;
  }
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return false;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return false;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  return true;
}

dx_result dx_asset_definition_language_enter_run(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  return dx_asset_definition_language_enter_on_scene(SELF, source, context);
}

dx_result dx_asset_definition_language_enter_on_scene_element(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    if (DX_ERROR_NOT_FOUND == dx_get_error()) {
      dx_log("the definition is issing the type field", sizeof("the definition is missing the type field") - 1);
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    }
    return DX_FAILURE;
  }
  //
  CASEOF(color_type, color);
  CASEOF(image_type, image);
  CASEOF(material_type, material);
  CASEOF(mesh_instance_type, mesh_instance);
  CASEOF(mesh_type, mesh);
  CASEOF(texture_type, texture);
  CASEOF(viewer_type, viewer);
  CASEOF(viewer_instance_type, viewer_instance)
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_color(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(color_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // name
  dx_string* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_scene(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(scene_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // elements?
  dx_ddl_node* child_source = NULL;
  if (dx_ddl_node_map_get(&child_source, source, NAME(elements_key))) {
    if (dx_get_error() != DX_ERROR_NOT_FOUND) {
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
    }
  }
  if (child_source) {
    dx_size n;
    if (dx_ddl_node_list_get_size(&n, child_source)) {
      DX_UNREFERENCE(child_source);
      child_source = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
    for (dx_size i = 0; i < n; ++i) {
      dx_ddl_node* temporary;
      if (dx_ddl_node_list_get(&temporary, child_source, i)) {
        DX_UNREFERENCE(child_source);
        child_source = NULL;
        DX_UNREFERENCE(received_type);
        received_type = NULL;
        return DX_FAILURE;
      }
      if (dx_asset_definition_language_enter_on_scene_element(SELF, temporary, context)) {
        DX_UNREFERENCE(temporary);
        temporary = NULL;
        DX_UNREFERENCE(child_source);
        child_source = NULL;
        DX_UNREFERENCE(received_type);
        received_type = NULL;
        return DX_FAILURE;
      }
      DX_UNREFERENCE(temporary);
      temporary = NULL;
    }
  }
  //
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_image(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(image_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // name
  dx_string* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_mesh(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(mesh_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // name
  dx_string* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_mesh_instance(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(mesh_instance_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // generated name
  dx_string* name = dx_adl_names_create_unique_name(context->names);
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_material(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(material_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // name
  dx_string* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_viewer_instance(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(viewer_instance_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // generated name
  dx_string* name = dx_adl_names_create_unique_name(context->names);
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_viewer(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(viewer_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // name
  dx_string* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_enter_on_texture(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(texture_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  // name
  dx_string* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (DX_ERROR_EXISTS == dx_get_error()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->number_of_bytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
    } else {
      DX_UNREFERENCE(name);
      name = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return DX_FAILURE;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return DX_SUCCESS;
}
