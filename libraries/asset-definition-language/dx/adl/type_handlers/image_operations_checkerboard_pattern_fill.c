#include "dx/adl/type_handlers/image_operations_checkerboard_pattern_fill.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"
#include "dx/adl/enter.h"
// strlen
#include <string.h>

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

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* node);

static Core_Result _parse_checker_colors(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, Core_Assets_ImageOperations_CheckerboardPatternFill* asset, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

#if 0
static Core_Result _enter(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* source, dx_adl_context* context);
#endif

Core_defineObjectType("dx.adl.type_handlers.image_operations.checkerboard_pattern_fill_reader",
                      dx_adl_type_handlers_image_operations_checkerboard_pattern_fill,
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

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF) {
  Core_InlineHashMapPP_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF) {
  Core_InlineHashMapPP_Configuration cfg = {
    .keyAddedCallback = &_on_expected_key_key_added,
    .keyRemovedCallback = &_on_expected_key_key_removed,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .hashKeyCallback = (Core_InlineHashMapPP_HashKeyCallback*)&_on_hash_expected_key_key,
    .compareKeysCallback = (Core_InlineHashMapPP_CompareKeysCallback*)&_on_compare_expected_key_keys,
  };
  if (Core_InlineHashMapPP_initialize(&SELF->expected_keys, &cfg)) {
    return Core_Failure;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    Core_String* expected_key = NULL; \
    if (Core_String_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      Core_InlineHashMapPP_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    if (Core_InlineHashMapPP_set(&SELF->expected_keys, expected_key, expected_key)) {\
      CORE_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      Core_InlineHashMapPP_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    CORE_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("numberOfCheckers");
  DEFINE("checkerSize");
  DEFINE("checkerColors");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  CORE_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  CORE_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* node) {
  Core_InlineArrayListP_Configuration configuration = {
    .addedCallback = &on_received_key_added,
    .removedCallback = &on_received_key_removed,
  };
  Core_InlineArrayListP received_keys;
  if (Core_InlineArrayListP_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (Core_InlineHashMapPP_getKeys(&node->map, &received_keys)) {
    Core_InlineArrayListP_uninitialize(&received_keys);
    return Core_Failure;
  }
  Core_Size number_of_received_keys = 0;
  if (Core_InlineArrayListP_getSize(&number_of_received_keys, &received_keys)) {
    Core_InlineArrayListP_uninitialize(&received_keys);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = number_of_received_keys; i < n; ++i) {
    Core_String* received_key = NULL;
    if (Core_InlineArrayListP_get(&received_key, &received_keys, i)) {
      Core_InlineArrayListP_uninitialize(&received_keys);
      return Core_Failure;
    }
    Core_String* expected_key = NULL;
    if (Core_InlineHashMapPP_get(&expected_key, &SELF->expected_keys, received_key)) {
      Core_InlineArrayListP_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  Core_InlineArrayListP_uninitialize(&received_keys);
  return Core_Success;
}

static Core_Result _parse_checker_colors(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, Core_Assets_ImageOperations_CheckerboardPatternFill* asset, dx_ddl_node* node, dx_adl_context* context) {
  // checkerColors
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, NAME(checker_colors_key))) {
    return Core_Failure;
  }
  // checkerColors.first
  if (dx_adl_semantical_read_color_instance_field(&asset->first_checker_color, child_node, false, NAME(first_key), context)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  // checkerColors.second
  if (dx_adl_semantical_read_color_instance_field(&asset->second_checker_color, child_node, false, NAME(second_key), context)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return Core_Success;
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  Core_Assets_ImageOperations_CheckerboardPatternFill* image_operation = NULL;
  if (Core_Assets_ImageOperations_CheckerboardPatternFill_create(&image_operation)) {
    return Core_Failure;
  }
  // numberOfCheckers
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(number_of_checkers_key))) {
      CORE_UNREFERENCE(image_operation);
      image_operation = NULL;
      return Core_Failure;
    }
    // numberOfCheckers.horizontal
    {
      Core_Size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(horizontal_key))) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
      if (Core_Assets_ImageOperations_CheckerboardPatternFill_setNumberOfCheckersHorizontal(image_operation, value)) {
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
    }
    // numberOfCheckers.vertical
    {
      Core_Size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(vertical_key))) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
      if (Core_Assets_ImageOperations_CheckerboardPatternFill_setNumberOfCheckersVertical(image_operation, value)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
  }
  // checkerSize
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(checker_size_key))) {
      CORE_UNREFERENCE(image_operation);
      image_operation = NULL;
      return Core_Failure;
    }
    // checkerSize.horizontal
    {
      Core_Size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(horizontal_key))) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
      if (Core_Assets_ImageOperations_CheckerboardPatternFill_setCheckerSizeHorizontal(image_operation, value)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
    }
    // checkerSize.vertical
    {
      Core_Size value;
      if (dx_adl_semantical_read_sz(&value, child_node, NAME(vertical_key))) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
      if (Core_Assets_ImageOperations_CheckerboardPatternFill_setCheckerSizeVertical(image_operation, value)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(image_operation);
        image_operation = NULL;
        return Core_Failure;
      }
    }
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
  }
  // checkerColors
  if (_parse_checker_colors(SELF, image_operation, node, context)) {
    CORE_UNREFERENCE(image_operation);
    image_operation = NULL;
    return Core_Failure;
  }
  *RETURN = CORE_OBJECT(image_operation);
  return Core_Success;
}

static Core_Result _resolve(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  Core_Assets_ImageOperations_CheckerboardPatternFill* asset = CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL(symbol->asset);
  // checkerColors
  {
    // checkerColors.first
    {
      dx_adl_symbol* color_symbol = NULL;
      if (dx_asset_definitions_get(&color_symbol, context->definitions, asset->first_checker_color->name)) {
        return Core_Failure;
      }
      Core_Assets_ColorRgbN8* color_asset = CORE_ASSETS_COLORRGBN8(color_symbol->asset);
      if (Core_Assets_ImageOperations_CheckerboardPatternFill_setFirstCheckerColor(asset, color_asset)) {
        CORE_UNREFERENCE(color_symbol);
        color_symbol = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(color_symbol);
      color_symbol = NULL;
    }
    // checkerColors.second
    {
      dx_adl_symbol* color_symbol = NULL;
      if (dx_asset_definitions_get(&color_symbol, context->definitions, asset->second_checker_color->name)) {
        return Core_Failure;
      }
      Core_Assets_ColorRgbN8* color_asset = CORE_ASSETS_COLORRGBN8(color_symbol->asset);
      if (Core_Assets_ImageOperations_CheckerboardPatternFill_setSecondCheckerColor(asset, color_asset)) {
        CORE_UNREFERENCE(color_symbol);
        color_symbol = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(color_symbol);
      color_symbol = NULL;
    }
  }
  //
  symbol->resolved = true;
  return Core_Success;
}

#if 0
static Core_Result _enter(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF, dx_ddl_node* source, dx_adl_context* context) {
  // type
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, source, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(material_type))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  // name
  Core_String* name = NULL;
  if (dx_asset_definition_language_parser_parse_name(&name, source, context)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  // enter
  dx_adl_symbol* symbol = NULL;
  if (dx_adl_symbol_create(&symbol, received_type, name)) {
    return Core_Failure;
  }
  if (source) {
    symbol->node = source;
    CORE_REFERENCE(symbol->node);
  }
  if (dx_asset_definitions_set(context->definitions, name, symbol)) {
    CORE_UNREFERENCE(symbol);
    symbol = NULL;
    if (Core_Error_Exists == Core_getError()) {
      /// TODO: Emit positions.
      /// TODO: Use dx_adl_diagnostics.
      dx_log("a definition of name `", sizeof("a definition of name `") - 1);
      dx_log(name->bytes, name->numberOfBytes);
      dx_log("` already exists", sizeof("` already exists") - 1);
      CORE_UNREFERENCE(name);
      name = NULL;
      CORE_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    } else {
      CORE_UNREFERENCE(name);
      name = NULL;
      CORE_UNREFERENCE(received_type);
      received_type = NULL;
      return Core_Failure;
    }
  }
  CORE_UNREFERENCE(symbol);
  symbol = NULL;
  CORE_UNREFERENCE(name);
  name = NULL;
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  return Core_Success;
}
#endif

Core_Result dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_destruct(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_constructDispatch(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_create(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_image_operations_checkerboard_pattern_fill);
  if (dx_adl_type_handlers_image_operations_checkerboard_pattern_fill_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
