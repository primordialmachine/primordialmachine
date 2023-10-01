#include "dx/val/viewer.h"

#include "dx/assets.h"

DX_DEFINE_OBJECT_TYPE("dx.val.viewer",
                      dx_val_viewer,
                      dx_object);

static void dx_val_viewer_destruct(dx_val_viewer* SELF) {
  DX_UNREFERENCE(SELF->asset_viewer_instance);
  SELF->asset_viewer_instance = NULL;
}

static void dx_val_viewer_dispatch_construct(dx_val_viewer_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_val_viewer_construct(dx_val_viewer* SELF, dx_asset_viewer_instance* asset_viewer_instance) {
  dx_rti_type* TYPE = dx_val_viewer_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->source = DX_ASSET_VIEWER(asset_viewer_instance->viewer_reference->object)->source;
  SELF->target = DX_ASSET_VIEWER(asset_viewer_instance->viewer_reference->object)->target;
  SELF->up = DX_ASSET_VIEWER(asset_viewer_instance->viewer_reference->object)->up;
  SELF->asset_viewer_instance = asset_viewer_instance;
  dx_mat4_set_identity(&SELF->view_matrix);
  dx_mat4_set_identity(&SELF->projection_matrix);
  DX_REFERENCE(asset_viewer_instance);
  DX_OBJECT(SELF)->type = _type;
  return DX_SUCCESS;
}

dx_result dx_val_viewer_create(dx_val_viewer** RETURN, dx_asset_viewer_instance* asset_viewer_instance) {
  DX_CREATE_PREFIX(dx_val_viewer)
  if (dx_val_viewer_construct(SELF, asset_viewer_instance)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_val_viewer_get_projection_matrix(DX_MAT4* RETURN, dx_val_viewer* SELF, dx_i32 canvas_width, dx_i32 canvas_height) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_asset_optics* optics = DX_ASSET_VIEWER(SELF->asset_viewer_instance->viewer_reference->object)->optics;
  if (!optics) {
    return DX_FAILURE;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(optics)->type, dx_asset_optics_perspective_get_type())) {
    dx_asset_optics_perspective* optics1 = DX_ASSET_OPTICS_PERSPECTIVE(optics);
    // use actual aspect ratio
    if (optics1->aspect_ratio) {
      dx_memory_deallocate(optics1->aspect_ratio);
      optics1->aspect_ratio = NULL;
    }
    dx_f32 aspect_ratio = (dx_f32)canvas_width / (dx_f32)canvas_height;
    if (optics1->aspect_ratio) {
      aspect_ratio = *optics1->aspect_ratio;
    }
    dx_mat4_set_perspective(&SELF->projection_matrix, optics1->field_of_view_y, aspect_ratio, optics1->near, optics1->far);
  } else if (dx_rti_type_is_leq(DX_OBJECT(optics)->type, dx_asset_optics_orthographic_get_type())) {
    dx_asset_optics_orthographic* optics1 = DX_ASSET_OPTICS_ORTHOGRAPHIC(optics);
    dx_f32 left = -1.f;
    dx_f32 right = +1.f;
    if (optics1->scale_x) {
      left *= *optics1->scale_x;
      right *= *optics1->scale_x;
    }
    dx_f32 bottom = -1.f;
    dx_f32 top = +1.f;
    if (optics1->scale_y) {
      bottom *= *optics1->scale_y;
      top *= *optics1->scale_y;
    }
    dx_mat4_set_ortho(&SELF->projection_matrix, left, right, bottom, top, optics1->near, optics1->far);
  } else {
    return DX_FAILURE;
  }
  *RETURN = SELF->projection_matrix;
  return DX_SUCCESS;
}

dx_result dx_val_viewer_get_view_matrix(DX_MAT4* RETURN, dx_val_viewer* SELF, dx_i32 canvas_width, dx_i32 canvas_height) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_mat4_set_look_at(&SELF->view_matrix, &SELF->source, &SELF->target, &SELF->up);
  *RETURN = SELF->view_matrix;
  return DX_SUCCESS;
}

