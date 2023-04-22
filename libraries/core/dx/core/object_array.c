#include "dx/core/object_array.h"

DX_DEFINE_OBJECT_TYPE("dx.object_array",
                      dx_object_array,
                      dx_object);

static void dx_object_array_destruct(dx_object_array* SELF) {
  dx_inline_object_array_uninitialize(&SELF->backend);
}

static void dx_object_array_dispatch_construct(dx_object_array_dispatch* SELF)
{/*Intentionally emtpy.*/}

dx_result dx_object_array_construct(dx_object_array* SELF, dx_size initial_capacity) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_object_array_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_inline_object_array_initialize(&SELF->backend, initial_capacity)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_object_array_create(dx_object_array** RETURN, dx_size initial_capacity) {
  dx_object_array* SELF = DX_OBJECT_ARRAY(dx_object_alloc(sizeof(dx_object_array)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_object_array_construct(SELF, initial_capacity)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
