#include "dx/assets/mesh_operation.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineEnumerationType("dx.asset.mesh_operation_kind",
                           dx_asset_mesh_operation_kind);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.asset.mesh_operation",
                      dx_asset_mesh_operation,
                      Core_Object);

static void dx_asset_mesh_operation_destruct(dx_asset_mesh_operation* SELF)
{/*Intentionally empty.*/}

static void dx_asset_mesh_operation_constructDispatch(dx_asset_mesh_operation_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_mesh_operation_construct(dx_asset_mesh_operation* SELF, dx_asset_mesh_operation_kind kind) {
  DX_CONSTRUCT_PREFIX(dx_asset_mesh_operation);
  SELF->kind = kind;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
