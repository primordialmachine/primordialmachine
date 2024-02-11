#if !defined(CORE_ASSETS_COLORGBN8_H_INCLUDED)
#define CORE_ASSETS_COLORGBN8_H_INCLUDED

#include "Core/Assets/Def.h"

Core_declareObjectType("Core.Assets.ColorRgbN8",
                       Core_Assets_ColorRgbN8,
                       Core_Object);

static inline Core_Assets_ColorRgbN8* CORE_ASSETS_COLORRGBN8(void* p) {
  return (Core_Assets_ColorRgbN8*)p;
}

struct Core_Assets_ColorRgbN8 {
  Core_Object _parent;
  Core_InlineRgbN8 value;
};

static inline Core_Assets_ColorRgbN8_Dispatch* CORE_ASSETS_COLORRGBN8_DISPATCH(void* p) {
  return (Core_Assets_ColorRgbN8_Dispatch*)p;
}

struct Core_Assets_ColorRgbN8_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @detail The color is initialized with the specified values.
/// @param value A pointer to a <code>Core_InlineRgbN8</code> object from which the values are read.
/// @constructor{Core_Assets_ColorRgbN8} 
Core_Result Core_Assets_ColorRgbN8_construct(Core_Assets_ColorRgbN8* SELF, Core_InlineRgbN8 const* value);

Core_Result Core_Assets_ColorRgbN8_create(Core_Assets_ColorRgbN8** RESULT, Core_InlineRgbN8 const* value);

/// @brief Get the values.
/// @param RETURN A pointer to a <code>Core_InlineRgbN8</code> object.
/// @success <code>*RETURN</code> was assigned the component values.
/// @constructor{Core_Assets_ColorRgbN8}
Core_Result Core_Assets_ColorRgbN8_get_value(Core_InlineRgbN8* RETURN, Core_Assets_ColorRgbN8* SELF);

#endif // CORE_ASSETS_COLORGBN8_H_INCLUDED
