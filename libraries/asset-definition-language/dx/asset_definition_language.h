/// @file dx/asset_definition_language.h
/// @brief Invocation of the Asset Definition Language
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)

#if !defined(DX_ASSET_DEFINITION_LANGUAGE_H_INCLUDED)
#define DX_ASSET_DEFINITION_LANGUAGE_H_INCLUDED

#include "dx/data_definition_language.h"
#include "dx/assets.h"

Core_Result dx_adl_compile(dx_assets_scene** RETURN, dx_ddl_node* node);

#endif // DX_ASSET_DEFINITION_LANGUAGE_H_INCLUDED
