#include "dx/application_presenter.h"

Core_defineObjectType("dx.application_presenter",
                      dx_application_presenter,
                      Core_Object);

static void dx_application_presenter_destruct(dx_application_presenter* SELF)
{/*Intentionally empty.*/}

static void dx_application_presenter_constructDispatch(dx_application_presenter_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_application_presenter_construct(dx_application_presenter* SELF) {
  DX_CONSTRUCT_PREFIX(dx_application_presenter);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}