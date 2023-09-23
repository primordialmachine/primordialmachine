#if !defined(DX_RUN_H_INCLUDED)
#define DX_RUN_H_INCLUDED

#include "dx/player/default_application_presenter.h"

/// @brief Get the singleton VAL context object.
/// @return A pointer to the VAL context on success. The null pointer on failure.
/// @success The caller acquired a reference to the context.
dx_val_context* dx_val_context_get();

/// @brief Get the singleton application presenter object.
/// @param RETURN A pointer to a <code>dx_application_presenter*</code> object.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the <code>dx_application_presenter</code> singleton object.
/// The caller acquired a reference to that object.
/// @method-call
/// @warning
/// The application presenter holds references to many engine objects.
/// If such an object acquires a reference to the application presenter, then this yields a reference counting cycle.
/// To avoid such cycles object should
/// - acquire a reference to the application presenter when required and
/// - relinquish that reference asap
/// to ensure cycles are only temporay.
dx_result dx_application_presenter_get(dx_application_presenter** RETURN);

/// @brief Run the engine with the specified command-line arguments. Does not return until the application terminates.
/// @param argc The number of command-line arguments.
/// @param argv A pointer to an array of @a argc pointers to C strings representing the command-line arguments.
/// @return The zero value on success. A non-zero value on failure.
dx_result dx_run(int argc, char** argv);

#endif // DX_RUN_H_INCLUDED
