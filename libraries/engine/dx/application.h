#if !defined(DX_APPLICATION_H_INCLUDED)
#define DX_APPLICATION_H_INCLUDED

#include "dx/core.h"

/// The representation of an application.
DX_DECLARE_OBJECT_TYPE("dx.application",
                       dx_application,
                       dx_object);

static dx_application* DX_APPLICATION(void* p) {
  return (dx_application*)p;
}

struct dx_application {
  dx_object _parent;
};

static dx_application_dispatch* DX_APPLICATION_DISPATCH(void* p) {
  return (dx_application_dispatch*)p;
}

struct dx_application_dispatch {
  dx_object_dispatch _parent;
  dx_result (*update)(dx_application* SELF);
};

dx_result dx_application_construct(dx_application* SELF);

static inline dx_result dx_application_update(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, update, SELF);
}

#endif // DX_APPLICATION_H_INCLUDED
