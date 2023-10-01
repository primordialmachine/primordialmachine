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
static dx_result _parse_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver);

/// @brief Parse
/// @code
/// children?
/// @endcode
/// @param manager 
/// @param source 
/// @param receiver 
/// @method{dx_ui_type_handlers_group} 
static dx_result _parse_maybe_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver);

static dx_result _parse_widget(dx_ui_widget** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source);

static dx_result _parse(dx_ui_group** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* node);

static void _on_expected_key_key_added(void** a);

static void _on_expected_key_key_removed(void** a);

static dx_result _on_hash_expected_key_key(dx_size* RETURN, void** a);

static dx_result _on_compare_expected_key_keys(dx_bool* RETURN, void** a, void** b);

static dx_result _initialize_expected_keys(dx_ui_type_handlers_group* SELF);

static dx_result _uninitialize_expected_keys(dx_ui_type_handlers_group* SELF);

static dx_result _check_keys(dx_ui_type_handlers_group* SELF, dx_ddl_node* node);

DX_DEFINE_OBJECT_TYPE("dx.ui.type_handlers.group",
                      dx_ui_type_handlers_group,
                      dx_ui_type_handler);

static dx_result _parse_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver) {
  dx_size n = 0;
  if (dx_ddl_node_list_get_size(&n, source)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_ddl_node* v = NULL;
    if (dx_ddl_node_list_get(&v, source, i)) {
      return DX_FAILURE;
    }
    dx_ui_widget* vv = NULL;
    if (_parse_widget(&vv, SELF, manager, v)) {
      DX_UNREFERENCE(v);
      v = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(v);
    v = NULL;
    if (dx_object_array_append(receiver->children, DX_OBJECT(vv))) {
      DX_UNREFERENCE(vv);
      vv = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(vv);
    vv = NULL;
  }
  return DX_SUCCESS;
}

static dx_result _parse_maybe_children(dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_group* receiver) {
  dx_string* k = NULL;
  if (dx_string_create(&k, "children", sizeof("children") - 1)) {
    return DX_FAILURE;
  }
  dx_ddl_node* v = NULL;
  if (dx_ddl_node_map_get(&v, source, k)) {
    DX_UNREFERENCE(k);
    k = NULL;
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(DX_NO_ERROR);
      return DX_SUCCESS;
    }
  }
  DX_UNREFERENCE(k);
  k = NULL;
  if (_parse_children(SELF, manager, v, receiver)) {
    DX_UNREFERENCE(v);
    v = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(v);
  v = NULL;
  return DX_SUCCESS;
}

static dx_result _parse_widget(dx_ui_widget** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* node) {
  dx_string* type_field_name = NULL;
  if (dx_string_create(&type_field_name, "type", sizeof("type") - 1)) {
    return DX_FAILURE;
  }
  dx_ddl_node* type_node = NULL;
  if (dx_ddl_node_map_get(&type_node, node, type_field_name)) {
    DX_UNREFERENCE(type_field_name);
    type_field_name = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(type_field_name);
  type_field_name = NULL;
  dx_string* received_type = NULL;
  if (dx_ddl_node_get_string(&received_type, type_node)) {
    return DX_FAILURE;
  }
  dx_ui_type_handler* type_handler = NULL;
  if (dx_inline_pointer_hashmap_get(&type_handler, &manager->type_handlers, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_ui_widget* temporary;
  if (dx_ui_type_handler_parse(&temporary, type_handler, manager, node)) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

static dx_result _parse(dx_ui_group** RETURN, dx_ui_type_handlers_group* SELF, dx_ui_manager* manager, dx_ddl_node* node) {
  if (_check_keys(SELF, node)) {
    return DX_FAILURE;
  }
  dx_ui_group* widget = NULL;
  if (dx_ui_group_create(&widget, manager)) {
    return DX_FAILURE;
  }
  if (dx_ui_type_handler_parse_widget_name(DX_UI_TYPE_HANDLER(SELF), manager, node, DX_UI_WIDGET(widget))) {
    DX_UNREFERENCE(widget);
    widget = NULL;
    return DX_FAILURE;
  }
  if (_parse_maybe_children(SELF, manager, node, widget)) {
    DX_UNREFERENCE(widget);
    widget = NULL;
    return DX_FAILURE;
  }
  *RETURN = widget;
  return DX_SUCCESS;
}

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

static dx_result _uninitialize_expected_keys(dx_ui_type_handlers_group* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->expected_keys);
  return DX_SUCCESS;
}

static dx_result _initialize_expected_keys(dx_ui_type_handlers_group* SELF) {
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
  DEFINE("children");
#undef DEFINE
  return DX_SUCCESS;
}

static void on_received_key_added(void** p) {
  DX_REFERENCE(*p);
}

static void on_received_key_removed(void** p) {
  DX_UNREFERENCE(*p);
}

static dx_result _check_keys(dx_ui_type_handlers_group* SELF, dx_ddl_node* node) {
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

static void dx_ui_type_handlers_group_destruct(dx_ui_type_handlers_group* SELF) {
  _initialize_expected_keys(SELF);
}

static void dx_ui_type_handlers_group_dispatch_construct(dx_ui_type_handlers_group_dispatch* SELF) {
  DX_UI_TYPE_HANDLER_DISPATCH(SELF)->parse = (dx_result(*)(dx_ui_widget**, dx_ui_type_handler*, dx_ui_manager*, dx_ddl_node*)) & _parse;
}

dx_result dx_ui_type_handlers_group_construct(dx_ui_type_handlers_group* SELF) {
  dx_rti_type* TYPE = dx_ui_type_handlers_group_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_ui_type_handler_construct(DX_UI_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  if (_initialize_expected_keys(SELF)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ui_type_handlers_group_create(dx_ui_type_handlers_group** RETURN) {
  DX_CREATE_PREFIX(dx_ui_type_handlers_group)
  if (dx_ui_type_handlers_group_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

