#include "dx/application.h"


#if DX_OPERATING_SYSTEM_WINDOWS == DX_OPERATING_SYSTEM
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("environment not (yet) supported")
#endif


#include "dx/val/system.h"
#include "dx/aal/system.h"
#include "dx/assets/system.h"


static dx_application* g_application = NULL;

DX_DEFINE_OBJECT_TYPE("dx.application",
                      dx_application,
                      dx_object);

static dx_result update(dx_application* SELF);

static dx_result startup_systems(dx_application* SELF);

static dx_result shutdown_systems(dx_application* SELF);

static dx_result get_val_context(dx_val_context** RETURN, dx_application* SELF);

static dx_result get_aal_context(dx_aal_context** RETURN, dx_application* SELF);

static dx_result get_assets_context(dx_assets_context** RETURN, dx_application* SELF);

static void dx_application_destruct(dx_application* SELF) {
  DX_UNREFERENCE(SELF->assets_system);
  SELF->assets_system = NULL;

  DX_UNREFERENCE(SELF->aal_system);
  SELF->aal_system = NULL;
  
  DX_UNREFERENCE(SELF->val_system);
  SELF->val_system = NULL;
  
  g_application = NULL; // Singleton guard.
}

static void dx_application_dispatch_construct(dx_application_dispatch* SELF) {
  DX_APPLICATION_DISPATCH(SELF)->startup_systems = (dx_result(*)(dx_application*)) & startup_systems;
  DX_APPLICATION_DISPATCH(SELF)->shutdown_systems = (dx_result(*)(dx_application*)) & shutdown_systems;
  DX_APPLICATION_DISPATCH(SELF)->update = (dx_result(*)(dx_application*))&update;
  DX_APPLICATION_DISPATCH(SELF)->get_val_context = (dx_result(*)(dx_val_context**, dx_application*)) & get_val_context;
  DX_APPLICATION_DISPATCH(SELF)->get_aal_context = (dx_result(*)(dx_aal_context**, dx_application*)) & get_aal_context;
  DX_APPLICATION_DISPATCH(SELF)->get_assets_context = (dx_result(*)(dx_assets_context**, dx_application*)) & get_assets_context;
}

static dx_result update(dx_application* SELF) {
#if DX_OPERATING_SYSTEM_WINDOWS == DX_OPERATING_SYSTEM
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      dx_application_emit_quit_msg(DX_APPLICATION(SELF));
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  // If an error is pending, then this function fails.
  return DX_NO_ERROR != dx_get_error() ? DX_FAILURE : DX_SUCCESS;
#else
  #error("environment not (yet) supported")
#endif
}

static dx_result startup_systems(dx_application* SELF) {
  if (dx_system_startup(DX_SYSTEM(SELF->val_system))) {
    return DX_FAILURE;
  }
  if (dx_system_startup(DX_SYSTEM(SELF->aal_system))) {
    dx_system_shutdown(DX_SYSTEM(SELF->val_system));
    return DX_FAILURE;
  }
  if (dx_system_startup(DX_SYSTEM(SELF->assets_system))) {
    dx_system_shutdown(DX_SYSTEM(SELF->aal_system));
    dx_system_shutdown(DX_SYSTEM(SELF->val_system));
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result shutdown_systems(dx_application* SELF) {
  if (dx_system_shutdown(DX_SYSTEM(SELF->assets_system))) {
    return DX_FAILURE;
  }
  if (dx_system_shutdown(DX_SYSTEM(SELF->aal_system))) {
    return DX_FAILURE;
  }
  if (dx_system_shutdown(DX_SYSTEM(SELF->val_system))) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

static dx_result get_val_context(dx_val_context** RETURN, dx_application* SELF) {
  dx_val_context* context = NULL;
  if (dx_val_system_get_context(&context, SELF->val_system)) {
    return DX_FAILURE;
  }
  *RETURN = context;
  return DX_SUCCESS;
}

static dx_result get_aal_context(dx_aal_context** RETURN, dx_application* SELF) {
  dx_aal_context* context = NULL;
  if (dx_aal_system_get_context(&context, SELF->aal_system)) {
    return DX_FAILURE;
  }
  *RETURN = context;
  return DX_SUCCESS;
}

static dx_result get_assets_context(dx_assets_context** RETURN, dx_application* SELF) {
  dx_assets_context* context = NULL;
  if (dx_assets_system_get_context(&context, SELF->assets_system)) {
    return DX_FAILURE;
  }
  *RETURN = context;
  return DX_SUCCESS;
}

dx_result dx_application_construct
  (
    dx_application* SELF,
    dx_val_system_factory* val_system_factory,
    dx_aal_system_factory* aal_system_factory,
    dx_assets_system_factory* assets_system_factory,
    dx_msg_queue* msg_queue
  )
{
  dx_rti_type* TYPE = dx_application_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  //
  SELF->msg_queue = msg_queue;
  //
  if (dx_system_factory_create_system((dx_system**)&SELF->val_system, DX_SYSTEM_FACTORY(val_system_factory), msg_queue)) {
    return DX_FAILURE;
  }
  //
  if (dx_system_factory_create_system((dx_system**)&SELF->aal_system, DX_SYSTEM_FACTORY(aal_system_factory), msg_queue)) {
    DX_UNREFERENCE(SELF->val_system);
    SELF->val_system = NULL;
    return DX_FAILURE;
  }
  //
  if (dx_system_factory_create_system((dx_system**)&SELF->assets_system, DX_SYSTEM_FACTORY(assets_system_factory), msg_queue)) {
    DX_UNREFERENCE(SELF->aal_system);
    SELF->aal_system = NULL;
    DX_UNREFERENCE(SELF->val_system);
    SELF->val_system = NULL;
    return DX_FAILURE;
  }
  //
  DX_OBJECT(SELF)->type = TYPE;
  //
  g_application = SELF; // Singleton guard.
  //
  return DX_SUCCESS;
}

dx_result dx_application_create
  (
    dx_application** RETURN,
    dx_val_system_factory* val_system_factory,
    dx_aal_system_factory* aal_system_factory,
    dx_assets_system_factory* assets_system_factory,
    dx_msg_queue* msg_queue
  )
{
  DX_CREATE_PREFIX(dx_application)
  if (dx_application_construct(SELF, val_system_factory, aal_system_factory, assets_system_factory, msg_queue)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

dx_result dx_application_emit_quit_msg(dx_application* SELF) {
  dx_msg* msg = NULL;
  if (dx_quit_msg_create((dx_quit_msg**)&msg)) {
    return DX_FAILURE;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return DX_SUCCESS;
}

dx_result dx_application_get(dx_application** RETURN) {
  if (!RETURN) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (!g_application) {
    dx_set_error(DX_ERROR_NOT_INITIALIZED);
    return DX_FAILURE;
  }
  DX_REFERENCE(g_application);
  *RETURN = g_application;
  return DX_SUCCESS;
}
