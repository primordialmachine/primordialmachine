#include "dx/scene_presenter.h"

Core_defineObjectType("dx.scene_presenter",
                      dx_scene_presenter,
                      Core_Object);

static void dx_scene_presenter_destruct(dx_scene_presenter* SELF)
{/*Intentionally empty.*/}

static void dx_scene_presenter_constructDispatch(dx_scene_presenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_scene_presenter_construct(dx_scene_presenter* SELF) {
  DX_CONSTRUCT_PREFIX(dx_scene_presenter);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}
