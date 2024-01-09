#if !defined(DX_VAL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_VAL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("dx.val.system_factory",
                       dx_val_system_factory,
                       dx_system_factory);

static inline dx_val_system_factory* DX_VAL_SYSTEM_FACTORY(void* p) {
  return (dx_val_system_factory*)p;
}

struct dx_val_system_factory {
  dx_system_factory _parent;
};

static inline dx_val_system_factory_Dispatch* DX_VAL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_val_system_factory_Dispatch*)p;
}

struct dx_val_system_factory_Dispatch {
  dx_system_factory_Dispatch _parent;
};

/// @brief Construct this VAL system factory_.
/// @param SELF A pointer to this VAL system factory object.
/// @method-call
Core_Result dx_val_system_factory_construct(dx_val_system_factory* SELF);

#endif // DX_VAL_SYSTEM_FACTORY_H_INCLUDED
