#if !defined(DX_CORE_SIGNALS_H_INCLUDED)
#define DX_CORE_SIGNALS_H_INCLUDED

#include "dx/core/error.h"
#include "dx/core/object.h"

/// @brief The opaque type of a signal.
typedef struct dx_signal {
  void* pimpl;
} dx_signal;

/// @brief The ID of a connection.
typedef uintptr_t dx_connection_id;

/// @brief Initialize this signal.
/// @param SELF A pointer to this signal.
/// @method-call
dx_result dx_signal_initialize(dx_signal* SELF);

/// @brief Uninitialize this signal.
/// @param SELF A pointer to thsi signal.
void dx_signal_uninitialize(dx_signal* SELF);

/// @brief Connect to a signal.
/// @param RETURN A pointer to a <code>dx_connection_id</code> variable.
/// @param SELF A pointer to this signal.
/// @param object A pointer to the object.
/// The object is wrapped in a weak reference.
/// @method-call
dx_result dx_signal_connect_object(dx_connection_id* RETURN, dx_signal* SELF, dx_object* object);

/// @brief Connect to a signal.
/// @param RETURN A pointer to a <code>dx_connection_id</code> variable.
/// @param SELF A pointer to this signal.
/// @param weak_reference A pointer to the weak reference.
/// @method-call
dx_result dx_signal_connect_weak_reference(dx_connection_id* RETURN, dx_signal* SELF, dx_weak_reference* weak_reference);

dx_result dx_signal_notify(dx_signal* SELF, dx_object* parameter);

#endif // DX_CORE_SIGNALS_H_INCLUDED
