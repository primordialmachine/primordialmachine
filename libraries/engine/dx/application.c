#include "dx/application.h"


#if Core_OperatingSystem_Windows == Core_OperatingSystem
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
                      Core_Object);

static Core_Result update(dx_application* SELF);

static Core_Result startup_systems(dx_application* SELF);

static Core_Result shutdown_systems(dx_application* SELF);

static Core_Result get_val_context(dx_val_context** RETURN, dx_application* SELF);

static Core_Result get_aal_context(dx_aal_context** RETURN, dx_application* SELF);

static Core_Result get_assets_context(dx_assets_context** RETURN, dx_application* SELF);

static void dx_application_destruct(dx_application* SELF) {
  DX_UNREFERENCE(SELF->assets_system);
  SELF->assets_system = NULL;

  DX_UNREFERENCE(SELF->aal_system);
  SELF->aal_system = NULL;
  
  DX_UNREFERENCE(SELF->val_system);
  SELF->val_system = NULL;
  
  g_application = NULL; // Singleton guard.
}

static void dx_application_constructDispatch(dx_application_dispatch* SELF) {
  DX_APPLICATION_DISPATCH(SELF)->startup_systems = (Core_Result(*)(dx_application*)) & startup_systems;
  DX_APPLICATION_DISPATCH(SELF)->shutdown_systems = (Core_Result(*)(dx_application*)) & shutdown_systems;
  DX_APPLICATION_DISPATCH(SELF)->update = (Core_Result(*)(dx_application*))&update;
  DX_APPLICATION_DISPATCH(SELF)->get_val_context = (Core_Result(*)(dx_val_context**, dx_application*)) & get_val_context;
  DX_APPLICATION_DISPATCH(SELF)->get_aal_context = (Core_Result(*)(dx_aal_context**, dx_application*)) & get_aal_context;
  DX_APPLICATION_DISPATCH(SELF)->get_assets_context = (Core_Result(*)(dx_assets_context**, dx_application*)) & get_assets_context;
}

static Core_Result update(dx_application* SELF) {
#if Core_OperatingSystem_Windows == Core_OperatingSystem
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
  return Core_Error_NoError != Core_getError() ? Core_Failure : Core_Success;
#else
  #error("environment not (yet) supported")
#endif
}

static Core_Result startup_systems(dx_application* SELF) {
  if (dx_system_startup(DX_SYSTEM(SELF->val_system))) {
    return Core_Failure;
  }
  if (dx_system_startup(DX_SYSTEM(SELF->aal_system))) {
    dx_system_shutdown(DX_SYSTEM(SELF->val_system));
    return Core_Failure;
  }
  if (dx_system_startup(DX_SYSTEM(SELF->assets_system))) {
    dx_system_shutdown(DX_SYSTEM(SELF->aal_system));
    dx_system_shutdown(DX_SYSTEM(SELF->val_system));
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown_systems(dx_application* SELF) {
  if (dx_system_shutdown(DX_SYSTEM(SELF->assets_system))) {
    return Core_Failure;
  }
  if (dx_system_shutdown(DX_SYSTEM(SELF->aal_system))) {
    return Core_Failure;
  }
  if (dx_system_shutdown(DX_SYSTEM(SELF->val_system))) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result get_val_context(dx_val_context** RETURN, dx_application* SELF) {
  dx_val_context* context = NULL;
  if (dx_val_system_get_context(&context, SELF->val_system)) {
    return Core_Failure;
  }
  *RETURN = context;
  return Core_Success;
}

static Core_Result get_aal_context(dx_aal_context** RETURN, dx_application* SELF) {
  dx_aal_context* context = NULL;
  if (dx_aal_system_get_context(&context, SELF->aal_system)) {
    return Core_Failure;
  }
  *RETURN = context;
  return Core_Success;
}

static Core_Result get_assets_context(dx_assets_context** RETURN, dx_application* SELF) {
  dx_assets_context* context = NULL;
  if (dx_assets_system_get_context(&context, SELF->assets_system)) {
    return Core_Failure;
  }
  *RETURN = context;
  return Core_Success;
}

Core_Result dx_application_construct
  (
    dx_application* SELF,
    dx_val_system_factory* val_system_factory,
    dx_aal_system_factory* aal_system_factory,
    dx_assets_system_factory* assets_system_factory,
    dx_msg_queue* msg_queue
  )
{
  DX_CONSTRUCT_PREFIX(dx_application);
  //
  SELF->msg_queue = msg_queue;
  //
  if (dx_system_factory_create_system((dx_system**)&SELF->val_system, DX_SYSTEM_FACTORY(val_system_factory), msg_queue)) {
    return Core_Failure;
  }
  //
  if (dx_system_factory_create_system((dx_system**)&SELF->aal_system, DX_SYSTEM_FACTORY(aal_system_factory), msg_queue)) {
    DX_UNREFERENCE(SELF->val_system);
    SELF->val_system = NULL;
    return Core_Failure;
  }
  //
  if (dx_system_factory_create_system((dx_system**)&SELF->assets_system, DX_SYSTEM_FACTORY(assets_system_factory), msg_queue)) {
    DX_UNREFERENCE(SELF->aal_system);
    SELF->aal_system = NULL;
    DX_UNREFERENCE(SELF->val_system);
    SELF->val_system = NULL;
    return Core_Failure;
  }
  //
  CORE_OBJECT(SELF)->type = TYPE;
  //
  g_application = SELF; // Singleton guard.
  //
  return Core_Success;
}

Core_Result dx_application_create
  (
    dx_application** RETURN,
    dx_val_system_factory* val_system_factory,
    dx_aal_system_factory* aal_system_factory,
    dx_assets_system_factory* assets_system_factory,
    dx_msg_queue* msg_queue
  )
{
  DX_CREATE_PREFIX(dx_application);
  if (dx_application_construct(SELF, val_system_factory, aal_system_factory, assets_system_factory, msg_queue)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_application_emit_quit_msg(dx_application* SELF) {
  Core_Message* msg = NULL;
  if (Core_ApplicationMessage_create((Core_ApplicationMessage**)&msg, Core_ApplicationMessageKind_QuitRequested)) {
    return Core_Failure;
  }
  if (dx_msg_queue_push(SELF->msg_queue, msg)) {
    DX_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(msg);
  msg = NULL;
  return Core_Success;
}

Core_Result dx_application_get(dx_application** RETURN) {
  if (!RETURN) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (!g_application) {
    Core_setError(Core_Error_NotInitialized);
    return Core_Failure;
  }
  DX_REFERENCE(g_application);
  *RETURN = g_application;
  return Core_Success;
}
