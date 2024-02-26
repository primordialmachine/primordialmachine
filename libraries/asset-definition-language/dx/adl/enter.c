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
      CORE_UNREFERENCE(received_type); \
      received_type = NULL; \
      return Core_Failure; \
    } \
    if (isEqualTo) { \
      dx_adl_type_handler* type_handler = NULL; \
      if (Core_InlineHashMapPP_get(&type_handler, &SELF->context->type_handlers, received_type)) { \
        return Core_Failure; \
      } \
      if (dx_adl_type_handler_enter(type_handler, source, context)) { \
        CORE_UNREFERENCE(received_type); \
        received_type = NULL; \
        return Core_Failure; \
      } \
    } else { \
      if (Core_getError()) { \
        CORE_UNREFERENCE(received_type); \
        received_type = NULL; \
        return Core_Failure; \
      } \
    } \
  }

Core_defineObjectType("dx.dx_asset_definition_language.enter",
                      dx_asset_definition_language_enter,
                      Core_Object);

static Core_Result add_symbol(dx_asset_definition_language_enter* SELF, Core_String* type, Core_String* name, dx_ddl_node* node) {
  dx_adl_symbol* symbol = NULL;
  if (dx_adl_symbol_create(&symbol, type, name)) {
    return Core_Failure;
  }
  if (node) {
    symbol->node = node;
    CORE_REFERENCE(symbol->node);
  }
  if (dx_asset_definitions_set(SELF->context->definitions, name, symbol)) {
    CORE_UNREFERENCE(symbol);
    symbol = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(symbol);
  symbol = NULL;
  return Core_Success;
}

static void dx_asset_definition_language_enter_destruct(dx_asset_definition_language_enter* SELF) {
  CORE_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
}

static void dx_asset_definition_language_enter_constructDispatch(dx_asset_definition_language_enter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_definition_language_enter_construct(dx_asset_definition_language_enter* SELF, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context) {
  DX_CONSTRUCT_PREFIX(dx_asset_definition_language_enter);
  SELF->context = context;
  if (!diagnostics) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->diagnostics = diagnostics;
  CORE_REFERENCE(SELF->diagnostics);

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_definition_language_enter_create(dx_asset_definition_language_enter** RETURN, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context) {
  DX_CREATE_PREFIX(dx_asset_definition_language_enter);
  if (dx_asset_definition_language_enter_construct(SELF, diagnostics, context)) {
    CORE_UNREFERENCE(SELF);
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
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return false;
  }
  if (!isEqualTo) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return false;
  }
  CORE_UNREFERENCE(received_type);
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
      dx_log("the definition is missing the type field", sizeof("the definition is missing the type field") - 1);
      Core_setError(Core_Error_SemanticalAnalysisFailed);
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
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
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
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_LexicalAnalysisFailed);
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
      CORE_UNREFERENCE(child_source);
      child_source = NULL;
      CORE_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    }
    for (Core_Size i = 0; i < n; ++i) {
      dx_ddl_node* temporary;
      if (dx_ddl_node_list_get(&temporary, child_source, i)) {
        CORE_UNREFERENCE(child_source);
        child_source = NULL;
        CORE_UNREFERENCE(received_type);
        received_type = NULL;
        return Core_Failure;
      }
      if (dx_asset_definition_language_enter_on_scene_element(SELF, temporary, context)) {
        CORE_UNREFERENCE(temporary);
        temporary = NULL;
        CORE_UNREFERENCE(child_source);
        child_source = NULL;
        CORE_UNREFERENCE(received_type);
        received_type = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(temporary);
      temporary = NULL;
    }
  }
  //
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  //
  return Core_Success;
}
