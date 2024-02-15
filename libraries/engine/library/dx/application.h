#if !defined(CORE_APPLICATION_H_INCLUDED)
#define CORE_APPLICATION_H_INCLUDED

#include "dx/core.h"

typedef struct dx_font_manager dx_font_manager;
typedef struct dx_rectangle_presenter dx_rectangle_presenter;
typedef struct dx_font_presenter dx_font_presenter;

typedef struct Core_Audials_Context Core_Audials_Context;
typedef struct Core_Audials_System Core_Audials_System;
typedef struct Core_Audials_SystemFactory Core_Audials_SystemFactory;

typedef struct Core_Visuals_Context Core_Visuals_Context;
typedef struct Core_Visuals_System Core_Visuals_System;
typedef struct Core_Visuals_SystemFactory Core_Visuals_SystemFactory;

typedef struct Core_Assets_Context Core_Assets_Context;
typedef struct Core_Assets_System Core_Assets_System;
typedef struct Core_Assets_SystemFactory Core_Assets_SystemFactory;

/// The representation of an application.
/// An application is the owner of at least the following objects:
/// - VAL (visuals abstraction layer) system.
/// - AAL (audials abstraction layer) system
/// - Assets system
/// These systems are started up and shut down through the application object's functions (dx_application_startup and dx_application_shutdown).
Core_declareObjectType("Core.application",
                       Core_Application,
                       Core_Object);

static Core_Application* CORE_APPLICATION(void* p) {
  return (Core_Application*)p;
}

struct Core_Application {
  Core_Object _parent;
  
  /// @brief The configuration.
  Core_Configuration* configuration;
  
  /// @brief A pointer to the message queue.
  /// The application does not retain a reference to the message queue.
  Core_MessageQueue* msg_queue;
  
  /// @brief The visuals system.
  Core_Visuals_System* visualsSystem;
  
  /// @brief The audials system.
  Core_Audials_System* audialsSystem;
  
  /// @brief The assets system.
  Core_Assets_System* assetsSystem;
  
  /// @brief The font manager.
  dx_font_manager* font_manager;

  dx_rectangle_presenter* rectangle_presenter;
  dx_font_presenter* font_presenter;
};

static Core_Application_Dispatch* CORE_APPLICATION_DISPATCH(void* p) {
  return (Core_Application_Dispatch*)p;
}

struct Core_Application_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*update)(Core_Application*);
  Core_Result(*startup_systems)(Core_Application*);
  Core_Result(*shutdown_systems)(Core_Application*);
  Core_Result(*get_val_context)(Core_Visuals_Context**, Core_Application*);
  Core_Result(*get_aal_context)(Core_Audials_Context**, Core_Application*);
  Core_Result(*get_assets_context)(Core_Assets_Context**, Core_Application*);
};

/// @internal
/// @param val_system_factory A pointer to the VAL system factory.
/// @param aal_system_factory A pointer to the AAL system factory.
/// @param asset_system_factory A pointer to the Assets system factory.
/// @param msg_queue A pointer to the message queue to be used by this application object.
/// @constructor{Core_Application}
Core_Result
Core_Application_construct
  (
    Core_Application* SELF,
    Core_Visuals_SystemFactory* val_system_factory,
    Core_Audials_SystemFactory* aal_system_factory,
    Core_Assets_SystemFactory* assets_system_factory,
    Core_MessageQueue* msg_queue
  );

Core_Result
Core_Application_create
  (
    Core_Application** RETURN,
    Core_Visuals_SystemFactory* val_system_factory,
    Core_Audials_SystemFactory* aal_system_factory,
    Core_Assets_SystemFactory* assets_system_factory,
    Core_MessageQueue* msg_queue
  );

/// @method{Core_Application}
static inline Core_Result Core_Application_update(Core_Application* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Application, update, SELF);
}

/// @brief Startup the systems.
/// @method{Core_Application}
/// @pre The systems are running.
static inline Core_Result Core_Application_startup_systems(Core_Application* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Application, startup_systems, SELF);
}

/// @brief Shutdown the systems.
/// @method{Core_Application}
/// @pre The systems are not running.
static inline Core_Result Core_Application_shutdown_systems(Core_Application* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Application, shutdown_systems, SELF);
}

/// @brief Get the VAL context.
/// @param RETURN A pointer to a <code>Core_Visuals_Context*</code> variable.
/// @method{Core_Application}
/// @success <code>*RETURN</code> was assigned a pointer to the VAL context object.
/// The caller acquired a reference to that object.
/// @pre The VAL system is running.
static inline Core_Result Core_Application_get_val_context(Core_Visuals_Context** RETURN, Core_Application* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Application, get_val_context, RETURN, SELF);
}

/// @brief Get the AAL context.
/// @param RETURN A pointer to a <code>Core_Audials_Context*</code> variable.
/// @method{Core_Application}
/// @success <code>*RETURN</code> was assigned a pointer to the AAL context object.
/// The caller acquired a reference to that object.
/// @pre The AAL system is running.
static inline Core_Result Core_Application_get_aal_context(Core_Audials_Context** RETURN, Core_Application* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Application, get_aal_context, RETURN, SELF);
}

/// @brief Get the Assets context.
/// @param RETURN A pointer to a <code>Core_Assets_Context*</code> variable.
/// @method{Core_Application}
/// @success <code>*RETURN</code> was assigned a pointer to the Assets context object.
/// The caller acquired a reference to that object.
/// @pre The Assets system is running.
static inline Core_Result Core_Application_get_assets_context(Core_Assets_Context** RETURN, Core_Application* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_Application, get_assets_context, RETURN, SELF);
}

/// @internal
/// @brief Emit a "quit" message.
/// @method{Core_Application}
Core_Result Core_Application_emit_quit_msg(Core_Application* SELF);

/// @internal
/// @brief Get the instance of the application singleton object.
/// @success <code>*RETURN</code> was assigned a pointer to the application singleton object.
/// The caller acquired a reference to that object.
/// @procedure
Core_Result Core_Application_get(Core_Application** RETURN);

#endif // CORE_APPLICATION_H_INCLUDED
