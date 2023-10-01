#include "dx/adl/diagnostics.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.diagnostics",
                      dx_asset_definition_language_diagnostics,
                      dx_object)

static void dx_asset_definition_language_diagnostics_destruct(dx_asset_definition_language_diagnostics* SELF)
{/*Intentionally empty.*/}

static void dx_asset_definition_language_diagnostics_dispatch_construct(dx_asset_definition_language_diagnostics_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_definition_language_diagnostics_construct(dx_asset_definition_language_diagnostics* SELF) {
  dx_rti_type* TYPE = dx_asset_definition_language_diagnostics_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_diagnostics_create(dx_asset_definition_language_diagnostics** RETURN) {
  DX_CREATE_PREFIX(dx_asset_definition_language_diagnostics)
  if (dx_asset_definition_language_diagnostics_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_diagnostics_on_unclosed_list(dx_asset_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "syntactical error: unclosed list\n", sizeof("syntactical error: unclosed list\n") - 1)) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_diagnostics_on_unclosed_map(dx_asset_definition_language_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "syntactical error: unclosed map\n", sizeof("syntactical error: unclosed map\n") - 1)) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}
