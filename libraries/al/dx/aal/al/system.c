#include "dx/aal/al/system.h"

#include "dx/aal/al/context.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_aal_al_context* g_context = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.aal.al.system",
                      dx_aal_al_system,
                      dx_aal_system);

static dx_result startup(dx_aal_al_system* SELF);

static dx_result shutdown(dx_aal_al_system* SELF);

static dx_result get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF);

static void dx_aal_al_system_destruct(dx_aal_al_system* SELF)
{/*Intentionally empty.*/}

static void dx_aal_al_system_dispatch_construct(dx_aal_al_system_dispatch* SELF) {
  DX_AAL_SYSTEM_DISPATCH(SELF)->get_context = (dx_result(*)(dx_aal_context**, dx_aal_system*)) & get_context;
  DX_SYSTEM_DISPATCH(SELF)->shutdown = (dx_result(*)(dx_system*)) & shutdown;
  DX_SYSTEM_DISPATCH(SELF)->startup = (dx_result(*)(dx_system*)) & startup;
}

static dx_result startup(dx_aal_al_system* SELF) {
  if (dx_aal_al_context_create(&g_context)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result shutdown(dx_aal_al_system* SELF) {
  DX_UNREFERENCE(g_context);
  g_context = NULL;
  return DX_SUCCESS;
}

static dx_result get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}

dx_result dx_aal_al_system_construct(dx_aal_al_system* SELF, dx_msg_queue* msg_queue) {
  dx_rti_type* TYPE = dx_aal_al_system_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_aal_system_construct(DX_AAL_SYSTEM(SELF), msg_queue)) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_aal_al_system_create(dx_aal_al_system** RETURN, dx_msg_queue* msg_queue) {
  dx_aal_al_system* SELF = DX_AAL_AL_SYSTEM(dx_object_alloc(sizeof(dx_aal_al_system)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_aal_al_system_construct(SELF, msg_queue)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_aal_al_system_get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF) {
  if (!g_context) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_context);
  *RETURN = g_context;
  return DX_SUCCESS;
}
