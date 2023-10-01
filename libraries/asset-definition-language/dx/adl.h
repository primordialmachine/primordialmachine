
/// @file dx/adl.h
/// @brief Invocation of the Asset Definition Language ("ADL") c/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)

#if !defined(DX_ADL_H_INCLUDED)
#define DX_ADL_H_INCLUDED

#include "dx/data_definition_language.h"
#include "dx/assets.h"

dx_result dx_adl_compile(dx_asset_scene** RETURN, dx_ddl_node* node);

#endif // DX_ADL_H_INCLUDED
