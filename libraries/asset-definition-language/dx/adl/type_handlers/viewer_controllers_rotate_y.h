#if !defined(DX_ADL_TYPE_HANDLERS_VIEWER_CONTROLLERS_ROTATE_Y_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_VIEWER_CONTROLLERS_ROTATE_Y_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.viewer_controllers_rotate_y",
                       dx_adl_type_handlers_viewer_controllers_rotate_y,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_viewer_controllers_rotate_y* DX_ADL_TYPE_HANDLERS_VIEWER_CONTROLLERS_ROTATE_Y(void* p) {
  return (dx_adl_type_handlers_viewer_controllers_rotate_y*)p;
}

struct dx_adl_type_handlers_viewer_controllers_rotate_y {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  dx_inline_pointer_hashmap expected_keys;
};

static inline dx_adl_type_handlers_viewer_controllers_rotate_y_dispatch* DX_ADL_TYPE_HANDLERS_VIEWER_CONTROLLERS_ROTATE_Y_DISPATCH(void* p) {
  return (dx_adl_type_handlers_viewer_controllers_rotate_y_dispatch*)p;
}

struct dx_adl_type_handlers_viewer_controllers_rotate_y_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

Core_Result dx_adl_type_handlers_viewer_controllers_rotate_y_construct(dx_adl_type_handlers_viewer_controllers_rotate_y* SELF);

Core_Result dx_adl_type_handlers_viewer_controllers_rotate_y_create(dx_adl_type_handlers_viewer_controllers_rotate_y** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_VIEWER_CONTROLLERS_ROTATE_Y_H_INCLUDED
