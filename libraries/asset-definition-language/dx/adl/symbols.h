#if !defined(DX_ADL_SYMBOLS_H_INCLUDED)
#define DX_ADL_SYMBOLS_H_INCLUDED

#include "dx/data_definition_language.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.adl.symbol",
                       dx_adl_symbol,
                       dx_object)

static inline dx_adl_symbol* DX_ADL_SYMBOL(void* p) {
  return (dx_adl_symbol*)p;
}

struct dx_adl_symbol {
  dx_object _parent;
  /// @brief The type of this symbol.
  dx_string* type;
  /// @brief The name of this symbol.
  dx_string* name;
  /// @brief The node (if any) defining this symbol.
  dx_ddl_node* node;
  /// @brief The asset (if fully created) of this symbol.
  dx_object* asset;
  /// @brief If the references were resolved and symbols for anonymous definitions were added.
  bool resolved;
};

static inline dx_adl_symbol_dispatch* DX_ADL_SYMBOL_DISPATCH(void* p) {
  return (dx_adl_symbol_dispatch*)p;
}

struct dx_adl_symbol_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_adl_symbol_construct(dx_adl_symbol* SELF, dx_string* type, dx_string* name);

dx_result dx_adl_symbol_create(dx_adl_symbol** RETURN, dx_string* type, dx_string* name);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.adl.definitions",
                       dx_asset_definitions,
                       dx_object)

static inline dx_asset_definitions* DX_ASSET_PALETTE(void* p) {
  return (dx_asset_definitions*)p;
}

struct dx_asset_definitions {
  dx_object _parent;
  /// A map from names (dx_string*) to symbols (dx_adl_symbol*).
  dx_inline_pointer_hashmap map;
};

struct dx_asset_definitions_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_asset_definitions_construct(dx_asset_definitions* SELF);

/// @create-operator{dx_asset_definitions}
dx_result dx_asset_definitions_create(dx_asset_definitions** RETURN);

dx_adl_symbol* dx_asset_definitions_get(dx_asset_definitions const* self, dx_string* name);

dx_result dx_asset_definitions_set(dx_asset_definitions* self, dx_string* name, dx_adl_symbol* symbol);

dx_result dx_asset_definitions_dump(dx_asset_definitions* self);

#endif // DX_ADL_SYMBOLS_H_INCLUDED
