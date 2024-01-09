#if !defined(DX_ASSET_REFERENCE_H_INCLUDED)
#define DX_ASSET_REFERENCE_H_INCLUDED

#include "dx/core.h"

/// @brief A reference to another definition.
/// For example
/// @code
/// { ... color : "malachite" ... }
/// @endcode
/// is a reference to an object of name @code{malachite}.
Core_declareObjectType("dx.asset.reference",
                       dx_asset_reference,
                       Core_Object);

static inline dx_asset_reference* DX_ASSET_REFERENCE(void* p) {
  return (dx_asset_reference*)p;
}

struct dx_asset_reference {
  Core_Object _parent;
  /// @brief The name.
  Core_String* name;
  /// @brief A pointer to the object if it was successfully resolved.
  Core_Object* object;
};

struct dx_asset_reference_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_asset_reference_construct(dx_asset_reference* SELF, Core_String* name);

Core_Result dx_asset_reference_create(dx_asset_reference** RETURN, Core_String* name);

#endif // DX_ASSET_REFERENCE_H_INCLUDED
