#if !defined(CORE_DEFAULTFONTSYSTEM_H_INCLUDED)
#define CORE_DEFAULTFONTSYSTEM_H_INCLUDED

#include "Core/FontSystem.h"
#include "dx/val/context.h"

Core_declareObjectType("Core.DefaultFontSystem",
                       Core_DefaultFontSystem,
                       Core_FontSystem);

static inline Core_DefaultFontSystem* CORE_DEFAULTFONTSYSTEM(void* p) {
  return (Core_DefaultFontSystem*)p;
}

struct Core_DefaultFontSystem {
  Core_FontSystem _parent;
  /// @brief The hidden implementation.
  void* pimpl;
  /// @brief The VAL context.
  Core_Visuals_Context* context;
  /// @brief Map from names to font objects.
  Core_InlineHashMapPP fonts;
};

static inline Core_DefaultFontSystem_Dispatch* CORE_DEFAULTFONTSYSTEM_DISPATCH(void* p) {
  return (Core_DefaultFontSystem_Dispatch*)p;
}

struct Core_DefaultFontSystem_Dispatch {
  Core_FontSystem_Dispatch _parent;
};

Core_Result Core_DefaultFontSystem_construct(Core_DefaultFontSystem* SELF, Core_Visuals_Context* context);

Core_Result Core_DefaultFontSystem_create(Core_DefaultFontSystem** RETURN, Core_Visuals_Context* context);

#endif // CORE_DEFAULTFONTSYSTEM_H_INCLUDED
