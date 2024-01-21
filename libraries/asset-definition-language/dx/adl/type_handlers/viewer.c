#include "dx/adl/type_handlers/viewer.h"

#include "dx/assets.h"
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

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_viewer* SELF);

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_viewer* SELF);

static Core_Result _check_keys(dx_adl_type_handlers_viewer* SELF, dx_ddl_node* node);

static Core_Result _parse_optics(dx_assets_optics** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse_viewer_controller(dx_assets_viewer_controller** RETURN, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_viewer* SELF, dx_ddl_node* node, dx_adl_context* context);

static Core_Result _resolve(dx_adl_type_handlers_viewer* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

Core_defineObjectType("dx.adl.type_handlers.viewer",
                      dx_adl_type_handlers_viewer,
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

static Core_Result _uninitialize_expected_keys(dx_adl_type_handlers_viewer* SELF) {
  Core_InlinePointerHashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_adl_type_handlers_viewer* SELF) {
  Core_InlinePointerHashMap_Configuration cfg = {
    .keyAddedCallback = &_on_expected_key_key_added,
    .keyRemovedCallback = &_on_expected_key_key_removed,
    .valueAddedCallback = NULL,
    .valueRemovedCallback = NULL,
    .hashKeyCallback = (Core_InlinePointerHashmap_HashKeyCallback*)&_on_hash_expected_key_key,
    .compareKeysCallback = (Core_InlinePointerHashmap_CompareKeysCallback*)&_on_compare_expected_key_keys,
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
  DEFINE("source");
  DEFINE("target");
  DEFINE("up");
  DEFINE("optics");
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

static Core_Result _check_keys(dx_adl_type_handlers_viewer* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  Core_InlinePointerArray received_keys;
  if (Core_InlinePointerArray_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (Core_InlinePointerHashmap_getKeys(&node->map, &received_keys)) {
    Core_InlinePointerArray_uninitialize(&received_keys);
    return Core_Failure;
  }
  Core_Size number_of_received_keys = 0;
  if (Core_InlinePointerArray_getSize(&number_of_received_keys, &received_keys)) {
    Core_InlinePointerArray_uninitialize(&received_keys);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = number_of_received_keys; i < n; ++i) {
    Core_String* received_key = NULL;
    if (Core_InlinePointerArray_get_at(&received_key, &received_keys, i)) {
      Core_InlinePointerArray_uninitialize(&received_keys);
      return Core_Failure;
    }
    Core_String* expected_key = NULL;
    if (Core_InlinePointerHashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      Core_InlinePointerArray_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  Core_InlinePointerArray_uninitialize(&received_keys);
  return Core_Success;
}

static Core_Result _parse_optics(dx_assets_optics** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo[2] = { Core_False, Core_False };
  if (Core_String_isEqualTo(&isEqualTo[0], received_type, NAME(optics_orthographic_type)) ||
      Core_String_isEqualTo(&isEqualTo[1], received_type, NAME(optics_perspective_type))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo[0] && !isEqualTo[1]) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  dx_adl_type_handler* type_handler = NULL;
  if (Core_InlinePointerHashmap_get(&type_handler, &context->type_handlers, received_type)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  dx_assets_optics* asset_optics = NULL;
  if (dx_adl_type_handler_read((Core_Object**)&asset_optics, type_handler, node, context)) {
    return Core_Failure;
  }
  *RETURN = asset_optics;
  return Core_Success;
}

static Core_Result _parse_viewer_controller(dx_assets_viewer_controller** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(viewer_controllers_rotate_y_type))) {
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
    dx_assets_viewer_controller* asset_viewer_controller = NULL;
    if (dx_adl_type_handler_read((Core_Object**)&asset_viewer_controller, type_handler, node, context)) {
      return Core_Failure;
    }
    *RETURN = asset_viewer_controller;
    return Core_Success;
  } else {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
}

static Core_Result _parse(Core_Object** RETURN, dx_adl_type_handlers_viewer* SELF, dx_ddl_node* node, dx_adl_context* context) {
  if (!node) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  dx_assets_viewer* viewer_value = NULL;
  Core_String* name_value = NULL;
  // name
  {
    if (dx_asset_definition_language_parser_parse_name(&name_value, node, context)) {
      return Core_Failure;
    }
  }
  if (dx_assets_viewer_create(&viewer_value, name_value)) {
    CORE_UNREFERENCE(name_value);
    name_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(name_value);
  name_value = NULL;
  // source?
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(source_key))) {
      if (Core_Error_NotFound != Core_getError()) {
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
    } else {
      dx_assets_vector_3_f32* value;
      if (dx_asset_definition_language_parser_parse_vector_3_f32(&value, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      viewer_value->source = value->value;
      CORE_UNREFERENCE(value);
      value = NULL;
    }
  }
  // target?
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(target_key))) {
      if (Core_Error_NotFound != Core_getError()) {
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
    } else {
      dx_assets_vector_3_f32* value;
      if (dx_asset_definition_language_parser_parse_vector_3_f32(&value, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      viewer_value->target = value->value;
      CORE_UNREFERENCE(value);
      value = NULL;
    }
  }
  // up?
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(up_key))) {
      if (Core_Error_NotFound != Core_getError()) {
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
    } else {
      dx_assets_vector_3_f32* value;
      if (dx_asset_definition_language_parser_parse_vector_3_f32(&value, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      }
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
      viewer_value->up = value->value;
      CORE_UNREFERENCE(value);
      value = NULL;
    }
  }
  // optics?
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(optics_key))) {
      if (Core_Error_NotFound != Core_getError()) {
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
    } else {
      dx_assets_optics* optics = NULL;
      if (_parse_optics(&optics, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      }
      if (viewer_value->optics) {
        CORE_UNREFERENCE(viewer_value->optics);
        viewer_value->optics = NULL;
      }
      viewer_value->optics = optics;
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  // controller?
  {
    dx_ddl_node* child_node = NULL;
    if (dx_ddl_node_map_get(&child_node, node, NAME(controller_key))) {
      if (Core_Error_NotFound != Core_getError()) {
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      } else {
        Core_setError(Core_Error_NoError);
      }
    } else {
      dx_assets_viewer_controller* controller = NULL;
      if (_parse_viewer_controller(&controller, child_node, context)) {
        CORE_UNREFERENCE(child_node);
        child_node = NULL;
        CORE_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return Core_Failure;
      }
      viewer_value->controller = controller;
      CORE_UNREFERENCE(child_node);
      child_node = NULL;
    }
  }
  *RETURN = CORE_OBJECT(viewer_value);
  return Core_Success;
}

static Core_Result _resolve(dx_adl_type_handlers_viewer* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return Core_Success;
  }
  symbol->resolved = true;
  return Core_Success;
}

Core_Result dx_adl_type_handlers_viewer_construct(dx_adl_type_handlers_viewer* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_type_handlers_viewer);
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

static void dx_adl_type_handlers_viewer_destruct(dx_adl_type_handlers_viewer* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_adl_type_handlers_viewer_constructDispatch(dx_adl_type_handlers_viewer_Dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (Core_Result (*)(Core_Object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & _parse;
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (Core_Result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & _resolve;
}

Core_Result dx_adl_type_handlers_viewer_create(dx_adl_type_handlers_viewer** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_viewer);
  if (dx_adl_type_handlers_viewer_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
