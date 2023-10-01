#include "dx/assets/mesh_operation.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_ENUMERATION_TYPE("dx.asset.mesh_operation_kind",
                           dx_asset_mesh_operation_kind);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.mesh_operation",
                      dx_asset_mesh_operation,
                      dx_object);

static void dx_asset_mesh_operation_destruct(dx_asset_mesh_operation* SELF)
{/*Intentionally empty.*/}

static void dx_asset_mesh_operation_dispatch_construct(dx_asset_mesh_operation_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_mesh_operation_construct(dx_asset_mesh_operation* SELF, dx_asset_mesh_operation_kind kind) {
  dx_rti_type* TYPE = dx_asset_mesh_operation_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  SELF->kind = kind;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
