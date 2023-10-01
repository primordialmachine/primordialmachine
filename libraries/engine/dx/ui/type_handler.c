#include "dx/ui/type_handler.h"

#include "dx/data_definition_language.h"
#include "dx/ui/widget.h"

DX_DEFINE_OBJECT_TYPE("dx.ui.type_handler",
                      dx_ui_type_handler,
                      dx_object);

/// Parse the widget name.
static dx_result _parse_widget_name(dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* node, dx_ui_widget* widget);

static void dx_ui_type_handler_destruct(dx_ui_type_handler* SELF)
{/*Intentionally empty.*/}

static void dx_ui_type_handler_dispatch_construct(dx_ui_type_handler_dispatch* SELF) {
  SELF->parse_widget_name = &_parse_widget_name;
}

static dx_result _parse_widget_name(dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* node, dx_ui_widget* widget) {
  dx_string* node_key = NULL;
  if (dx_string_create(&node_key, "name", sizeof("name") - 1)) {
    return DX_FAILURE;
  }
  dx_ddl_node* node_value = NULL;
  if (dx_ddl_node_map_get(&node_value, node, node_key)) {
    DX_UNREFERENCE(node_key);
    node_key = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(node_key);
  node_key = NULL;
  dx_string* string_value = NULL;
  if (dx_ddl_node_get_string(&string_value, node_value)) {
    DX_UNREFERENCE(node_value);
    node_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(node_value);
  node_value = NULL;

  if (dx_ui_widget_set_name(widget, string_value)) {
    DX_UNREFERENCE(string_value);
    string_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(string_value);
  string_value = NULL;
  return DX_SUCCESS;
}

dx_result dx_ui_type_handler_construct(dx_ui_type_handler* SELF) {
  dx_rti_type* TYPE = dx_ui_type_handler_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
