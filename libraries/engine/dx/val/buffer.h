#if !defined(DX_VAL_BUFFER_H_INCLUDED)
#define DX_VAL_BUFFER_H_INCLUDED

#include "dx/val/context.h"

Core_declareObjectType("dx.val.buffer",
                       dx_val_buffer,
                       Core_Object);

static inline dx_val_buffer* DX_VAL_BUFFER(void* p) {
  return (dx_val_buffer*)p;
}

struct dx_val_buffer {
  Core_Object _parent;
  dx_val_context* context;
};

static inline dx_val_buffer_Dispatch* DX_VAL_BUFFER_DISPATCH(void* p) {
  return (dx_val_buffer_Dispatch*)p;
}

struct dx_val_buffer_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*set_data)(dx_val_buffer*, void const*, Core_Size);
};

Core_Result dx_val_buffer_construct(dx_val_buffer* SELF, dx_val_context* context);

static inline Core_Result dx_val_buffer_set_data(dx_val_buffer* SELF, void const* p, Core_Size n) {
  DX_OBJECT_VIRTUALCALL(dx_val_buffer, set_data, SELF, p, n);
}

#endif // DX_VAL_BUFFER_H_INCLUDED
