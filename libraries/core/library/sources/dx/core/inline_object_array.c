#include "dx/core/inline_object_array.h"

static void added_callback(Core_Object **p) {
  if (*p) CORE_REFERENCE(*p);
}

static void removed_callback(Core_Object** p) {
  if (*p) CORE_UNREFERENCE(*p);
}

Core_Result dx_inline_object_array_initialize(dx_inline_object_array* SELF, Core_Size initial_capacity) {
  Core_InlineArrayListP_Configuration configuration = {
    .addedCallback = (void(*)(void*)) & added_callback,
    .removedCallback = (void(*)(void*)) & removed_callback,
  };
  if (Core_InlineArrayListP_initialize(&SELF->backend, initial_capacity, &configuration)) {
    return Core_Failure;
  }
  return Core_Success;
}

void dx_inline_object_array_uninitialize(dx_inline_object_array* SELF) {
  Core_InlineArrayListP_uninitialize(&SELF->backend);
}

Core_Result dx_inline_object_array_increase_capacity(dx_inline_object_array* SELF, Core_Size additional_capacity)  {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_increaseCapacity(&SELF->backend, additional_capacity);
}

Core_Result dx_inline_object_array_ensure_free_capacity(dx_inline_object_array* SELF, Core_Size required_free_capacity) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_ensureFreeCapacity(&SELF->backend, required_free_capacity);
}

Core_Result dx_inline_object_array_append(dx_inline_object_array* SELF, Core_Object *pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_append(&SELF->backend, pointer);
}

Core_Result dx_inline_object_array_prepend(dx_inline_object_array* SELF, Core_Object *pointer) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_prepend(&SELF->backend, pointer);
}

Core_Result dx_inline_object_array_insert(dx_inline_object_array* SELF, Core_Object *pointer, Core_Size index) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_insert(&SELF->backend, index, pointer);
}

Core_Result dx_inline_object_array_get_at(Core_Object** RETURN, dx_inline_object_array const* SELF, Core_Size index) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_get(RETURN, &SELF->backend, index);
}

Core_Result dx_inline_object_array_get_size(Core_Size* RETURN, dx_inline_object_array const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_getSize(RETURN, &SELF->backend);
}

Core_Result dx_inline_object_array_get_capacity(Core_Size* RETURN, dx_inline_object_array const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_getCapacity(RETURN, &SELF->backend);
}

Core_Result dx_inline_object_array_get_free_capacity(Core_Size* RETURN, dx_inline_object_array const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_getFreeCapacity(RETURN, &SELF->backend);
}

Core_Result dx_inline_object_array_clear(dx_inline_object_array* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_clear(&SELF->backend);
}

Core_Result dx_inline_object_array_pop_back_n(dx_inline_object_array* SELF, Core_Size n) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return Core_InlineArrayListP_removeManyBack(&SELF->backend, n);
}