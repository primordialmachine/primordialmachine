/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ADL_RESOLVE_H_INCLUDED)
#define DX_ADL_RESOLVE_H_INCLUDED

#include "dx/core.h"
#include "dx/data_definition_language.h"
#include "dx/assets.h"
#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.resolve",
                       dx_adl_resolve,
                       dx_object)

static inline dx_adl_resolve* DX_ADL_RESOLVE(void* p) {
  return (dx_adl_resolve*)p;
}

struct dx_adl_resolve {
  dx_object _parent;
  dx_adl_context* context;
  dx_inline_pointer_array queue;
};

struct dx_adl_resolve_dispatch {
  dx_object_dispatch _parent;
};

int dx_adl_resolve_construct(dx_adl_resolve* self, dx_adl_context* context);

dx_adl_resolve* dx_adl_resolve_create(dx_adl_context* context);

int dx_adl_resolve_run(dx_adl_resolve* self);

#endif // DX_ADL_RESOLVE_H_INCLUDED
