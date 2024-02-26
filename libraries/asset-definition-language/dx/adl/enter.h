#if !defined(DX_ASSET_DEFINITION_LANGUAGE_ENTER_H_INCLUDED)
#define DX_ASSET_DEFINITION_LANGUAGE_ENTER_H_INCLUDED

#include "dx/core.h"
#include "dx/data_definition_language.h"
#include "dx/assets.h"
#include "dx/adl/context.h"
typedef struct dx_asset_definition_language_diagnostics dx_asset_definition_language_diagnostics;

/// "enter" phase: Enter symbols into the symbol table.
Core_declareObjectType("dx.asset_definition_language.enter",
                       dx_asset_definition_language_enter,
                       Core_Object);

static inline dx_asset_definition_language_enter* DX_ASSET_DEFINITION_LANGUAGE_ENTER(void* p) {
  return (dx_asset_definition_language_enter*)p;
}

struct dx_asset_definition_language_enter {
  Core_Object _parent;
  /// @brief The diagnostics object used by this phase.
  dx_asset_definition_language_diagnostics* diagnostics;
  dx_adl_context* context;
};

static inline dx_asset_definition_language_enter_Dispatch* DX_ASSET_DEFINITION_LANGUAGE_ENTER_DISPATCH(void* p) {
  return (dx_asset_definition_language_enter_Dispatch*)p;
}

struct dx_asset_definition_language_enter_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_asset_definition_language_enter_construct(dx_asset_definition_language_enter* SELF, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_create(dx_asset_definition_language_enter** RETURN, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_run(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_scene(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

#endif // DX_ASSET_DEFINITION_LANGUAGE_ENTER_H_INCLUDED
