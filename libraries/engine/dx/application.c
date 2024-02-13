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

#include "dx/font_manager.h"

#include "dx/rectangle_presenter.h"
#include "dx/font_presenter.h"


static dx_application* g_application = NULL;

Core_defineObjectType("dx.application",
                      dx_application,
                      Core_Object);

static Core_Result update(dx_application* SELF);

static Core_Result startup_secondary_systems(dx_application* SELF);

static Core_Result startup_systems(dx_application* SELF);

static Core_Result shutdown_secondary_systems(dx_application* SELF);

static Core_Result shutdown_systems(dx_application* SELF);

static Core_Result get_val_context(dx_val_context** RETURN, dx_application* SELF);

static Core_Result get_aal_context(dx_aal_context** RETURN, dx_application* SELF);

static Core_Result get_assets_context(Core_Assets_Context** RETURN, dx_application* SELF);

static void dx_application_destruct(dx_application* SELF) {
  CORE_UNREFERENCE(SELF->assets_system);
  SELF->assets_system = NULL;

  CORE_UNREFERENCE(SELF->aal_system);
  SELF->aal_system = NULL;
  
  CORE_UNREFERENCE(SELF->val_system);
  SELF->val_system = NULL;

  CORE_UNREFERENCE(SELF->configuration);
  SELF->configuration = NULL;
  
  g_application = NULL; // Singleton guard.
}

static void dx_application_constructDispatch(dx_application_Dispatch* SELF) {
  DX_APPLICATION_DISPATCH(SELF)->startup_systems = (Core_Result(*)(dx_application*)) & startup_systems;
  DX_APPLICATION_DISPATCH(SELF)->shutdown_systems = (Core_Result(*)(dx_application*)) & shutdown_systems;
  DX_APPLICATION_DISPATCH(SELF)->update = (Core_Result(*)(dx_application*))&update;
  DX_APPLICATION_DISPATCH(SELF)->get_val_context = (Core_Result(*)(dx_val_context**, dx_application*)) & get_val_context;
  DX_APPLICATION_DISPATCH(SELF)->get_aal_context = (Core_Result(*)(dx_aal_context**, dx_application*)) & get_aal_context;
  DX_APPLICATION_DISPATCH(SELF)->get_assets_context = (Core_Result(*)(Core_Assets_Context**, dx_application*)) & get_assets_context;
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

static Core_Result startup_secondary_systems(dx_application* SELF) {
  dx_val_context* val_context = NULL;
  if (dx_application_get_val_context(&val_context, g_application)) {
    return Core_Failure;
  }
  dx_aal_context* aal_context = NULL;
  if (dx_application_get_aal_context(&aal_context, g_application)) {
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_font_manager_create(&SELF->font_manager, val_context)) {
    CORE_UNREFERENCE(aal_context);
    aal_context = NULL;
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_rectangle_presenter_create(&SELF->rectangle_presenter, val_context, aal_context)) {
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    CORE_UNREFERENCE(aal_context);
    aal_context = NULL;
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  if (dx_font_presenter_create(&SELF->font_presenter, SELF->font_manager, SELF->rectangle_presenter)) {
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    CORE_UNREFERENCE(SELF->font_manager);
    SELF->font_manager = NULL;
    CORE_UNREFERENCE(aal_context);
    aal_context = NULL;
    CORE_UNREFERENCE(val_context);
    val_context = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(aal_context);
  aal_context = NULL;
  CORE_UNREFERENCE(val_context);
  val_context = NULL;
  return Core_Success;
}

static Core_Result startup_systems(dx_application* SELF) {
  if (Core_System_startup(CORE_SYSTEM(SELF->val_system))) {
    return Core_Failure;
  }
  if (Core_System_startup(CORE_SYSTEM(SELF->aal_system))) {
    Core_System_shutdown(CORE_SYSTEM(SELF->val_system));
    return Core_Failure;
  }
  if (Core_System_startup(CORE_SYSTEM(SELF->assets_system))) {
    Core_System_shutdown(CORE_SYSTEM(SELF->aal_system));
    Core_System_shutdown(CORE_SYSTEM(SELF->val_system));
    return Core_Failure;
  }
  if (startup_secondary_systems(SELF)) {
    Core_System_shutdown(CORE_SYSTEM(SELF->assets_system));
    Core_System_shutdown(CORE_SYSTEM(SELF->aal_system));
    Core_System_shutdown(CORE_SYSTEM(SELF->val_system));
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown_secondary_systems(dx_application* SELF) {
  CORE_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;
  CORE_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;
  CORE_UNREFERENCE(SELF->font_manager);
  SELF->font_manager = NULL;
  return Core_Success;
}

static Core_Result shutdown_systems(dx_application* SELF) {
  if (shutdown_secondary_systems(SELF)) {
    return Core_Failure;
  }
  if (Core_System_shutdown(CORE_SYSTEM(SELF->assets_system))) {
    return Core_Failure;
  }
  if (Core_System_shutdown(CORE_SYSTEM(SELF->aal_system))) {
    return Core_Failure;
  }
  if (Core_System_shutdown(CORE_SYSTEM(SELF->val_system))) {
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

static Core_Result get_assets_context(Core_Assets_Context** RETURN, dx_application* SELF) {
  Core_Assets_Context* context = NULL;
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
    Core_MessageQueue* msg_queue
  )
{
  DX_CONSTRUCT_PREFIX(dx_application);
  //
  SELF->msg_queue = msg_queue;
  //
  if (Core_Configuration_create(&SELF->configuration)) {
    return Core_Failure;
  }
  //
  if (Core_SystemFactory_create_system((Core_System**)&SELF->val_system, CORE_SYSTEMFACTORY(val_system_factory), msg_queue)) {
    CORE_UNREFERENCE(SELF->configuration);
    SELF->configuration = NULL;
    return Core_Failure;
  }
  //
  if (Core_SystemFactory_create_system((Core_System**)&SELF->aal_system, CORE_SYSTEMFACTORY(aal_system_factory), msg_queue)) {
    CORE_UNREFERENCE(SELF->val_system);
    SELF->val_system = NULL;
    CORE_UNREFERENCE(SELF->configuration);
    SELF->configuration = NULL;
    return Core_Failure;
  }
  //
  if (Core_SystemFactory_create_system((Core_System**)&SELF->assets_system, CORE_SYSTEMFACTORY(assets_system_factory), msg_queue)) {
    CORE_UNREFERENCE(SELF->aal_system);
    SELF->aal_system = NULL;
    CORE_UNREFERENCE(SELF->val_system);
    SELF->val_system = NULL;
    CORE_UNREFERENCE(SELF->configuration);
    SELF->configuration = NULL;
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
    Core_MessageQueue* msg_queue
  )
{
  DX_CREATE_PREFIX(dx_application);
  if (dx_application_construct(SELF, val_system_factory, aal_system_factory, assets_system_factory, msg_queue)) {
    CORE_UNREFERENCE(SELF);
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
  if (Core_MessageQueue_push(SELF->msg_queue, msg)) {
    CORE_UNREFERENCE(msg);
    msg = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(msg);
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
  CORE_REFERENCE(g_application);
  *RETURN = g_application;
  return Core_Success;
}
