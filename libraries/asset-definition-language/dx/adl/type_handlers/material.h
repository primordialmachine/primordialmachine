#if !defined(DX_ADL_TYPE_HANDLERS_MATERIAL_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_MATERIAL_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.material",
                       dx_adl_type_handlers_material,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_material* DX_ADL_TYPE_HANDLERS_MATERIAL(void* p) {
  return (dx_adl_type_handlers_material*)p;
}

struct dx_adl_type_handlers_material {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlineHashMapPP expected_keys;
};

static inline dx_adl_type_handlers_material_Dispatch* DX_ADL_TYPE_HANDLERS_MATERIAL_DISPATCH(void* p) {
  return (dx_adl_type_handlers_material_Dispatch*)p;
}

struct dx_adl_type_handlers_material_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* SELF);

Core_Result dx_adl_type_handlers_material_create(dx_adl_type_handlers_material** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_MATERIAL_H_INCLUDED
