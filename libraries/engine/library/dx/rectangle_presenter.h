#if !defined(DX_RECTANGLE_PRESENTER_H_INCLUDED)
#define DX_RECTANGLE_PRESENTER_H_INCLUDED

#include "dx/presenter.h"
#include "dx/val/buffer.h"
#include "dx/val/material.h"

Core_declareObjectType("dx.rectangle_presenter",
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

static inline dx_rectangle_presenter_Dispatch* DX_RECTANGLE_PRESENTER_DISPATCH(void* p) {
  return (dx_rectangle_presenter_Dispatch*)p;
}

struct dx_rectangle_presenter_Dispatch {
  dx_presenter_Dispatch _parent;
};

Core_Result dx_rectangle_presenter_construct(dx_rectangle_presenter* SELf, Core_Visuals_Context* val_context, Core_Audials_Context* aal_context);

Core_Result dx_rectangle_presenter_create(dx_rectangle_presenter** RETURN, Core_Visuals_Context* val_context, Core_Audials_Context* aal_context);

Core_Result dx_rectangle_presenter_fill_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color);

Core_Result dx_rectangle_presenter_stroke_rectangle(dx_rectangle_presenter* SELF, DX_RECT2_F32 const* target_rectangle, Core_Real32 target_depth, Core_InlineRgbaR32 const* color);

#endif // DX_RECTANGLE_PRESENTER_H_INCLUDED
