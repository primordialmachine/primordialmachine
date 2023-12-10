#include "dx/ui/type_handlers/group.h"

#include "dx/data_definition_language.h"
#include "dx/ui/manager.h"
#include "dx/ui/group.h"

/// @brief Parse
/// @code
/// children : ListOf&lt;UI.Widget&gt;
/// @endcode
/// @param manager 
/// @param source 
/// @param receiver 
/// @method{dx_ui_type_handlers_group}
static Core_Result _parse_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver);

/// @brief Parse
/// @code
/// children?
/// @endcode
/// @param manager 
/// @param source 
/// @param receiver 
/// @method{dx_ui_type_handlers_group} 
static Core_Result _parse_maybe_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver);

static Core_Result _parse_widget(dx_ui_widget** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source);

static Core_Result _parse(dx_ui_group** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* node);

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static Core_Result _on_hash_expected_key_key(Core_Size* RETURN, Core_String** a);

static Core_Result _on_compare_expected_key_keys(Core_Boolean* RETURN, Core_String** a, Core_String** b);

static Core_Result _initialize_expected_keys(dx_ui_type_handlers_group* SELF);

static Core_Result _uninitialize_expected_keys(dx_ui_type_handlers_group* SELF);

static Core_Result _check_keys(dx_ui_type_handlers_group* SELF, dx_ddl_node* node);

DX_DEFINE_OBJECT_TYPE("dx.ui.type_handlers.group",
                      dx_ui_type_handlers_group,
                      dx_ui_type_handler);

static Core_Result _parse_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver) {
  Core_Size n = 0;
  if (dx_ddl_node_list_get_size(&n, source)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_ddl_node* v = NULL;
    if (dx_ddl_node_list_get(&v, source, i)) {
      return Core_Failure;
    }
    dx_ui_widget* vv = NULL;
    if (_parse_widget(&vv, SELF, manager, v)) {
      DX_UNREFERENCE(v);
      v = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(v);
    v = NULL;
    if (dx_object_array_append(receiver->children, CORE_OBJECT(vv))) {
      DX_UNREFERENCE(vv);
      vv = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(vv);
    vv = NULL;
  }
  return Core_Success;
}

static Core_Result _parse_maybe_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver) {
  Core_String* k = NULL;
  if (Core_String_create(&k, "children", sizeof("children") - 1)) {
    return Core_Failure;
  }
  dx_ddl_node* v = NULL;
  if (dx_ddl_node_map_get(&v, source, k)) {
    DX_UNREFERENCE(k);
    k = NULL;
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(Core_Error_NoError);
      return Core_Success;
    }
  }
  DX_UNREFERENCE(k);
  k = NULL;
  if (_parse_children(SELF, manager, v, receiver)) {
    DX_UNREFERENCE(v);
    v = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(v);
  v = NULL;
  return Core_Success;
}

static Core_Result _parse_widget(dx_ui_widget** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* node) {
  Core_String* type_field_name = NULL;
  if (Core_String_create(&type_field_name, "type", sizeof("type") - 1)) {
    return Core_Failure;
  }
  dx_ddl_node* type_node = NULL;
  if (dx_ddl_node_map_get(&type_node, node, type_field_name)) {
    DX_UNREFERENCE(type_field_name);
    type_field_name = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(type_field_name);
  type_field_name = NULL;
  Core_String* received_type = NULL;
  if (dx_ddl_node_get_string(&received_type, type_node)) {
    return Core_Failure;
  }
  dx_ui_type_handler* type_handler = NULL;
  if (dx_inline_pointer_hashmap_get(&type_handler, &manager->type_handlers, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_ui_widget* temporary;
  if (dx_ui_type_handler_parse(&temporary, type_handler, manager, node)) {
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

static Core_Result _parse(dx_ui_group** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* node) {
  if (_check_keys(SELF, node)) {
    return Core_Failure;
  }
  dx_ui_group* widget = NULL;
  if (dx_ui_group_create(&widget, manager)) {
    return Core_Failure;
  }
  if (dx_ui_type_handler_parse_widget_name(DX_UI_TYPE_HANDLER(SELF), manager, node, DX_UI_WIDGET(widget))) {
    DX_UNREFERENCE(widget);
    widget = NULL;
    return Core_Failure;
  }
  if (_parse_maybe_children(SELF, manager, node, widget)) {
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

static Core_Result _uninitialize_expected_keys(dx_ui_type_handlers_group* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return Core_Success;
}

static Core_Result _initialize_expected_keys(dx_ui_type_handlers_group* SELF) {
  DX_INLINE_POINTER_HASHMAP_CONFIGURATION cfg = {
    .key_added_callback = &_on_expected_key_key_added,
    .key_removed_callback = &_on_expected_key_key_removed,
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
  DEFINE("children");
#undef DEFINE
  return Core_Success;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static Core_Result _check_keys(dx_ui_type_handlers_group* SELF, dx_ddl_node* node) {
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

static void dx_ui_type_handlers_group_destruct(dx_ui_type_handlers_group* SELF) {
  _initialize_expected_keys(SELF);
}

static void dx_ui_type_handlers_group_constructDispatch(dx_ui_type_handlers_group_dispatch* SELF) {
  DX_UI_TYPE_HANDLER_DISPATCH(SELF)->parse = (Core_Result(*)(dx_ui_widget**, dx_ui_type_handler*, dx_ui_manager*, dx_ddl_node*)) & _parse;
}

Core_Result dx_ui_type_handlers_group_construct(dx_ui_type_handlers_group* SELF) {
  DX_CONSTRUCT_PREFIX(dx_ui_type_handlers_group);
  if (dx_ui_type_handler_construct(DX_UI_TYPE_HANDLER(SELF))) {
    return Core_Failure;
  }
  if (_initialize_expected_keys(SELF)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ui_type_handlers_group_create(dx_ui_type_handlers_group** RETURN) {
  DX_CREATE_PREFIX(dx_ui_type_handlers_group)
  if (dx_ui_type_handlers_group_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

