#if !defined(DX_ADL_TYPE_HANDLERS_TEXTURE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_TEXTURE_H_INCLUDED

#include "dx/adl/context.h"

Core_declareObjectType("dx.adl.type_handlers.texture",
                       dx_adl_type_handlers_texture,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_texture* DX_ADL_TYPE_HANDLERS_TEXTURE(void* p) {
  return (dx_adl_type_handlers_texture*)p;
}

struct dx_adl_type_handlers_texture {
  dx_adl_type_handler _parent;
  /// @brief
  /// Set of expected keys.
  /// If a key is not in that set, then this is a semantical error.
  Core_InlineHashMapPP expected_keys;
};

static inline dx_adl_type_handlers_texture_Dispatch* DX_ADL_TYPE_HANDLERS_TEXTURE_DISPATCH(void* p) {
  return (dx_adl_type_handlers_texture_Dispatch*)p;
}

struct dx_adl_type_handlers_texture_Dispatch {
  dx_adl_type_handler_Dispatch _parent;
};

Core_Result dx_adl_type_handlers_texture_construct(dx_adl_type_handlers_texture* SELF);

Core_Result dx_adl_type_handlers_texture_create(dx_adl_type_handlers_texture** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_TEXTURE_H_INCLUDED
