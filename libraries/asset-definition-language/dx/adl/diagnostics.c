#include "dx/adl/diagnostics.h"

Core_defineObjectType("dx.adl.diagnostics",
                      dx_asset_definition_language_diagnostics,
                      Core_Object);

static void dx_asset_definition_language_diagnostics_destruct(dx_asset_definition_language_diagnostics* SELF)
{/*Intentionally empty.*/}

static void dx_asset_definition_language_diagnostics_constructDispatch(dx_asset_definition_language_diagnostics_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_definition_language_diagnostics_construct(dx_asset_definition_language_diagnostics* SELF) {
  DX_CONSTRUCT_PREFIX(dx_asset_definition_language_diagnostics);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_definition_language_diagnostics_create(dx_asset_definition_language_diagnostics** RETURN) {
  DX_CREATE_PREFIX(dx_asset_definition_language_diagnostics);
  if (dx_asset_definition_language_diagnostics_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_asset_definition_language_diagnostics_on_unclosed_list(dx_asset_definition_language_diagnostics* SELF) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "syntactical error: unclosed list\n", sizeof("syntactical error: unclosed list\n") - 1)) {
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}

Core_Result dx_asset_definition_language_diagnostics_on_unclosed_map(dx_asset_definition_language_diagnostics* SELF) {
  Core_String* format = NULL;
  if (Core_String_create(&format, "syntactical error: unclosed map\n", sizeof("syntactical error: unclosed map\n") - 1)) {
    return Core_Failure;
  }
  CORE_UNREFERENCE(format);
  format = NULL;
  return Core_Success;
}
