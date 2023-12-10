#include "dx/adl/enter.h"

#include "dx/adl/parser.h"
#include "dx/assets.h"

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

#define CASEOF(_NAME, _DISPATCH) \
  { \
    Core_Boolean isEqualTo = Core_False; \
    if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(_NAME))) { \
      DX_UNREFERENCE(received_type); \
      received_type = NULL; \
      return Core_Failure; \
    } \
    if (isEqualTo) { \
      if (dx_asset_definition_language_enter_on_##_DISPATCH(SELF, source, context)) { \
        DX_UNREFERENCE(received_type); \
        received_type = NULL; \
        return Core_Failure; \
      } \
    } else { \
      if (Core_getError()) { \
        DX_UNREFERENCE(received_type); \
        received_type = NULL; \
        return Core_Failure; \
      } \
    } \
  }

DX_DEFINE_OBJECT_TYPE("dx.dx_asset_definition_language.enter",
                      dx_asset_definition_language_enter,
                      Core_Object);

static Core_Result add_symbol(dx_asset_definition_language_enter* SELF, Core_String* type, Core_String* name, dx_ddl_node* node) {
  dx_adl_symbol* symbol = NULL;
  if (dx_adl_symbol_create(&symbol, type, name)) {
    return Core_Failure;
  }
  if (node) {
    symbol->node = node;
    DX_REFERENCE(symbol->node);
  }
  if (dx_asset_definitions_set(SELF->context->definitions, name, symbol)) {
    DX_UNREFERENCE(symbol);
    symbol = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(symbol);
  symbol = NULL;
  return Core_Success;
}

static void dx_asset_definition_language_enter_destruct(dx_asset_definition_language_enter* SELF) {
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
}

static void dx_asset_definition_language_enter_constructDispatch(dx_asset_definition_language_enter_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_definition_language_enter_construct(dx_asset_definition_language_enter* SELF, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context) {
  DX_CONSTRUCT_PREFIX(dx_asset_definition_language_enter);
  SELF->context = context;
  if (!diagnostics) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->diagnostics = diagnostics;
  DX_REFERENCE(SELF->diagnostics);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_create(dx_asset_definition_language_enter** RETURN, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context) {
  DX_CREATE_PREFIX(dx_asset_definition_language_enter);
  if (dx_asset_definition_language_enter_construct(SELF, diagnostics, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static bool is_of_type(dx_ddl_node* source, Core_String* expected_type, dx_adl_context* context) {
  if (Core_getError()) {
    return false;
  }
  if (!source || dx_ddl_node_kind_map != source->kind) {
    return false;
  }
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return false;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return false;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return false;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  return true;
}

Core_Result dx_asset_definition_language_enter_run(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  return dx_asset_definition_language_enter_on_scene(SELF, source, context);
}

Core_Result dx_asset_definition_language_enter_on_scene_element(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    if (Core_Error_NotFound == Core_getError()) {
      dx_log("the definition is issing the type field", sizeof("the definition is missing the type field") - 1);
      Core_setError(Core_Error_SemanticalError);
    }
    return Core_Failure;
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
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_color(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(color_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_scene(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(scene_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_LexicalError);
    return Core_Failure;
  }
  // elements?
  dx_ddl_node* child_source = NULL;
  if (dx_ddl_node_map_get(&child_source, source, NAME(elements_key))) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
    }
  }
  if (child_source) {
    Core_Size n;
    if (dx_ddl_node_list_get_size(&n, child_source)) {
      DX_UNREFERENCE(child_source);
      child_source = NULL;
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    }
    for (Core_Size i = 0; i < n; ++i) {
      dx_ddl_node* temporary;
      if (dx_ddl_node_list_get(&temporary, child_source, i)) {
        DX_UNREFERENCE(child_source);
        child_source = NULL;
        DX_UNREFERENCE(received_type);
        received_type = NULL;
        return Core_Failure;
      }
      if (dx_asset_definition_language_enter_on_scene_element(SELF, temporary, context)) {
        DX_UNREFERENCE(temporary);
        temporary = NULL;
        DX_UNREFERENCE(child_source);
        child_source = NULL;
        DX_UNREFERENCE(received_type);
        received_type = NULL;
        return Core_Failure;
      }
      DX_UNREFERENCE(temporary);
      temporary = NULL;
    }
  }
  //
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_image(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(image_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_mesh(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(mesh_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_mesh_instance(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(mesh_instance_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // generated name
  Core_String* name = dx_adl_names_create_unique_name(context->names);
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_material(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(material_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_viewer_instance(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(viewer_instance_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // generated name
  Core_String* name = dx_adl_names_create_unique_name(context->names);
  // enter
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_viewer(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(viewer_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_on_texture(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(texture_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalError);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  //
  if (add_symbol(SELF, received_type, name, source)) {
    if (Core_Error_Exists == Core_getError()) {
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
      return Core_Failure;
    }
  }
  //
  DX_UNREFERENCE(name);
  name = NULL;
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}
