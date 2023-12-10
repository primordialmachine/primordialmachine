#include "dx/ui/type_handlers/text_field.h"

#include "dx/data_definition_language.h"
#include "dx/ui/text_field.h"

static Core_Result _parse(dx_ui_text_field** RETURN, dx_ui_type_handlers_text_field* SELF, dx_ui_manager* manager, dx_ddl_node* node);

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static Core_Result _initialize_expected_keys(dx_ui_type_handlers_text_field* SELF);

static Core_Result _uninitialize_expected_keys(dx_ui_type_handlers_text_field* SELF);

static Core_Result _check_keys(dx_ui_type_handlers_text_field* SELF, dx_ddl_node* node);

DX_DEFINE_OBJECT_TYPE("dx.ui.type_handlers.text_field",
                      dx_ui_type_handlers_text_field,
                      dx_ui_type_handler);

static Core_Result _parse(dx_ui_text_field** RETURN, dx_ui_type_handlers_text_field* SELF, dx_ui_manager* manager, dx_ddl_node* node) {
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  dx_ui_text_field* widget = NULL;
  if (dx_ui_text_field_create(&widget, manager)) {
    return Core_Failure;
  }
  if (dx_ui_type_handler_parse_widget_name(DX_UI_TYPE_HANDLER(SELF), manager, node, DX_UI_WIDGET(widget))) {
    DX_UNREFERENCE(widget);
    widget = NULL;
    return Core_Failure;
  }
  *RETURN = widget;
  return Core_Success;
}

static void _on_expected_key_key_added(void** a) {
  DX_REFERENCE(*a);
}

static void _on_expected_key_key_removed(void** a) {
  DX_UNREFERENCE(*a);
}

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result _uninitialize_expected_keys(dx_ui_type_handlers_text_field* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_ui_type_handlers_text_field* SELF) {
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION cfg = {
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&_on_expected_key_key_added,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&_on_expected_key_key_removed,
    .value_added_callback = NULL,
    .value_removed_callback = NULL,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&_on_hash_expected_key_key,
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&_on_compare_expected_key_keys,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->expected_keys, &cfg)) {
    return Core_Failure;
  }

#define DEFINE(EXPECTED_KEY) \
  { \
    Core_String* expected_key = NULL; \
    if (Core_String_create(&expected_key, EXPECTED_KEY, sizeof(EXPECTED_KEY)-1)) { \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    if (dx_inline_pointer_hashmap_set(&SELF->expected_keys, expected_key, expected_key)) {\
      DX_UNREFERENCE(expected_key); \
      expected_key = NULL; \
      dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys); \
      return Core_Failure; \
    } \
    DX_UNREFERENCE(expected_key); \
    expected_key = NULL; \
  }
  DEFINE("type");
  DEFINE("name");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_ui_type_handlers_text_field* SELF, dx_ddl_node* node) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration = {
    .added_callback = &on_received_key_added,
    .removed_callback = &on_received_key_removed,
  };
  dx_inline_pointer_array received_keys;
  if (dx_inline_pointer_array_initialize(&received_keys, 0, &configuration)) {
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_get_keys(&node->map, &received_keys)) {
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
    if (dx_inline_pointer_hashmap_get(&expected_key, &SELF->expected_keys, received_key)) {
      dx_inline_pointer_array_uninitialize(&received_keys);
      return Core_Failure;
    }
  }
  dx_inline_pointer_array_uninitialize(&received_keys);
  return Core_Success;
}

static void dx_ui_type_handlers_text_field_destruct(dx_ui_type_handlers_text_field* SELF) {
  _uninitialize_expected_keys(SELF);
}

static void dx_ui_type_handlers_text_field_constructDispatch(dx_ui_type_handlers_text_field_dispatch* SELF) {
  DX_UI_TYPE_HANDLER_DISPATCH(SELF)->parse = (Core_Result(*)(dx_ui_widget**, dx_ui_type_handler*, dx_ui_manager*, dx_ddl_node*)) & _parse;
}

Core_Result dx_ui_type_handlers_text_field_construct(dx_ui_type_handlers_text_field* SELF) {
  DX_CONSTRUCT_PREFIX(dx_ui_type_handlers_text_field);
  if (dx_ui_type_handler_construct(DX_UI_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ui_type_handlers_text_field_create(dx_ui_type_handlers_text_field** RETURN) {
  DX_CREATE_PREFIX(dx_ui_type_handlers_text_field);
  if (dx_ui_type_handlers_text_field_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
