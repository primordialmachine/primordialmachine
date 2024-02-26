#include "dx/core/object_array.h"

Core_defineObjectType("dx.object_array",
                      dx_object_array,
                      Core_Object);

static void dx_object_array_destruct(dx_object_array* SELF) {
  dx_inline_object_array_uninitialize(&SELF->backend);
}

static void dx_object_array_constructDispatch(dx_object_array_Dispatch* SELF)
{/*Intentionally emtpy.*/}

Core_Result dx_object_array_construct(dx_object_array* SELF, Core_Size initial_capacity) {
  Core_BeginConstructor(dx_object_array);
  if (dx_inline_object_array_initialize(&SELF->backend, initial_capacity)) {
    return Core_Failure;
  }
  Core_EndConstructor(dx_object_array);
}

Core_Result dx_object_array_create(dx_object_array** RETURN, Core_Size initial_capacity) {
  DX_CREATE_PREFIX(dx_object_array);
  if (dx_object_array_construct(SELF, initial_capacity)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
