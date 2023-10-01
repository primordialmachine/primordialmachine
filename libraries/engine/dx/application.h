#if !defined(DX_APPLICATION_H_INCLUDED)
#define DX_APPLICATION_H_INCLUDED

#include "dx/core.h"

typedef struct dx_aal_context dx_aal_context;
typedef struct dx_aal_system dx_aal_system;
typedef struct dx_aal_system_factory dx_aal_system_factory;

typedef struct dx_val_context dx_val_context;
typedef struct dx_val_system dx_val_system;
typedef struct dx_val_system_factory dx_val_system_factory;

/// The representation of an application.
/// An application is the owner of at least the following objects:
/// - VAL (visuals abstraction layer) system.
/// - AAL (audials abstraction layer) system
/// These systems are started up and shut down through the application object's functions (dx_application_startup and dx_application_shutdown).
DX_DECLARE_OBJECT_TYPE("dx.application",
                       dx_application,
                       dx_object);

static dx_application* DX_APPLICATION(void* p) {
  return (dx_application*)p;
}

struct dx_application {
  dx_object _parent;
  /// @brief A non-referencing pointer to the message queue.
  dx_msg_queue* msg_queue;
  /// @brief The VAL system.
  dx_val_system* val_system;
  /// @brief The AAL system.
  dx_aal_system* aal_system;
};

static dx_application_dispatch* DX_APPLICATION_DISPATCH(void* p) {
  return (dx_application_dispatch*)p;
}

struct dx_application_dispatch {
  dx_object_dispatch _parent;
  dx_result (*update)(dx_application*);
  dx_result(*startup_systems)(dx_application*);
  dx_result(*shutdown_systems)(dx_application*);
  dx_result(*get_val_context)(dx_val_context**, dx_application*);
  dx_result(*get_aal_context)(dx_aal_context**, dx_application*);
};

/// @internal
/// @brief Construct this application object.
/// @param SELF A pointer to this application object.
/// @param val_system_factory A pointer to the VAL system factory.
/// @param aal_system_factory A pointer to the AAL system factory.
/// @param msg_queue A pointer to the message queue to be used by this application object.
/// @method-call
dx_result dx_application_construct(dx_application* SELF, dx_val_system_factory* val_system_factory, dx_aal_system_factory* aal_system_factory, dx_msg_queue* msg_queue);

dx_result dx_application_create(dx_application** RETURN, dx_val_system_factory* val_system_factory, dx_aal_system_factory* aal_system_factory, dx_msg_queue* msg_queue);

/// @param SELF A pointer to this application.
/// @method-call
static inline dx_result dx_application_update(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, update, SELF);
}

/// @brief Startup the systems.
/// @param SELF A pointer to this application.
/// @method-call
/// @pre The systems are running.
static inline dx_result dx_application_startup_systems(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, startup_systems, SELF);
}

/// @brief Shutdown the systems.
/// @param SELF A pointer to this application.
/// @method-call
/// @pre The systems are not running.
static inline dx_result dx_application_shutdown_systems(dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, shutdown_systems, SELF);
}

/// @brief Get the VAL context.
/// @param RETURN A pointer to a <code>dx_val_context*</code> variable.
/// @param SELF A pointer to this application.
/// @method-call
/// @success <code>*RETURN</code> was assigned a pointer to the VAL context object. The caller acquired a reference to that object.
/// @pre The VAL backend is running.
static inline dx_result dx_application_get_val_context(dx_val_context** RETURN, dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, get_val_context, RETURN, SELF);
}

/// @brief Get the AAL context.
/// @param RETURN A pointer to a <code>dx_aal_context*</code> variable.
/// @param SELF A pointer to this application.
/// @method-call
/// @success <code>*RETURN</code> was assigned a pointer to the AAL context object. The caller acquired a reference to that object.
/// @pre The AAL backend is running.
static inline dx_result dx_application_get_aal_context(dx_aal_context** RETURN, dx_application* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application, get_aal_context, RETURN, SELF);
}

/// @internal
/// @brief Emit a "quit" message.
/// @param SELF A pointer to this application object.
/// @method-call
dx_result dx_application_emit_quit_msg(dx_application* SELF);

/// @internal
/// @brief Get the instance of the application singleton object.
/// @success <code>*RETURN</code> was assigned a pointer to the application singleton object.
/// The caller acquired a reference to that object.
/// @procedure-call
dx_result dx_application_get(dx_application** RETURN);

#endif // DX_APPLICATION_H_INCLUDED
