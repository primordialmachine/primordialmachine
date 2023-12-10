#if !defined(DX_UI_TYPE_HANDLER_H_INCLUDED)
#define DX_UI_TYPE_HANDLER_H_INCLUDED

#include "dx/core.h"
typedef struct dx_ddl_node dx_ddl_node;
typedef struct dx_ui_widget dx_ui_widget;
typedef struct dx_ui_manager dx_ui_manager;

DX_DECLARE_OBJECT_TYPE("dx.ui.type_handler",
                       dx_ui_type_handler,
                       Core_Object);

static inline dx_ui_type_handler* DX_UI_TYPE_HANDLER(void* p) {
  return (dx_ui_type_handler*)p;
}

struct dx_ui_type_handler {
  Core_Object _parent;
};

static inline dx_ui_type_handler_dispatch* DX_UI_TYPE_HANDLER_DISPATCH(void* p) {
  return (dx_ui_type_handler_dispatch*)p;
}

struct dx_ui_type_handler_dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*parse)(dx_ui_widget** RETURN, dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* source);
  Core_Result(*parse_widget_name)(dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_widget* widget);
};

Core_Result dx_ui_type_handler_construct(dx_ui_type_handler* SELF);

static inline Core_Result dx_ui_type_handler_parse(dx_ui_widget** RETURN, dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* source) {
  DX_OBJECT_VIRTUALCALL(dx_ui_type_handler, parse, RETURN, SELF, manager, source);
}

/// @todo Should be package scope.
static inline Core_Result dx_ui_type_handler_parse_widget_name(dx_ui_type_handler* SELF, dx_ui_manager* manager, dx_ddl_node* source, dx_ui_widget* widget) {
  DX_OBJECT_VIRTUALCALL(dx_ui_type_handler, parse_widget_name, SELF, manager, source, widget);
}

#endif // DX_UI_TYPE_HANDLER_H_INCLUDED
