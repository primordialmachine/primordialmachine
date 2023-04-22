#if !defined(DX_VAL_BUFFER_H_INCLUDED)
#define DX_VAL_BUFFER_H_INCLUDED

#include "dx/val/context.h"

DX_DECLARE_OBJECT_TYPE("dx.val.buffer",
                       dx_val_buffer,
                       dx_object);

static inline dx_val_buffer* DX_VAL_BUFFER(void* p) {
  return (dx_val_buffer*)p;
}

struct dx_val_buffer {
  dx_object _parent;
  dx_val_context* context;
};

static inline dx_val_buffer_dispatch* DX_VAL_BUFFER_DISPATCH(void* p) {
  return (dx_val_buffer_dispatch*)p;
}

struct dx_val_buffer_dispatch {
  dx_object_dispatch _parent;
  dx_result(*set_data)(dx_val_buffer*, void const*, dx_size);
};

dx_result dx_val_buffer_construct(dx_val_buffer* SELF, dx_val_context* context);

static inline dx_result dx_val_buffer_set_data(dx_val_buffer* SELF, void const* p, dx_size n) {
  DX_OBJECT_VIRTUALCALL(dx_val_buffer, set_data, SELF, p, n);
}

#endif // DX_VAL_BUFFER_H_INCLUDED
