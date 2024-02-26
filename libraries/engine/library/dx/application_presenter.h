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
/// 2) _startup is called for the "application presenter" by the "application execution environment".
/// If the call succeeds, go to 3) or 4).
/// Otherwise got to 2) or 5)
/// 3) _run is called for the "application presenter" by the "application execution environment".
/// 4) _shutdown is called for the "application presenter" by the "application execution environment".
/// Go to 5.
/// 5) the "application presenter" is destroyed by the "application execution environment".
Core_declareObjectType("Core.ApplicationPresenter",
                       Core_ApplicationPresenter,
                       Core_Object);

static inline Core_ApplicationPresenter* CORE_APPLICATIONPRESENTER(void *p) {
  return (Core_ApplicationPresenter*)p;
}

struct Core_ApplicationPresenter {
  Core_Object _parent;
};

static inline Core_ApplicationPresenter_Dispatch* CORE_APPLICATIONPRESENTER_DISPATCH(void* p) {
  return (Core_ApplicationPresenter_Dispatch*)p;
}

struct Core_ApplicationPresenter_Dispatch {
  Core_Object_Dispatch _parent;
  Core_Result(*startup)(Core_ApplicationPresenter* SELF);
  Core_Result(*run)(Core_ApplicationPresenter* SELF);
  Core_Result(*shutdown)(Core_ApplicationPresenter* SELF);
  Core_Result(*get_console)(dx_console**, Core_ApplicationPresenter* SELF);
  Core_Result(*get_cl_interpreter)(dx_cl_interpreter**, Core_ApplicationPresenter* SELF);
  Core_Result(*request_quit)(Core_ApplicationPresenter* SELF);
  Core_Result(*quit_requested)(Core_Boolean* RETURN, Core_ApplicationPresenter* SELF);
};

/// @constructor{Core_ApplicationPresenter}
Core_Result Core_ApplicationPresenter_construct(Core_ApplicationPresenter* SELF);

/// @brief Callback. Invoked by the application execution environment.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_startup(Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, startup, SELF);
}

/// @brief Callback. Invoked by the application execution environment.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_run(Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, run, SELF);
}

/// @brief Callback. Invoked by the application execution environment.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_shutdown(Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, shutdown, SELF);
}

/// @brief Acquire a reference to the console object.
/// @param RETURN A pointer to a <code>Core_Context*</code> variable.
/// @success <code>*RETURN</code> was assigned a reference to the console object.
/// The caller acquired a reference to that object.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_get_console(dx_console** RETURN, Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, get_console, RETURN, SELF);
}

/// @brief Acquire a reference to the CL interpreter object.
/// @param RETURN A pointer to a <code>dx_cl_interpreter*</code> variable.
/// @success <code>*RETURN</code> was assigned a reference to the CL interpreter object.
/// The caller acquired a reference to that object.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_get_cl_interpreter(dx_cl_interpreter** RETURN, Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, get_cl_interpreter, RETURN, SELF);
}

/// @brief Request the application to quit.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_request_quit(Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, request_quit, SELF);
}

/// @brief Get if the application was requested to quit.
/// @param RETURN A pointer to a <code>Core_Boolean</code> variable.
/// @method{Core_ApplicationPresenter}
static inline Core_Result Core_ApplicationPresenter_quit_requested(Core_Boolean* RETURN, Core_ApplicationPresenter* SELF) {
  DX_OBJECT_VIRTUALCALL(Core_ApplicationPresenter, quit_requested, RETURN, SELF);
}

#endif // DX_APPLICATION_PRESENTER_H_INCLUDED
