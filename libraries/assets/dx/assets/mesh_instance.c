#include "dx/assets/mesh_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_instance",
                      dx_asset_mesh_instance,
                      dx_object);

static void dx_asset_mesh_instance_destruct(dx_asset_mesh_instance* SELF) {
  DX_UNREFERENCE(SELF->world_matrix);
  SELF->world_matrix = NULL;
  DX_UNREFERENCE(SELF->mesh_reference);
  SELF->mesh_reference = NULL;
}

static void dx_asset_mesh_instance_dispatch_construct(dx_asset_mesh_instance_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_mesh_instance_construct(dx_asset_mesh_instance* SELF, dx_asset_reference* mesh_reference) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_asset_mesh_instance_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }

  if (!mesh_reference) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->mesh_reference = mesh_reference;
  DX_REFERENCE(SELF->mesh_reference);

  DX_MAT4 temporary;
  dx_mat4_set_identity(&temporary);
  if (dx_assets_matrix_4x4_f32_create(&SELF->world_matrix, &temporary)) {
    DX_UNREFERENCE(SELF->mesh_reference);
    SELF->mesh_reference = NULL;
    return DX_FAILURE;
  }

  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_mesh_instance_create(dx_asset_mesh_instance** RETURN, dx_asset_reference* mesh_reference) {
  DX_CREATE_PREFIX(dx_asset_mesh_instance)
  if (dx_asset_mesh_instance_construct(SELF, mesh_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
