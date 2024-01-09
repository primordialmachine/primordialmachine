#include "dx/ui/type_handler.h"

#include "dx/data_definition_language.h"
#include "dx/ui/widget.h"

Core_defineObjectType("dx.ui.type_handler",
                      dx_ui_type_handler,
                      Core_Object);

/// Parse the widget name.
static Core_Result _parse_widget_name(dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* node, dx_ui_widget* widget);

static void dx_ui_type_handler_destruct(dx_ui_type_handler* SELF)
{/*Intentionally empty.*/}

static void dx_ui_type_handler_constructDispatch(dx_ui_type_handler_Dispatch* SELF) {
  SELF->parse_widget_name = &_parse_widget_name;
}

static Core_Result _parse_widget_name(dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* node, dx_ui_widget* widget) {
  Core_String* node_key = NULL;
  if (Core_String_create(&node_key, "name", sizeof("name") - 1)) {
    return Core_Failure;
  }
  dx_ddl_node* node_value = NULL;
  if (dx_ddl_node_map_get(&node_value, node, node_key)) {
    CORE_UNREFERENCE(node_key);
    node_key = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(node_key);
  node_key = NULL;
  Core_String* string_value = NULL;
  if (dx_ddl_node_get_string(&string_value, node_value)) {
    CORE_UNREFERENCE(node_value);
    node_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(node_value);
  node_value = NULL;

  if (dx_ui_widget_set_name(widget, string_value)) {
    CORE_UNREFERENCE(string_value);
    string_value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(string_value);
  string_value = NULL;
  return Core_Success;
}

Core_Result dx_ui_type_handler_construct(dx_ui_type_handler* SELF) {
  DX_CONSTRUCT_PREFIX(dx_ui_type_handler);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
