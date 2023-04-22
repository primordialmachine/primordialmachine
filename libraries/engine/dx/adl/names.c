#include "dx/adl/names.h"

// strlen
#include <string.h>
#include <stdio.h>

DX_DECLARE_ENUMERATION_TYPE("dx.adl.name_index",
                            dx_adl_name_index)

DX_DEFINE_OBJECT_TYPE("dx.adl.names",
                      dx_adl_names,
                      dx_object)

static void dx_adl_names_destruct(dx_adl_names* self) {
  for (dx_size i = 0, n = DX_ADL_NAMES_NUMBER_OF_NAMES; i < n; ++i) {
    DX_DEBUG_ASSERT(self->names[i]);
    DX_UNREFERENCE(self->names[i]);
    self->names[i] = NULL;
  }
  if (self->names) {
    dx_memory_deallocate(self->names);
    self->names = NULL;
  }
}

static void dx_adl_names_dispatch_construct(dx_adl_names_dispatch* self)
{/*Intentionally empty.*/}

int dx_adl_names_construct(dx_adl_names* self) {
  dx_rti_type* _type = dx_adl_names_get_type();
  if (!_type) {
    goto ERROR;
  }
  if (dx_memory_allocate(&self->names, sizeof(dx_string*)*(DX_ADL_NAMES_NUMBER_OF_NAMES))) {
    goto ERROR;
  }
  memset(self->names, 0, sizeof(dx_string*) * (DX_ADL_NAMES_NUMBER_OF_NAMES));
#define DEFINE(NAME, STRING) \
  if (dx_string_create(&(self->names[dx_adl_name_index_##NAME]), STRING, sizeof(STRING) - 1)) { \
    goto ERROR; \
  }
#include "dx/adl/names.i"
#undef DEFINE

  self->counter = 0;

  DX_OBJECT(self)->type = _type;
  return 0;
ERROR:
  if (self->names) {
    for (dx_size i = 0, n = dx_adl_name_index_z_key + 1; i < n; ++i) {
      if (self->names[i]) {
        DX_UNREFERENCE(self->names[i]);
        self->names[i] = NULL;
      }
    }
  }
  if (self->names) {
    dx_memory_deallocate(self->names);
    self->names = NULL;
  }
  return 1;
}

dx_adl_names* dx_adl_names_create() {
  dx_adl_names* self = DX_ADL_NAMES(dx_object_alloc(sizeof(dx_adl_names)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_names_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
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
