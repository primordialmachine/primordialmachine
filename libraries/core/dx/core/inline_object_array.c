#include "dx/core/inline_object_array.h"

static void added_callback(dx_object **p) {
  if (*p) DX_REFERENCE(*p);
}

static void removed_callback(dx_object** p) {
  if (*p) DX_UNREFERENCE(*p);
}

dx_result dx_inline_object_array_initialize(dx_inline_object_array* self, dx_size initial_capacity) {
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration;
  configuration.added_callback = (void(*)(void*)) & added_callback;
  configuration.removed_callback = (void(*)(void*)) & removed_callback;
  if (dx_inline_pointer_array_initialize(&self->backend, initial_capacity, &configuration)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

void dx_inline_object_array_uninitialize(dx_inline_object_array* self) {
  dx_inline_pointer_array_uninitialize(&self->backend);
}

dx_result dx_inline_object_array_increase_capacity(dx_inline_object_array* self, dx_size additional_capacity)  {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_increase_capacity(&self->backend, additional_capacity);
}

dx_result dx_inline_object_array_ensure_free_capacity(dx_inline_object_array* self, dx_size required_free_capacity) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_ensure_free_capacity(&self->backend, required_free_capacity);
}

dx_result dx_inline_object_array_append(dx_inline_object_array* self, dx_object *pointer) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_append(&self->backend, pointer);
}

dx_result dx_inline_object_array_prepend(dx_inline_object_array* self, dx_object *pointer) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_prepend(&self->backend, pointer);
}

dx_result dx_inline_object_array_insert(dx_inline_object_array* self, dx_object *pointer, dx_size index) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_insert(&self->backend, pointer, index);
}

dx_result dx_inline_object_array_get_at(dx_object** RETURN, dx_inline_object_array const* SELF, dx_size index) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_get_at(RETURN, &SELF->backend, index);
}

dx_result dx_inline_object_array_get_size(dx_size* RETURN, dx_inline_object_array const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_get_size(RETURN, &SELF->backend);
}

dx_result dx_inline_object_array_get_capacity(dx_size* RETURN, dx_inline_object_array const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_get_capacity(RETURN, &SELF->backend);
}

dx_result dx_inline_object_array_get_free_capacity(dx_size* RETURN, dx_inline_object_array const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_get_free_capacity(RETURN, &SELF->backend);
}

dx_result dx_inline_object_array_clear(dx_inline_object_array* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_clear(&SELF->backend);
}

dx_result dx_inline_object_array_pop_back_n(dx_inline_object_array* SELF, dx_size n) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return dx_inline_pointer_array_pop_back_n(&SELF->backend, n);
}
