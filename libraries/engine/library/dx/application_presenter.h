#if !defined(DX_APPLICATION_PRESENTER_H_INCLUDED)
#define DX_APPLICATION_PRESENTER_H_INCLUDED

#include "dx/core.h"
#include "dx/console.h"
#include "dx/val/context.h"
#include "dx/cl_interpreter.h"

/// The base of all "application presenters".
///
/// An "application" presents "scene (assets)" using "scene presenters".
/// "application presenters" present these "applications".
/// "applications presenters" are created, destroyed, and operated by "application execution environment".
/// "application presenters" have a contract with the "application execution environment".
/// 1) the "application presenter" is created by the "application execution environment"
/// 2) dx_applicatione_presenter_startup is called for the "application presenter" by the "application execution environment".
/// If the call succeeds, go to 3) or 4).
/// Otherwise got to 2) or 5)
/// 3) dx_application_presenter_run is called for the "application presenter" by the "application execution environment".
/// 4) dx_application_presenter_shutdown is called for the "application presenter" by the "application execution environment".
/// Go to 5.
/// 5) the "application presenter" is destroyed by the "application execution environment".
Core_declareObjectType("dx.application_presenter",
                       dx_application_presenter,
                       Core_Object);

static inline dx_application_presenter* DX_APPLICATION_PRESENTER(void *p) {
  return (dx_application_presenter*)p;
}

struct dx_application_presenter {
  Core_Object _parent;
};

static inline dx_application_presenter_Dispatch* DX_APPLICATION_PRESENTER_DISPATCH(void* p) {
  return (dx_application_presenter_Dispatch*)p;
}

struct dx_application_presenter_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*startup)(dx_application_presenter* SELF);
  Core_Result(*run)(dx_application_presenter* SELF);
  Core_Result(*shutdown)(dx_application_presenter* SELF);
  Core_Result(*get_console)(dx_console**, dx_application_presenter* SELF);
  Core_Result(*get_cl_interpreter)(dx_cl_interpreter**, dx_application_presenter* SELF);
  Core_Result(*request_quit)(dx_application_presenter* SELF);
  Core_Result(*quit_requested)(Core_Boolean* RETURN, dx_application_presenter* SELF);
};

/// @constructor{dx_application_presenter}
Core_Result dx_application_presenter_construct(dx_application_presenter* SELF);

/// @brief Callback. Invoked by the application execution environment.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_startup(dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, startup, SELF);
}

/// @brief Callback. Invoked by the application execution environment.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_run(dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, run, SELF);
}

/// @brief Callback. Invoked by the application execution environment.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_shutdown(dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, shutdown, SELF);
}

/// @brief Acquire a reference to the console object.
/// @param RETURN A pointer to a <code>Core_Context*</code> variable.
/// @success <code>*RETURN</code> was assigned a reference to the console object.
/// The caller acquired a reference to that object.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_get_console(dx_console** RETURN, dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, get_console, RETURN, SELF);
}

/// @brief Acquire a reference to the CL interpreter object.
/// @param RETURN A pointer to a <code>dx_cl_interpreter*</code> variable.
/// @success <code>*RETURN</code> was assigned a reference to the CL interpreter object.
/// The caller acquired a reference to that object.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_get_cl_interpreter(dx_cl_interpreter** RETURN, dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, get_cl_interpreter, RETURN, SELF);
}

/// @brief Request the application to quit.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_request_quit(dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, request_quit, SELF);
}

/// @brief Get if the application was requested to quit.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @method{dx_application_presenter}
static inline Core_Result dx_application_presenter_quit_requested(Core_Boolean* RETURN, dx_application_presenter* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_application_presenter, quit_requested, RETURN, SELF);
}

#endif // DX_APPLICATION_PRESENTER_H_INCLUDED
