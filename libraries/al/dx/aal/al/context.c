#include "dx/aal/al/context.h"

DX_DEFINE_OBJECT_TYPE("dx.aal.al.context",
                      dx_aal_al_context,
                      dx_aal_context);

static void dx_aal_al_context_destruct(dx_aal_al_context* SELF)
{/*Intentionally empty.*/}

static void dx_aal_al_context_dispatch_construct(dx_aal_al_context_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_aal_al_context_construct(dx_aal_al_context * SELF) {
  dx_rti_type* TYPE = dx_aal_al_context_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_aal_context_construct(DX_AAL_CONTEXT(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_aal_al_context_create(dx_aal_al_context** RETURN) {
  dx_aal_al_context* SELF = DX_AAL_AL_CONTEXT(dx_object_alloc(sizeof(dx_aal_al_context)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_aal_al_context_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
