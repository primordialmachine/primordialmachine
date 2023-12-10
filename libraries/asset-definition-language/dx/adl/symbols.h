#if !defined(DX_ADL_SYMBOLS_H_INCLUDED)
#define DX_ADL_SYMBOLS_H_INCLUDED

#include "dx/data_definition_language.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.adl.symbol",
                       dx_adl_symbol,
                       Core_Object);

static inline dx_adl_symbol* DX_ADL_SYMBOL(void* p) {
  return (dx_adl_symbol*)p;
}

struct dx_adl_symbol {
  Core_Object _parent;
  /// @brief The type of this symbol.
  Core_String* type;
  /// @brief The name of this symbol.
  Core_String* name;
  /// @brief The node (if any) defining this symbol.
  dx_ddl_node* node;
  /// @brief The asset (if fully created) of this symbol.
  Core_Object* asset;
  /// @brief If the references were resolved and symbols for anonymous definitions were added.
  bool resolved;
};

static inline dx_adl_symbol_dispatch* DX_ADL_SYMBOL_DISPATCH(void* p) {
  return (dx_adl_symbol_dispatch*)p;
}

struct dx_adl_symbol_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_adl_symbol_construct(dx_adl_symbol* SELF, Core_String* type, Core_String* name);

Core_Result dx_adl_symbol_create(dx_adl_symbol** RETURN, Core_String* type, Core_String* name);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.adl.definitions",
                       dx_asset_definitions,
                       Core_Object);

static inline dx_asset_definitions* DX_ASSET_PALETTE(void* p) {
  return (dx_asset_definitions*)p;
}

struct dx_asset_definitions {
  Core_Object _parent;
  /// A map from names (Core_String*) to symbols (dx_adl_symbol*).
  dx_inline_pointer_hashmap map;
};

struct dx_asset_definitions_dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_asset_definitions_construct(dx_asset_definitions* SELF);

/// @create-operator{dx_asset_definitions}
Core_Result dx_asset_definitions_create(dx_asset_definitions** RETURN);

Core_Result dx_asset_definitions_get(dx_adl_symbol** RETURN, dx_asset_definitions const* SELF, Core_String* name);

Core_Result dx_asset_definitions_set(dx_asset_definitions* SELF, Core_String* name, dx_adl_symbol* symbol);

Core_Result dx_asset_definitions_dump(dx_asset_definitions* SELF);

#endif // DX_ADL_SYMBOLS_H_INCLUDED
