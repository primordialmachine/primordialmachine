#if !defined(DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.viewer_instance",
                       dx_adl_type_handlers_viewer_instance,
                       dx_adl_type_handler);

static inline dx_adl_type_handlers_viewer_instance* DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE(void* p) {
  return (dx_adl_type_handlers_viewer_instance*)p;
}

struct dx_adl_type_handlers_viewer_instance {
  dx_adl_type_handler _parent;
};

struct dx_adl_type_handlers_viewer_instance_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

int dx_adl_type_handlers_viewer_instance_construct(dx_adl_type_handlers_viewer_instance* self);

dx_adl_type_handlers_viewer_instance* dx_adl_type_handlers_viewer_instance_create();

#endif // DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE_H_INCLUDED
