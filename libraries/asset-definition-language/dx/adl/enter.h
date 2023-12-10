#if !defined(DX_ASSET_DEFINITION_LANGUAGE_ENTER_H_INCLUDED)
#define DX_ASSET_DEFINITION_LANGUAGE_ENTER_H_INCLUDED

#include "dx/core.h"
#include "dx/data_definition_language.h"
#include "dx/assets.h"
#include "dx/adl/context.h"
typedef struct dx_asset_definition_language_diagnostics dx_asset_definition_language_diagnostics;

/// "enter" phase: Enter symbols into the symbol table.
DX_DECLARE_OBJECT_TYPE("dx.asset_definition_language.enter",
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

static inline dx_asset_definition_language_enter_dispatch* DX_ASSET_DEFINITION_LANGUAGE_ENTER_DISPATCH(void* p) {
  return (dx_asset_definition_language_enter_dispatch*)p;
}

struct dx_asset_definition_language_enter_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_asset_definition_language_enter_construct(dx_asset_definition_language_enter* SELF, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_create(dx_asset_definition_language_enter** RETURN, dx_asset_definition_language_diagnostics* diagnostics, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_run(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_color(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_scene(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_image(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_mesh(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_mesh_instance(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_material(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_viewer_instance(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_viewer(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

Core_Result dx_asset_definition_language_enter_on_texture(dx_asset_definition_language_enter* SELF, dx_ddl_node* source, dx_adl_context* context);

#endif // DX_ASSET_DEFINITION_LANGUAGE_ENTER_H_INCLUDED
