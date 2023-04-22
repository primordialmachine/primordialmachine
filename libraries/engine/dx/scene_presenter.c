#include "dx/scene_presenter.h"

DX_DEFINE_OBJECT_TYPE("dx.scene_presenter",
                      dx_scene_presenter,
                      dx_object);

static void dx_scene_presenter_destruct(dx_scene_presenter* SELF)
{/*Intentionally empty.*/}

static void dx_scene_presenter_dispatch_construct(dx_scene_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_scene_presenter_construct(dx_scene_presenter* SELF) {
  dx_rti_type* TYPE = dx_scene_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
