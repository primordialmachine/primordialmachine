#if !defined(DX_MOUSE_STATE_H_INCLUDED)
#define DX_MOUSE_STATE_H_INCLUDED

#include "dx/core/object.h"
#include "dx/core/hapticals.h"

DX_DECLARE_OBJECT_TYPE("dx.mouse_state",
                       dx_mouse_state,
                       dx_object);

static inline dx_mouse_state* DX_MOUSE_STATE(void* p) {
  return (dx_mouse_state*)p;
}

struct dx_mouse_state {
  dx_object _parent;
};

static inline dx_mouse_state_dispatch* DX_MOUSE_STATE_DISPATCH(void* p) {
  return (dx_mouse_state_dispatch*)p;
}

struct dx_mouse_state_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_mouse_state_construct(dx_mouse_state* SELF);

dx_result dx_mouse_state_create(dx_mouse_state** RETURN);

#endif // DX_MOUSE_STATE_H_INCLUDED
