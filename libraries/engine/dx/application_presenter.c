#include "dx/application_presenter.h"

DX_DEFINE_OBJECT_TYPE("dx.application_presenter",
                      dx_application_presenter,
                      dx_object);

static void dx_application_presenter_destruct(dx_application_presenter* SELF)
{/*Intentionally empty.*/}

static void dx_application_presenter_dispatch_construct(dx_application_presenter_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_application_presenter_construct(dx_application_presenter* SELF) {
  dx_rti_type* TYPE = dx_application_presenter_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}
