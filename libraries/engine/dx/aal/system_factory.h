#if !defined(DX_AAL_SYSTEM_FACTORY_H_INCLUDED)
#define DX_AAL_SYSTEM_FACTORY_H_INCLUDED

#include "dx/core.h"

Core_declareObjectType("dx.aal.system_factory",
                       dx_aal_system_factory,
                       dx_system_factory);

static inline dx_aal_system_factory* DX_AAL_SYSTEM_FACTORY(void* p) {
  return (dx_aal_system_factory*)p;
}

struct dx_aal_system_factory {
  dx_system_factory _parent;
};

static inline dx_aal_system_factory_Dispatch* DX_AAL_SYSTEM_FACTORY_DISPATCH(void* p) {
  return (dx_aal_system_factory_Dispatch*)p;
}

struct dx_aal_system_factory_Dispatch {
  dx_system_factory_Dispatch _parent;
};

/// @brief Construct this AAL system factory_.
/// @param SELF A pointer to this AAL system factory object.
/// @method-call
Core_Result dx_aal_system_factory_construct(dx_aal_system_factory* SELF);

#endif // DX_AAL_SYSTEM_FACTORY_H_INCLUDED
