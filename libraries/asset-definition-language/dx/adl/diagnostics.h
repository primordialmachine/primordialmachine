/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_ASSET_DEFINITION_LANGUAGE_DIAGNOSTICS_H_INCLUDED)
#define DX_ASSET_DEFINITION_LANGUAGE_DIAGNOSTICS_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_OBJECT_TYPE("dx.asset_definition_language.diagnostics",
                       dx_asset_definition_language_diagnostics,
                       dx_object)

static inline dx_asset_definition_language_diagnostics* DX_ASSET_DEFINITION_LANGUAGE_DIAGNOSTICS(void* p) {
  return (dx_asset_definition_language_diagnostics*)p;
}

struct dx_asset_definition_language_diagnostics {
  dx_object _parent;
};

static inline dx_asset_definition_language_diagnostics_dispatch* DX_ASSET_DEFINITION_LANGUAGE_DIAGNOSTICS_DISPATCH(void* p) {
  return (dx_asset_definition_language_diagnostics_dispatch*)p;
}

struct dx_asset_definition_language_diagnostics_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_asset_definition_language_diagnostics_construct(dx_asset_definition_language_diagnostics* SELF);

/// @create-operator{dx_asset_definition_language_diagnostics}
dx_result dx_asset_definition_language_diagnostics_create(dx_asset_definition_language_diagnostics** RETURN);

dx_result dx_asset_definition_language_diagnostics_on_unclosed_list(dx_asset_definition_language_diagnostics* SELF);

dx_result dx_asset_definition_language_diagnostics_on_unclosed_map(dx_asset_definition_language_diagnostics* SELF);

#endif // DX_ASSET_DEFINITION_LANGUAGE_DIAGNOSTICS_H_INCLUDED
