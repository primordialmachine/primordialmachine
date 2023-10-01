#if !defined(DX_RECTANGLE_PRESENTER_H_INCLUDED)
#define DX_RECTANGLE_PRESENTER_H_INCLUDED

#include "dx/presenter.h"
#include "dx/val/buffer.h"
#include "dx/val/material.h"

DX_DECLARE_OBJECT_TYPE("dx.rectangle_presenter",
                       dx_rectangle_presenter,
                       dx_presenter);

static inline dx_rectangle_presenter* DX_RECTANGLE_PRESENTER(void* p) {
  return (dx_rectangle_presenter*)p;
}

struct dx_rectangle_presenter {
  dx_presenter _parent;
  /// the material to render the rectangle
  dx_val_material* val_material;
  // the program to render the rectangle
  dx_val_program* val_program;
  // the constant binding to render the rectangle
  dx_val_cbinding* val_cbinding;
  // the variable binding to render the rectangle
  dx_val_vbinding* val_vbinding;
  // the buffer to render the rectangle
  dx_val_buffer* val_buffer;
  // the command list to render the rectangle
  dx_val_command_list* val_command_list;
};

static inline dx_rectangle_presenter_dispatch* DX_RECTANGLE_PRESENTER_DISPATCH(void* p) {
  return (dx_rectangle_presenter_dispatch*)p;
}

struct dx_rectangle_presenter_dispatch {
  dx_presenter_dispatch _parent;
};

dx_result dx_rectangle_presenter_construct(dx_rectangle_presenter* SELf, dx_val_context* val_context, dx_aal_context* aal_context);

dx_result dx_rectangle_presenter_create(dx_rectangle_presenter** RETURN, dx_val_context* val_context, dx_aal_context* aal_context);

dx_result dx_rectangle_presenter_fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color);

dx_result dx_rectangle_presenter_stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, dx_f32 target_depth, DX_RGBA_F32 const* color);

#endif // DX_RECTANGLE_PRESENTER_H_INCLUDED
