#if !defined(DX_ADL_CONTEXT_H_INCLUDED)
#define DX_ADL_CONTEXT_H_INCLUDED

#include "dx/data_definition_language.h"
#include "dx/assets.h"

#include "dx/adl/type_handler.h"
#include "dx/adl/parser.h"
#include "dx/adl/names.h"

/// @brief The state maintained of a compiler when compiling an "Asset Definition Language" program from a "Data Definition Language" program.
Core_declareObjectType("dx.adl.context",
                       dx_adl_context,
                       Core_Object);

static inline dx_adl_context* DX_ADL_CONTEXT(void* p) {
  return (dx_adl_context*)p;
}

struct dx_adl_context {
  Core_Object _parent;
  /// @brief The definitions being loaded.
  dx_asset_definitions* definitions;
  /// @brief The scene being loaded.
  dx_assets_scene* scene;

  /// @brief Map from names (Core_String) to type handlers (dx_adl_type_handler).
  Core_InlineHashMapPP type_handlers;
  /// @brief A pointer to the names.
  dx_adl_names* names;
  /// @brief A pointer to the parser object.
  dx_asset_definition_language_parser* parser;
  /// @brief A pointer to the diagnostics object.
  dx_asset_definition_language_diagnostics* diagnostics;
};

struct dx_adl_context_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_adl_context_construct(dx_adl_context* SELF);

/// @create-operator{dx_adl_context}
Core_Result dx_adl_context_create(dx_adl_context** RETURN);

/// @brief Add a map entry (name, node) to the map from names (Core_String) to a type handler (dx_adl_type_handler).
/// @param name The name.
/// @param type_handler The type handler.
/// @error #DX_ERROR_ALREADY_EXISTS A map entry for the specified name already exists.
/// @error #Core_Error_AllocationFailed an allocation failed.
/// @error #Core_Error_ArgumentInvalid @a name or @a type_handler is a null pointer.
/// @method{dx_adl_context}
Core_Result dx_adl_context_add_type_handler(dx_adl_context* SELF, Core_String* name, dx_adl_type_handler* type_handler);

#endif // DX_ADL_CONTEXT_H_INCLUDED
