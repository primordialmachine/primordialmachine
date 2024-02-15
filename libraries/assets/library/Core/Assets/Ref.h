// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
#if !defined(CORE_ASSETS_REF_H_INCLUDED)
#define CORE_ASSETS_REF_H_INCLUDED

#include "dx/core.h"

/// @brief A reference (short: "ref") to a definition (short: "def").
/// For example
/// @code
/// { ... color : "MyPalette.Malachite" ... }
/// @endcode
/// is a ref to the def of the name @code{malachite}.
Core_declareObjectType("Core.Assets.Ref",
                       Core_Assets_Ref,
                       Core_Object);

static inline Core_Assets_Ref* CORE_ASSETS_REF_REFERENCE(void* p) {
  return (Core_Assets_Ref*)p;
}

struct Core_Assets_Ref {
  Core_Object _parent;
  /// @brief The name.
  Core_String* name;
  /// @brief A pointer to the object if it was successfully resolved.
  Core_Object* object;
};

struct Core_Assets_Ref_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result Core_Assets_Ref_construct(Core_Assets_Ref* SELF, Core_String* name);

Core_Result Core_Assets_Ref_create(Core_Assets_Ref** RETURN, Core_String* name);

#endif // CORE_ASSETS_REF_H_INCLUDED
