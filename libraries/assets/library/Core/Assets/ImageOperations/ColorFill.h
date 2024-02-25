#if !defined(CORE_ASSETS_IMAGEOPERATIONS_COLORFILL_H_INCLUDED)
#define CORE_ASSETS_IMAGEOPERATIONS_COLORFILL_H_INCLUDED

#include "Core/Assets/ImageOperation.h"
#include "Core/Assets/Ref.h"
#include "Core/Assets/ColorRgbN8.h"

/* http://localhost/assets#core-assets-imageoperations-colorfill */
Core_declareObjectType("Core.Assets.ImageOperations.ColorFill",
                       Core_Assets_ImageOperations_ColorFill,
                       Core_Assets_ImageOperation);

static inline Core_Assets_ImageOperations_ColorFill* CORE_ASSETS_IMAGEOPERATIONS_COLORFILL(void* p) {
  return (Core_Assets_ImageOperations_ColorFill*)p;
}

struct Core_Assets_ImageOperations_ColorFill {
  Core_Assets_ImageOperation _parent;
  // The reference to the color.
  // Resolves to a Core_Assets_ColorRgbN8.
  Core_Assets_Ref* color;
};

static inline Core_Assets_ImageOperations_ColorFill_Dispatch* CORE_ASSETS_IMAGEOPERATIONS_COLORFILL_DISPATCH(void* p) {
  return (Core_Assets_ImageOperations_ColorFill_Dispatch*)p;
}

struct Core_Assets_ImageOperations_ColorFill_Dispatch {
  Core_Assets_ImageOperation_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{Core_Assets_ImageOperations_ColorFill}
Core_Result Core_Assets_ImageOperations_ColorFill_construct(Core_Assets_ImageOperations_ColorFill* SELF);

/// @create-operator{Core_Assets_ImageOperations_ColorFill}
Core_Result Core_Assets_ImageOperations_ColorFill_create(Core_Assets_ImageOperations_ColorFill** RETURN);

/// @brief Set the color.
/// @param color A pointer to the color object.
/// @method{Core_Assets_ImageOperations_ColorFill}
Core_Result Core_Assets_ImageOperations_ColorFill_set_color(Core_Assets_ImageOperations_ColorFill* SELF, Core_Assets_ColorRgbN8* color);

/// @brief Get the color.
/// @param RETURN A pointer to the DX_RGB_U8 color.
/// @success <code>*RETURN</code> was assigened the values.
/// @method{Core_Assets_ImageOperations_ColorFill}
Core_Result Core_Assets_ImageOperations_ColorFill_get_color(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_ColorFill* SELF);

#endif // CORE_ASSETS_IMAGEOPERATIONS_COLORFILL_H_INCLUDED
