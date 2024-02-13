#include "dx/val/viewer.h"

#include "dx/assets.h"

Core_defineObjectType("dx.val.viewer",
                      dx_val_viewer,
                      Core_Object);

static void dx_val_viewer_destruct(dx_val_viewer* SELF) {
  CORE_UNREFERENCE(SELF->asset_viewer_instance);
  SELF->asset_viewer_instance = NULL;
}

static void dx_val_viewer_constructDispatch(dx_val_viewer_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_val_viewer_construct(dx_val_viewer* SELF, dx_assets_viewer_instance* asset_viewer_instance) {
  DX_CONSTRUCT_PREFIX(dx_val_viewer);
  SELF->source = CORE_ASSETS_VIEWER(asset_viewer_instance->viewer_reference->object)->source;
  SELF->target = CORE_ASSETS_VIEWER(asset_viewer_instance->viewer_reference->object)->target;
  SELF->up = CORE_ASSETS_VIEWER(asset_viewer_instance->viewer_reference->object)->up;
  SELF->asset_viewer_instance = asset_viewer_instance;
  dx_mat4_set_identity(&SELF->view_matrix);
  dx_mat4_set_identity(&SELF->projection_matrix);
  CORE_REFERENCE(asset_viewer_instance);
  CORE_OBJECT(SELF)->type = _type;
  return Core_Success;
}

Core_Result dx_val_viewer_create(dx_val_viewer** RETURN, dx_assets_viewer_instance* asset_viewer_instance) {
  DX_CREATE_PREFIX(dx_val_viewer);
  if (dx_val_viewer_construct(SELF, asset_viewer_instance)) {
    CORE_UNREFERENCE(SELF);
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
  Core_Assets_Optics* optics = CORE_ASSETS_VIEWER(SELF->asset_viewer_instance->viewer_reference->object)->optics;
  if (!optics) {
    return Core_Failure;
  }
  Core_Boolean result;
  Core_Type* type = NULL;
  //
  if (Core_Assets_PerspectiveOptics_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(optics)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    Core_Assets_PerspectiveOptics* optics1 = CORE_ASSETS_PERSPECTIVEOPTICS(optics);
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
  //
  if (Core_Assets_OrthographicOptics_getType(&type)) {
    return Core_Failure;
  }
  if (Core_Type_isLowerThanOrEqualTo(&result, CORE_OBJECT(optics)->type, type)) {
    return Core_Failure;
  }
  if (result) {
    Core_Assets_OrthographicOptics* optics1 = CORE_ASSETS_ORTHOGRAPHICOPTICS(optics);
    Core_Real32 left = -1.f;
    Core_Real32 right = +1.f;
    if (optics1->scaleX) {
      left *= *optics1->scaleX;
      right *= *optics1->scaleX;
    }
    Core_Real32 bottom = -1.f;
    Core_Real32 top = +1.f;
    if (optics1->scaleY) {
      bottom *= *optics1->scaleY;
      top *= *optics1->scaleY;
    }
    dx_mat4_set_ortho(&SELF->projection_matrix, left, right, bottom, top, optics1->near, optics1->far);
    *RETURN = SELF->projection_matrix;
    return Core_Success;
  }
  //
  Core_setError(Core_Error_SemanticalAnalysisFailed);
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

