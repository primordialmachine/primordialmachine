#if !defined(DX_ADL_CONTEXT_H_INCLUDED)
#define DX_ADL_CONTEXT_H_INCLUDED

#include "dx/data_definition_language.h"
#include "dx/assets.h"

#include "dx/adl/type_handler.h"
#include "dx/adl/parser.h"
#include "dx/adl/names.h"

/// @brief The state maintained of a compiler when compiling an "Asset Definition Language" program from a "Data Definition Language" program.
DX_DECLARE_OBJECT_TYPE("dx.adl.context",
                       dx_adl_context,
                       dx_object);

static inline dx_adl_context* DX_ADL_CONTEXT(void* p) {
  return (dx_adl_context*)p;
}

struct dx_adl_context {
  dx_object _parent;
  /// @brief The definitions being loaded.
  dx_asset_definitions* definitions;
  /// @brief The scene being loaded.
  dx_asset_scene* scene;

  /// @brief Map from names (dx_string) to type handlers (dx_adl_type_handler).
  dx_inline_pointer_hashmap type_handlers;
  /// @brief A pointer to the names.
  dx_adl_names* names;
  /// @brief A pointer to the parser object.
  dx_asset_definition_language_parser* parser;
  /// @brief A pointer to the diagnostics object.
  dx_asset_definition_language_diagnostics* diagnostics;
};

struct dx_adl_context_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_adl_context_construct(dx_adl_context* SELF);

/// @create-operator{dx_adl_context}
dx_result dx_adl_context_create(dx_adl_context** RETURN);

/// @brief Add a map entry (name, node) to the map from names (dx_string) to a type handler (dx_adl_type_handler).
/// @param name The name.
/// @param type_handler The type handler.
/// @error #DX_ERROR_ALREADY_EXISTS A map entry for the specified name already exists.
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed.
/// @error #DX_ERROR_INVALID_ARGUMENT @a name or @a type_handler is a null pointer.
/// @method{dx_adl_context}
dx_result dx_adl_context_add_type_handler(dx_adl_context* SELF, dx_string* name, dx_adl_type_handler* type_handler);

#endif // DX_ADL_CONTEXT_H_INCLUDED
