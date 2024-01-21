/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ADL_RESOLVE_H_INCLUDED)
#define DX_ADL_RESOLVE_H_INCLUDED

#include "dx/core.h"
#include "dx/data_definition_language.h"
#include "dx/assets.h"
#include "dx/adl/context.h"

/// "resolve" phase: Resolve references.
Core_declareObjectType("dx.adl.resolve",
                       dx_adl_resolve,
                       Core_Object);

static inline dx_adl_resolve* DX_ADL_RESOLVE(void* p) {
  return (dx_adl_resolve*)p;
}

struct dx_adl_resolve {
  Core_Object _parent;
  dx_adl_context* context;
  Core_InlinePointerArray queue;
};

struct dx_adl_resolve_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_adl_resolve_construct(dx_adl_resolve* SELF, dx_adl_context* context);

Core_Result dx_adl_resolve_create(dx_adl_resolve** RETURN, dx_adl_context* context);

Core_Result dx_adl_resolve_run(dx_adl_resolve* SELF);

#endif // DX_ADL_RESOLVE_H_INCLUDED
