#if !defined(DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.material_controllers",
                       dx_adl_type_handlers_material_controllers,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_material_controllers* DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS(void* p) {
  return (dx_adl_type_handlers_material_controllers*)p;
}

struct dx_adl_type_handlers_material_controllers {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlinePointerHashmap expected_keys;
};

static inline dx_adl_type_handlers_material_controllers_Dispatch* DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_DISPATCH(void* p) {
  return (dx_adl_type_handlers_material_controllers_Dispatch*)p;
}

struct dx_adl_type_handlers_material_controllers_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_material_controllers_construct(dx_adl_type_handlers_material_controllers* SELF);

Core_Result dx_adl_type_handlers_material_controllers_create(dx_adl_type_handlers_material_controllers** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_MATERIAL_CONTROLLERS_H_INCLUDED
