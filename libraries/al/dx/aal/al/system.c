#include "dx/aal/al/system.h"

#include "dx/aal/al/context.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alext.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_aal_al_context* g_context = NULL;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_defineObjectType("dx.aal.al.system",
                      dx_aal_al_system,
                      Core_Audials_System);

static Core_Result startup(dx_aal_al_system* SELF);

static Core_Result shutdown(dx_aal_al_system* SELF);

static Core_Result get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF);

static void dx_aal_al_system_destruct(dx_aal_al_system* SELF)
{/*Intentionally empty.*/}

static void dx_aal_al_system_constructDispatch(dx_aal_al_system_Dispatch* SELF) {
  CORE_AUDIALS_SYSTEM_DISPATCH(SELF)->getContext = (Core_Result(*)(Core_Audials_Context**, Core_Audials_System*)) & get_context;
  CORE_SYSTEM_DISPATCH(SELF)->shutdown = (Core_Result(*)(Core_System*)) & shutdown;
  CORE_SYSTEM_DISPATCH(SELF)->startup = (Core_Result(*)(Core_System*)) & startup;
}

static Core_Result startup(dx_aal_al_system* SELF) {
  SELF->device = alcOpenDevice(NULL);
  if (!SELF->device) {
    dx_log("unable to create OpenAL device\n", sizeof("unable to create OpenAL device\n") - 1);
    return Core_Failure;
  }
  if (dx_aal_al_context_create(&g_context, SELF)) {
    if (!alcCloseDevice(SELF->device)) {
      dx_log("unable to destroy OpenAL device\n", sizeof("unable to destroy OpenAL device\n") - 1);
    }
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown(dx_aal_al_system* SELF) {
  if (!alcCloseDevice(SELF->device)) {
    dx_log("unable to destroy OpenAL device\n", sizeof("unable to destroy OpenAL device\n") - 1);
  }
  CORE_UNREFERENCE(g_context);
  g_context = NULL;
  return Core_Success;
}

static Core_Result get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF) {
  if (!g_context) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  CORE_REFERENCE(g_context);
  *RETURN = g_context;
  return Core_Success;
}

Core_Result dx_aal_al_system_construct(dx_aal_al_system* SELF, Core_MessageQueue* msg_queue) {
  DX_CONSTRUCT_PREFIX(dx_aal_al_system);
  if (Core_Audials_System_construct(CORE_AUDIALS_SYSTEM(SELF), msg_queue)) {
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_aal_al_system_create(dx_aal_al_system** RETURN, Core_MessageQueue* msg_queue) {
  DX_CREATE_PREFIX(dx_aal_al_system);
  if (dx_aal_al_system_construct(SELF, msg_queue)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_aal_al_system_get_context(dx_aal_al_context** RETURN, dx_aal_al_system* SELF) {
  if (!g_context) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  CORE_REFERENCE(g_context);
  *RETURN = g_context;
  return Core_Success;
}
