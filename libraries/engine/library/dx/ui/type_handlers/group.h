#if !defined(DX_UI_TYPE_HANDLERS_GROUP_H_INCLUDED)
#define DX_UI_TYPE_HANDLERS_GROUP_H_INCLUDED

#include "dx/ui/type_handler.h"

Core_declareObjectType("dx.ui.type_handlers.group",
                       dx_ui_type_handlers_group,
                       dx_ui_type_handler);

static inline dx_ui_type_handlers_group* DX_UI_TYPE_HANDLERS_GROUP(void* p) {
  return (dx_ui_type_handlers_group*)p;
}

struct dx_ui_type_handlers_group {
  dx_ui_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlineHashMapPP expected_keys;
};

static inline dx_ui_type_handlers_group_Dispatch* DX_UI_TYPE_HANDLERS_GROUP_DISPATCH(void* p) {
  return (dx_ui_type_handlers_group_Dispatch*)p;
}

struct dx_ui_type_handlers_group_Dispatch {
  dx_ui_type_handler_Dispatch _parent;
};

Core_Result dx_ui_type_handlers_group_construct(dx_ui_type_handlers_group* SELF);

Core_Result dx_ui_type_handlers_group_create(dx_ui_type_handlers_group** RETURN);

#endif // DX_UI_TYPE_HANDLERS_GROUP_H_INCLUDED
