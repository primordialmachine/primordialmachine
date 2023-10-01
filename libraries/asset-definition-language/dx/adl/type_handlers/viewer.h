#if !defined(DX_ADL_TYPE_HANDLERS_VIEWER_H_INCLUDED)
#define DX_ADL_TYPE_HANDLERS_VIEWER_H_INCLUDED

#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.type_handlers.viewer_reader",
                       dx_adl_type_handlers_viewer,
                       dx_adl_type_handler);
                       
static inline dx_adl_type_handlers_viewer* DX_ADL_TYPE_HANDLERS_VIEWER(void* p) {
  return (dx_adl_type_handlers_viewer*)p;
}

struct dx_adl_type_handlers_viewer {
  dx_adl_type_handler _parent;
};

static inline dx_adl_type_handlers_viewer_dispatch* DX_ADL_TYPE_HANDLERS_VIEWER_DISPATCH(void* p) {
  return (dx_adl_type_handlers_viewer_dispatch*)p;
}

struct dx_adl_type_handlers_viewer_dispatch {
  dx_adl_type_handler_dispatch _parent;
};

dx_result dx_adl_type_handlers_viewer_construct(dx_adl_type_handlers_viewer* SELF);

dx_result dx_adl_type_handlers_viewer_create(dx_adl_type_handlers_viewer** RETURN);

#endif // DX_ADL_TYPE_HANDLERS_VIEWER_H_INCLUDED
