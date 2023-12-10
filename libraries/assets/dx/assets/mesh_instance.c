#include "dx/assets/mesh_instance.h"

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_instance",
                      dx_asset_mesh_instance,
                      Core_Object);

static void dx_asset_mesh_instance_destruct(dx_asset_mesh_instance* SELF) {
  DX_UNREFERENCE(SELF->world_matrix);
  SELF->world_matrix = NULL;
  DX_UNREFERENCE(SELF->mesh_reference);
  SELF->mesh_reference = NULL;
}

static void dx_asset_mesh_instance_constructDispatch(dx_asset_mesh_instance_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_mesh_instance_construct(dx_asset_mesh_instance* SELF, dx_asset_reference* mesh_reference) {
  DX_CONSTRUCT_PREFIX(dx_asset_mesh_instance);

  if (!mesh_reference) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->mesh_reference = mesh_reference;
  DX_REFERENCE(SELF->mesh_reference);

  DX_MAT4 temporary;
  dx_mat4_set_identity(&temporary);
  if (dx_assets_matrix_4x4_f32_create(&SELF->world_matrix, &temporary)) {
    DX_UNREFERENCE(SELF->mesh_reference);
    SELF->mesh_reference = NULL;
    return Core_Failure;
  }

  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_mesh_instance_create(dx_asset_mesh_instance** RETURN, dx_asset_reference* mesh_reference) {
  DX_CREATE_PREFIX(dx_asset_mesh_instance);
  if (dx_asset_mesh_instance_construct(SELF, mesh_reference)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}
