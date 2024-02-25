#include "dx/application.h"


#if Core_OperatingSystem_Windows == Core_OperatingSystem
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#else
  #error("environment not (yet) supported")
#endif


#include "dx/val/system.h"
#include "Core/Audials/System.h"
#include "Core/Assets/System.h"

#include "dx/font_manager.h"

#include "dx/rectangle_presenter.h"
#include "dx/font_presenter.h"


static Core_Application* g_application = NULL;

Core_defineObjectType("Core.Application",
                      Core_Application,
                      Core_Object);

static Core_Result update(Core_Application* SELF);

static Core_Result startup_secondary_systems(Core_Application* SELF);

static Core_Result startup_systems(Core_Application* SELF);

static Core_Result shutdown_secondary_systems(Core_Application* SELF);

static Core_Result shutdown_systems(Core_Application* SELF);

static Core_Result getVisualsContext(Core_Visuals_Context** RETURN, Core_Application* SELF);

static Core_Result getAudialsContext(Core_Audials_Context** RETURN, Core_Application* SELF);

static Core_Result getAssetsContext(Core_Assets_Context** RETURN, Core_Application* SELF);

static void Core_Application_destruct(Core_Application* SELF) {
  CORE_UNREFERENCE(SELF->assetsSystem);
  SELF->assetsSystem = NULL;

  CORE_UNREFERENCE(SELF->audialsSystem);
  SELF->audialsSystem = NULL;
  
  CORE_UNREFERENCE(SELF->visualsSystem);
  SELF->visualsSystem = NULL;

  CORE_UNREFERENCE(SELF->configuration);
  SELF->configuration = NULL;
  
  g_application = NULL; // Singleton guard.
}

static void Core_Application_constructDispatch(Core_Application_Dispatch* SELF) {
  CORE_APPLICATION_DISPATCH(SELF)->startup_systems = (Core_Result(*)(Core_Application*)) & startup_systems;
  CORE_APPLICATION_DISPATCH(SELF)->shutdown_systems = (Core_Result(*)(Core_Application*)) & shutdown_systems;
  CORE_APPLICATION_DISPATCH(SELF)->update = (Core_Result(*)(Core_Application*))&update;
  CORE_APPLICATION_DISPATCH(SELF)->get_val_context = (Core_Result(*)(Core_Visuals_Context**, Core_Application*)) & getVisualsContext;
  CORE_APPLICATION_DISPATCH(SELF)->get_aal_context = (Core_Result(*)(Core_Audials_Context**, Core_Application*)) & getAudialsContext;
  CORE_APPLICATION_DISPATCH(SELF)->get_assets_context = (Core_Result(*)(Core_Assets_Context**, Core_Application*)) & getAssetsContext;
}

static Core_Result update(Core_Application* SELF) {
#if Core_OperatingSystem_Windows == Core_OperatingSystem
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      Core_Application_emit_quit_msg(CORE_APPLICATION(SELF));
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

static Core_Result startup_secondary_systems(Core_Application* SELF) {
  Core_Visuals_Context* visualsContext = NULL;
  if (Core_Application_get_val_context(&visualsContext, g_application)) {
    return Core_Failure;
  }
  Core_Audials_Context* audialsContext = NULL;
  if (Core_Application_get_aal_context(&audialsContext, g_application)) {
    CORE_UNREFERENCE(visualsContext);
    visualsContext = NULL;
    return Core_Failure;
  }
  if (Core_DefaultFontSystem_create((Core_DefaultFontSystem**) & SELF->fontSystem, visualsContext)) {
    CORE_UNREFERENCE(audialsContext);
    audialsContext = NULL;
    CORE_UNREFERENCE(visualsContext);
    visualsContext = NULL;
    return Core_Failure;
  }
  if (Core_System_startup(CORE_SYSTEM(SELF->fontSystem))) {
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    CORE_UNREFERENCE(audialsContext);
    audialsContext = NULL;
    CORE_UNREFERENCE(visualsContext);
    visualsContext = NULL;
    return Core_Failure;
  }
  if (dx_rectangle_presenter_create(&SELF->rectangle_presenter, visualsContext, audialsContext)) {
    Core_System_shutdown(CORE_SYSTEM(SELF->fontSystem));
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    CORE_UNREFERENCE(audialsContext);
    audialsContext = NULL;
    CORE_UNREFERENCE(visualsContext);
    visualsContext = NULL;
    return Core_Failure;
  }
  if (dx_font_presenter_create(&SELF->font_presenter, SELF->fontSystem, SELF->rectangle_presenter)) {
    CORE_UNREFERENCE(SELF->rectangle_presenter);
    SELF->rectangle_presenter = NULL;
    Core_System_shutdown(CORE_SYSTEM(SELF->fontSystem));
    CORE_UNREFERENCE(SELF->fontSystem);
    SELF->fontSystem = NULL;
    CORE_UNREFERENCE(audialsContext);
    audialsContext = NULL;
    CORE_UNREFERENCE(visualsContext);
    visualsContext = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(audialsContext);
  audialsContext = NULL;
  CORE_UNREFERENCE(visualsContext);
  visualsContext = NULL;
  return Core_Success;
}

static Core_Result startup_systems(Core_Application* SELF) {
  if (Core_System_startup(CORE_SYSTEM(SELF->visualsSystem))) {
    return Core_Failure;
  }
  if (Core_System_startup(CORE_SYSTEM(SELF->audialsSystem))) {
    Core_System_shutdown(CORE_SYSTEM(SELF->visualsSystem));
    return Core_Failure;
  }
  if (Core_System_startup(CORE_SYSTEM(SELF->assetsSystem))) {
    Core_System_shutdown(CORE_SYSTEM(SELF->audialsSystem));
    Core_System_shutdown(CORE_SYSTEM(SELF->visualsSystem));
    return Core_Failure;
  }
  if (startup_secondary_systems(SELF)) {
    Core_System_shutdown(CORE_SYSTEM(SELF->assetsSystem));
    Core_System_shutdown(CORE_SYSTEM(SELF->audialsSystem));
    Core_System_shutdown(CORE_SYSTEM(SELF->visualsSystem));
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result shutdown_secondary_systems(Core_Application* SELF) {
  CORE_UNREFERENCE(SELF->font_presenter);
  SELF->font_presenter = NULL;
  CORE_UNREFERENCE(SELF->rectangle_presenter);
  SELF->rectangle_presenter = NULL;
  Core_System_shutdown(CORE_SYSTEM(SELF->fontSystem));
  CORE_UNREFERENCE(SELF->fontSystem);
  SELF->fontSystem = NULL;
  return Core_Success;
}

static Core_Result shutdown_systems(Core_Application* SELF) {
  if (shutdown_secondary_systems(SELF)) {
    return Core_Failure;
  }
  if (Core_System_shutdown(CORE_SYSTEM(SELF->assetsSystem))) {
    return Core_Failure;
  }
  if (Core_System_shutdown(CORE_SYSTEM(SELF->audialsSystem))) {
    return Core_Failure;
  }
  if (Core_System_shutdown(CORE_SYSTEM(SELF->visualsSystem))) {
    return Core_Failure;
  }
  return Core_Success;
}

static Core_Result getVisualsContext(Core_Visuals_Context** RETURN, Core_Application* SELF) {
  Core_Visuals_Context* context = NULL;
  if (Core_Visuals_System_getContext(&context, SELF->visualsSystem)) {
    return Core_Failure;
  }
  *RETURN = context;
  return Core_Success;
}

static Core_Result getAudialsContext(Core_Audials_Context** RETURN, Core_Application* SELF) {
  Core_Audials_Context* context = NULL;
  if (Core_Audials_System_getContext(&context, SELF->audialsSystem)) {
    return Core_Failure;
  }
  *RETURN = context;
  return Core_Success;
}

static Core_Result getAssetsContext(Core_Assets_Context** RETURN, Core_Application* SELF) {
  Core_Assets_Context* context = NULL;
  if (Core_Assets_System_getContext(&context, SELF->assetsSystem)) {
    return Core_Failure;
  }
  *RETURN = context;
  return Core_Success;
}

Core_Result
Core_Application_construct
  (
    Core_Application* SELF,
    Core_Visuals_SystemFactory* val_system_factory,
    Core_Audials_SystemFactory* aal_system_factory,
    Core_Assets_SystemFactory* assets_system_factory,
    Core_MessageQueue* msg_queue
  )
{
  Core_BeginConstructor(Core_Application);
  //
  SELF->msg_queue = msg_queue;
  //
  if (Core_Configuration_create(&SELF->configuration)) {
    return Core_Failure;
  }
  //
  if (Core_SystemFactory_createSystem((Core_System**)&SELF->visualsSystem, CORE_SYSTEMFACTORY(val_system_factory), msg_queue)) {
    CORE_UNREFERENCE(SELF->configuration);
    SELF->configuration = NULL;
    return Core_Failure;
  }
  //
  if (Core_SystemFactory_createSystem((Core_System**)&SELF->audialsSystem, CORE_SYSTEMFACTORY(aal_system_factory), msg_queue)) {
    CORE_UNREFERENCE(SELF->visualsSystem);
    SELF->visualsSystem = NULL;
    CORE_UNREFERENCE(SELF->configuration);
    SELF->configuration = NULL;
    return Core_Failure;
  }
  //
  if (Core_SystemFactory_createSystem((Core_System**)&SELF->assetsSystem, CORE_SYSTEMFACTORY(assets_system_factory), msg_queue)) {
    CORE_UNREFERENCE(SELF->audialsSystem);
    SELF->audialsSystem = NULL;
    CORE_UNREFERENCE(SELF->visualsSystem);
    SELF->visualsSystem = NULL;
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

Core_Result
Core_Application_create
  (
    Core_Application** RETURN,
    Core_Visuals_SystemFactory* val_system_factory,
    Core_Audials_SystemFactory* aal_system_factory,
    Core_Assets_SystemFactory* assets_system_factory,
    Core_MessageQueue* msg_queue
  )
{
  DX_CREATE_PREFIX(Core_Application);
  if (Core_Application_construct(SELF, val_system_factory, aal_system_factory, assets_system_factory, msg_queue)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result Core_Application_emit_quit_msg(Core_Application* SELF) {
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

Core_Result Core_Application_get(Core_Application** RETURN) {
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
