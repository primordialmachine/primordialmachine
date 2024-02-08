#if !defined(DX_APPLICATION_H_INCLUDED)
#define DX_APPLICATION_H_INCLUDED

#include "dx/core.h"

typedef struct dx_font_manager dx_font_manager;
typedef struct dx_rectangle_presenter dx_rectangle_presenter;
typedef struct dx_font_presenter dx_font_presenter;

typedef struct dx_aal_context dx_aal_context;
typedef struct dx_aal_system dx_aal_system;
typedef struct dx_aal_system_factory dx_aal_system_factory;

typedef struct dx_val_context dx_val_context;
typedef struct dx_val_system dx_val_system;
typedef struct dx_val_system_factory dx_val_system_factory;

typedef struct dx_assets_context dx_assets_context;
typedef struct dx_assets_system dx_assets_system;
typedef struct dx_assets_system_factory dx_assets_system_factory;

/// The representation of an application.
/// An application is the owner of at least the following objects:
/// - VAL (visuals abstraction layer) system.
/// - AAL (audials abstraction layer) system
/// - Assets system
/// These systems are started up and shut down through the application object's functions (dx_application_startup and dx_application_shutdown).
Core_declareObjectType("dx.application",
                       dx_application,
                       Core_Object);

static dx_application* DX_APPLICATION(void* p) {
  return (dx_application*)p;
}

struct dx_application {
  Core_Object _parent;
  
  /// @brief The configuration.
  Core_Configuration* configuration;
  
  /// @brief A pointer to the message queue.
  /// The application does not retain a reference to the message queue.
  Core_MessageQueue* msg_queue;
  
  /// @brief The VAL system.
  dx_val_system* val_system;
  
  /// @brief The AAL system.
  dx_aal_system* aal_system;
  
  /// @brief The assets system.
  dx_assets_system* assets_system;
  
  /// @brief The font manager.
  dx_font_manager* font_manager;

  dx_rectangle_presenter* rectangle_presenter;
  dx_font_presenter* font_presenter;
};

static dx_application_Dispatch* DX_APPLICATION_DISPATCH(void* p) {
  return (dx_application_Dispatch*)p;
}

struct dx_application_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result (*update)(dx_application*);
  Core_Result(*startup_systems)(dx_application*);
  Core_Result(*shutdown_systems)(dx_application*);
  Core_Result(*get_val_context)(dx_val_context**, dx_application*);
  Core_Result(*get_aal_context)(dx_aal_context**, dx_application*);
  Core_Result(*get_assets_context)(dx_assets_context**, dx_application*);
};

/// @internal
/// @param val_system_factory A pointer to the VAL system factory.
/// @param aal_system_factory A pointer to the AAL system factory.
/// @param asset_system_factory A pointer to the Assets system factory.
/// @param msg_queue A pointer to the message queue to be used by this application object.
/// @constructor{dx_application}
Core_Result dx_application_construct
  (
    dx_application* SELF,
    dx_val_system_factory* val_system_factory,
    dx_aal_system_factory* aal_system_factory,
    dx_assets_system_factory* assets_system_factory,
    Core_MessageQueue* msg_queue
  );

Core_Result dx_application_create
  (
    dx_application** RETURN,
    dx_val_system_factory* val_system_factory,
    dx_aal_system_factory* aal_system_factory,
    dx_assets_system_factory* assets_system_factory,
    Core_MessageQueue* msg_queue
  );

/// @method{dx_application}
static inline Core_Result dx_application_update(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, update, SELF);
}

/// @brief Startup the systems.
/// @method{dx_application}
/// @pre The systems are running.
static inline Core_Result dx_application_startup_systems(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, startup_systems, SELF);
}

/// @brief Shutdown the systems.
/// @method{dx_application}
/// @pre The systems are not running.
static inline Core_Result dx_application_shutdown_systems(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, shutdown_systems, SELF);
}

/// @brief Get the VAL context.
/// @param RETURN A pointer to a <code>dx_val_context*</code> variable.
/// @method{dx_application}
/// @success <code>*RETURN</code> was assigned a pointer to the VAL context object. The caller acquired a reference to that object.
/// @pre The VAL backend is running.
static inline Core_Result dx_application_get_val_context(dx_val_context** RETURN, dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, get_val_context, RETURN, SELF);
}

/// @brief Get the AAL context.
/// @param RETURN A pointer to a <code>dx_aal_context*</code> variable.
/// @method{dx_application}
/// @success <code>*RETURN</code> was assigned a pointer to the AAL context object. The caller acquired a reference to that object.
/// @pre The AAL backend is running.
static inline Core_Result dx_application_get_aal_context(dx_aal_context** RETURN, dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, get_aal_context, RETURN, SELF);
}

/// @brief Get the Assets context.
/// @param RETURN A pointer to a <code>dx_assets_context*</code> variable.
/// @method{dx_application}
/// @success <code>*RETURN</code> was assigned a pointer to the Assets context object. The caller acquired a reference to that object.
/// @pre The Assets backend is running.
static inline Core_Result dx_application_get_assets_context(dx_assets_context** RETURN, dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, get_assets_context, RETURN, SELF);
}

/// @internal
/// @brief Emit a "quit" message.
/// @method{dx_application}
Core_Result dx_application_emit_quit_msg(dx_application* SELF);

/// @internal
/// @brief Get the instance of the application singleton object.
/// @success <code>*RETURN</code> was assigned a pointer to the application singleton object.
/// The caller acquired a reference to that object.
/// @procedure
Core_Result dx_application_get(dx_application** RETURN);

#endif // DX_APPLICATION_H_INCLUDED
