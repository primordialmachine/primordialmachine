#include "dx/val/viewer.h"

#include "dx/assets.h"

DX_DEFINE_OBJECT_TYPE("dx.val.viewer",
                      dx_val_viewer,
                      Core_Object);

static void dx_val_viewer_destruct(dx_val_viewer* SELF) {
  DX_UNREFERENCE(SELF->asset_viewer_instance);
  SELF->asset_viewer_instance = NULL;
}

static void dx_val_viewer_constructDispatch(dx_val_viewer_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_viewer_construct(dx_val_viewer* SELF, dx_assets_viewer_instance* asset_viewer_instance) {
  DX_CONSTRUCT_PREFIX(dx_val_viewer);
  SELF->source = DX_ASSETS_VIEWER(asset_viewer_instance->viewer_reference->object)->source;
  SELF->target = DX_ASSETS_VIEWER(asset_viewer_instance->viewer_reference->object)->target;
  SELF->up = DX_ASSETS_VIEWER(asset_viewer_instance->viewer_reference->object)->up;
  SELF->asset_viewer_instance = asset_viewer_instance;
  dx_mat4_set_identity(&SELF->view_matrix);
  dx_mat4_set_identity(&SELF->projection_matrix);
  DX_REFERENCE(asset_viewer_instance);
  CORE_OBJECT(SELF)->type = _type;
  return Core_Success;
}

Core_Result dx_val_viewer_create(dx_val_viewer** RETURN, dx_assets_viewer_instance* asset_viewer_instance) {
  DX_CREATE_PREFIX(dx_val_viewer);
  if (dx_val_viewer_construct(SELF, asset_viewer_instance)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_val_viewer_get_projection_matrix(DX_MAT4* RETURN, dx_val_viewer* SELF, Core_Integer32 canvas_width, Core_Integer32 canvas_height) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  dx_assets_optics* optics = DX_ASSETS_VIEWER(SELF->asset_viewer_instance->viewer_reference->object)->optics;
  if (!optics) {
    return Core_Failure;
  }
  Core_Type* type = NULL;
  if (dx_asset_optics_perspective_getType(&type)) {
    return Core_Failure;
  }
  if (dx_rti_type_is_leq(CORE_OBJECT(optics)->type, type)) {
    dx_asset_optics_perspective* optics1 = DX_ASSET_OPTICS_PERSPECTIVE(optics);
    // use actual aspect ratio
    if (optics1->aspect_ratio) {
      Core_Memory_deallocate(optics1->aspect_ratio);
      optics1->aspect_ratio = NULL;
    }
    Core_Real32 aspect_ratio = (Core_Real32)canvas_width / (Core_Real32)canvas_height;
    if (optics1->aspect_ratio) {
      aspect_ratio = *optics1->aspect_ratio;
    }
    dx_mat4_set_perspective(&SELF->projection_matrix, optics1->field_of_view_y, aspect_ratio, optics1->near, optics1->far);
    *RETURN = SELF->projection_matrix;
    return Core_Success;
  }
  
  if (dx_asset_optics_orthographic_getType(&type)) {
    return Core_Failure;
  }
  if (dx_rti_type_is_leq(CORE_OBJECT(optics)->type, type)) {
    dx_asset_optics_orthographic* optics1 = DX_ASSET_OPTICS_ORTHOGRAPHIC(optics);
    Core_Real32 left = -1.f;
    Core_Real32 right = +1.f;
    if (optics1->scale_x) {
      left *= *optics1->scale_x;
      right *= *optics1->scale_x;
    }
    Core_Real32 bottom = -1.f;
    Core_Real32 top = +1.f;
    if (optics1->scale_y) {
      bottom *= *optics1->scale_y;
      top *= *optics1->scale_y;
    }
    dx_mat4_set_ortho(&SELF->projection_matrix, left, right, bottom, top, optics1->near, optics1->far);
    *RETURN = SELF->projection_matrix;
    return Core_Success;
  }

  Core_setError(Core_Error_SemanticalError);
  return Core_Failure;
}

Core_Result dx_val_viewer_get_view_matrix(DX_MAT4* RETURN, dx_val_viewer* SELF, Core_Integer32 canvas_width, Core_Integer32 canvas_height) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  dx_mat4_set_look_at(&SELF->view_matrix, &SELF->source, &SELF->target, &SELF->up);
  *RETURN = SELF->view_matrix;
  return Core_Success;
}

