#include "dx/core/string_iterator.h"

DX_DEFINE_OBJECT_TYPE("dx.string_iterator",
                       dx_string_iterator,
                       dx_object);

static void dx_string_iterator_destruct(dx_string_iterator* SELF)
{/*Intentionally empty.*/}

static void dx_string_iterator_dispatch_construct(dx_string_iterator_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_string_iterator_construct(dx_string_iterator* SELF) {
  dx_rti_type* TYPE = dx_string_iterator_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
