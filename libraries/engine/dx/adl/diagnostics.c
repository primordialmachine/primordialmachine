#include "dx/adl/diagnostics.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.diagnostics",
                      dx_adl_diagnostics,
                      dx_object)

static void dx_adl_diagnostics_destruct(dx_adl_diagnostics* self)
{/*Intentionally empty.*/}

static void dx_adl_diagnostics_dispatch_construct(dx_adl_diagnostics_dispatch* self)
{/*Intentionally empty.*/}

dx_result dx_adl_diagnostics_construct(dx_adl_diagnostics* SELF) {
  dx_rti_type* TYPE = dx_adl_diagnostics_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_adl_diagnostics* dx_adl_diagnostics_create() {
  dx_adl_diagnostics* self = DX_ADL_DIAGNOSTICS(dx_object_alloc(sizeof(dx_adl_diagnostics)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_diagnostics_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

dx_result dx_adl_diagnostics_on_unclosed_list(dx_adl_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "syntactical error: unclosed list\n", sizeof("syntactical error: unclosed list\n") - 1)) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}

dx_result dx_adl_diagnostics_on_unclosed_map(dx_adl_diagnostics* SELF) {
  dx_string* format = NULL;
  if (dx_string_create(&format, "syntactical error: unclosed map\n", sizeof("syntactical error: unclosed map\n") - 1)) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(format);
  format = NULL;
  return DX_SUCCESS;
}
