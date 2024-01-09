#include "dx/adl/names.h"

// strlen
#include <string.h>
#include <stdio.h>

Core_defineEnumerationType("dx.adl.name_index",
                            dx_adl_name_index);

Core_defineObjectType("dx.adl.names",
                      dx_adl_names,
                      Core_Object);

static void dx_adl_names_destruct(dx_adl_names* SELF) {
  for (Core_Size i = 0, n = DX_ADL_NAMES_NUMBER_OF_NAMES; i < n; ++i) {
    DX_DEBUG_ASSERT(SELF->names[i]);
    CORE_UNREFERENCE(SELF->names[i]);
    SELF->names[i] = NULL;
  }
  if (SELF->names) {
    Core_Memory_deallocate(SELF->names);
    SELF->names = NULL;
  }
}

static void dx_adl_names_constructDispatch(dx_adl_names_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_adl_names_construct(dx_adl_names* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_names);
  if (Core_Memory_allocate((void**)&SELF->names, sizeof(Core_String*) * (DX_ADL_NAMES_NUMBER_OF_NAMES))) {
    goto ERROR;
  }
  Core_Memory_zero(SELF->names, sizeof(Core_String*) * (DX_ADL_NAMES_NUMBER_OF_NAMES));
#define DEFINE(NAME, STRING) \
  if (Core_String_create(&(SELF->names[dx_adl_name_index_##NAME]), STRING, sizeof(STRING) - 1)) { \
    goto ERROR; \
  }
#include "dx/adl/names.i"
#undef DEFINE

  SELF->counter = 0;

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
ERROR:
  if (SELF->names) {
    for (Core_Size i = 0, n = dx_adl_name_index_z_key + 1; i < n; ++i) {
      if (SELF->names[i]) {
        CORE_UNREFERENCE(SELF->names[i]);
        SELF->names[i] = NULL;
      }
    }
  }
  if (SELF->names) {
    Core_Memory_deallocate(SELF->names);
    SELF->names = NULL;
  }
  return Core_Failure;
}

Core_Result dx_adl_names_create(dx_adl_names** RETURN) {
  DX_CREATE_PREFIX(dx_adl_names);
  if (dx_adl_names_construct(SELF)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_String* dx_adl_names_create_unique_name(dx_adl_names* self) {
  /// @todo Use dx_string_formatf and dx_log.
  char buffer[1024 + 1];
  snprintf(buffer, 1024, "%zu", self->counter);
  Core_String* string = NULL;
  if (Core_String_create(&string, buffer, strlen(buffer))) {
    return NULL;
  }
  self->counter++;
  return string;
}
