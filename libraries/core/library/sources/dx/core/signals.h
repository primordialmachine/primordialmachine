// A signal is identified by a type t and a name n.
// We write (t,n) -> s if the signal is identified by the type t and the name n.
// If a signal (t,n) -> s is defined then trying to define a signal (t,n) -> s' is an error.  
// If (t,n) -> s then (t',n) -> s for any descendant types t' of t. We write t' < t to indicate that t' is a descendant type of t.
// By the above: If a signal (t,n) -> s is defined then defining a signal (t',n) -> s' for any type t' <= t is an error.
#if !defined(CORE_SIGNALS_H_INCLUDED)
#define CORE_SIGNALS_H_INCLUDED

#include "Core/Error.h"
#include "Core/Result.h"
#include "dx/core/Object.h"

typedef Core_Result(Core_Callback)(Core_Object* context, Core_Object* parameter);

/// @brief Add a signal to a type.
/// @param type The type of the signal.
/// @param name The name of the signal.
/// @procedure
Core_Result Core_Signals_add(Core_Type* type, char const* p, Core_Size l);

/// @brief Add a connection.
/// @param instance The instance to add the connection to.
/// @param p, n The name of the signal to connect to.
/// @param context, callback The context and the callback.
/// @remarks
/// The signal does not store a reference but a weak reference to the context object.
/// If the weak reference becomes invalid, then this connection will not be invoked anymore.
/// @procedure
Core_Result Core_Signals_connect(void** RETURN, Core_Object* instance, char const *p, Core_Size n, Core_Object* context, Core_Callback *callback);

/// @brief Notify a signal.
/// @param instance The instance to notify the signal for.
/// @param p, n The name of the signal to invoke.
/// @param argument The argument to pass.
/// @procedure
Core_Result Core_Signals_invoke(Core_Object* instance, char const *p, Core_Size n, Core_Object* argument);

#endif // CORE_SIGNALS_H_INCLUDED
