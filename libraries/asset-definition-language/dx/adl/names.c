#include "dx/adl/names.h"

// strlen
#include <string.h>
#include <stdio.h>

DX_DECLARE_ENUMERATION_TYPE("dx.adl.name_index",
                            dx_adl_name_index)

DX_DEFINE_OBJECT_TYPE("dx.adl.names",
                      dx_adl_names,
                      dx_object)

static void dx_adl_names_destruct(dx_adl_names* SELF) {
  for (dx_size i = 0, n = DX_ADL_NAMES_NUMBER_OF_NAMES; i < n; ++i) {
    DX_DEBUG_ASSERT(SELF->names[i]);
    DX_UNREFERENCE(SELF->names[i]);
    SELF->names[i] = NULL;
  }
  if (SELF->names) {
    dx_memory_deallocate(SELF->names);
    SELF->names = NULL;
  }
}

static void dx_adl_names_dispatch_construct(dx_adl_names_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_adl_names_construct(dx_adl_names* SELF) {
  dx_rti_type* TYPE = dx_adl_names_get_type();
  if (!TYPE) {
    goto ERROR;
  }
  if (dx_memory_allocate(&SELF->names, sizeof(dx_string*)*(DX_ADL_NAMES_NUMBER_OF_NAMES))) {
    goto ERROR;
  }
  memset(SELF->names, 0, sizeof(dx_string*) * (DX_ADL_NAMES_NUMBER_OF_NAMES));
#define DEFINE(NAME, STRING) \
  if (dx_string_create(&(SELF->names[dx_adl_name_index_##NAME]), STRING, sizeof(STRING) - 1)) { \
    goto ERROR; \
  }
#include "dx/adl/names.i"
#undef DEFINE

  SELF->counter = 0;

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
ERROR:
  if (SELF->names) {
    for (dx_size i = 0, n = dx_adl_name_index_z_key + 1; i < n; ++i) {
      if (SELF->names[i]) {
        DX_UNREFERENCE(SELF->names[i]);
        SELF->names[i] = NULL;
      }
    }
  }
  if (SELF->names) {
    dx_memory_deallocate(SELF->names);
    SELF->names = NULL;
  }
  return DX_FAILURE;
}

dx_result dx_adl_names_create(dx_adl_names** RETURN) {
  DX_CREATE_PREFIX(dx_adl_names)
  if (dx_adl_names_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_string* dx_adl_names_create_unique_name(dx_adl_names* self) {
  /// @todo Use dx_string_formatf and dx_log.
  char buffer[1024 + 1];
  snprintf(buffer, 1024, "%zu", self->counter);
  dx_string* string = NULL;
  if (dx_string_create(&string, buffer, strlen(buffer))) {
    return NULL;
  }
  self->counter++;
  return string;
}
