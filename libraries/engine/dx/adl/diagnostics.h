/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ADL_DIAGNOSTICS_H_INCLUDED)
#define DX_ADL_DIAGNOSTICS_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.diagnostics",
                       dx_adl_diagnostics,
                       dx_object)

static inline dx_adl_diagnostics* DX_ADL_DIAGNOSTICS(void* p) {
  return (dx_adl_diagnostics*)p;
}

struct dx_adl_diagnostics {
  dx_object _parent;
};

struct dx_adl_diagnostics_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_adl_diagnostics_construct(dx_adl_diagnostics* SELF);

/// @create-operator{dx_adl_diagnostics}
dx_result dx_adl_diagnostics_create(dx_adl_diagnostics** RETURN);

dx_result dx_adl_diagnostics_on_unclosed_list(dx_adl_diagnostics* SELF);

dx_result dx_adl_diagnostics_on_unclosed_map(dx_adl_diagnostics* SELF);

#endif // DX_ADL_DIAGNOSTICS_H_INCLUDED
