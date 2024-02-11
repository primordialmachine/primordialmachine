#if !defined(CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL_H_INCLUDED)
#define CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL_H_INCLUDED

#include "Core/Assets/ImageOperation.h"
#include "Core/Assets/Ref.h"
#include "Core/Assets/ColorRgbN8.h"

/// @extends Core_Assets_ImageOperation
/// @brief An image operation.
/// Fills an image with the a checkerboard pattern.
Core_declareObjectType("Core.Assets.ImageOperations.CheckerboardPatternFill",
                       Core_Assets_ImageOperations_CheckerboardPatternFill,
                       Core_Assets_ImageOperation);

static inline Core_Assets_ImageOperations_CheckerboardPatternFill* CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL(void* p) {
  return (Core_Assets_ImageOperations_CheckerboardPatternFill*)p;
}

struct Core_Assets_ImageOperations_CheckerboardPatternFill {
  Core_Assets_ImageOperation _parent;
  
  Core_Size number_of_checkers_horizontal;
  Core_Size number_of_checkers_vertical;

  Core_Size checker_size_horizontal;
  Core_Size checker_size_vertical;

  /// @brief
  /// The reference to the first checker color.
  /// Resolves to a dx_assets_color_rgb_n8.
  Core_Assets_Ref* first_checker_color;

  /// @brief
  /// The reference to the second checker color.
  /// Resolves to a dx_assets_color_rgb_n8.
  Core_Assets_Ref* second_checker_color;
};

static inline Core_Assets_ImageOperations_CheckerboardPatternFill_Dispatch* CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL_DISPATCH(void* p) {
  return (Core_Assets_ImageOperations_CheckerboardPatternFill_Dispatch*)p;
}

struct Core_Assets_ImageOperations_CheckerboardPatternFill_Dispatch {
  Core_Assets_ImageOperation_Dispatch _parent;
};

/// @detail The object is constructed with default values.
/// @constructor{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_construct(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);

/// @create-operator{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_create(Core_Assets_ImageOperations_CheckerboardPatternFill** RETURN);



/// @brief Set the number of checkers along the horizontal axis.
/// @param number_of_checkers_horizontal The number of checkers along the horizontal axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_number_of_checkers_horizontal(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size number_of_checkers_horizontal);

/// @brief Get the number of checkers along the horizontal axis.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the number of checkers along the horizontal axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_number_of_checkers_horizontal(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);



/// @brief Set the number of checkers along the vertical axis.
/// @param number_of_checkers_vertical The number of checkers along the vertical axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_number_of_checkers_vertical(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size number_of_checkers_vertical);

/// @brief Get the number of checkers along the vertical axis.
/// @param RETURN A pointer to a <code>Core_Size</code>.
/// @success <code>*RETURN</code> was assigned the number of checkers along the vertical axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_number_of_checkers_vertical(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);



/// @brief Set the size of a checker along the horizontal axis.
/// @param checker_size_horizontal The size of a checker along the horizontal axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_checker_size_horizontal(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size checker_size_horizontal);

/// @brief Get the size of a checker along the horizontal axis.
/// @param RETURN A pointer to the Core_Size variable.
/// @success <code>*RETURN</code> was assigned the size of a checker along the horizontal axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_checker_size_horizontal(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);



/// @brief Set the size of a checker along the vertical axis.
/// @param checker_size_vertical The size of a checker along the vertical axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_checker_size_vertical(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Size checker_size_vertical);

/// @brief Get the size of a checker along the vertical axis.
/// @param RETURN A pointer to the Core_Size variable.
/// @success <code>*RETURN</code> was assigned the size of a checker along the vertical axis.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_checker_size_vertical(Core_Size* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);



/// @brief Set the first checker color.
/// @param first_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_first_checker_color(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Assets_ColorRgbN8* first_checker_color);

/// @brief Get the first checker color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_first_checker_color(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);



/// @brief Set the second checker color.
/// @param second_checker_color A pointer to the DX_RGB_U8 color object providing the color values.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_set_second_checker_color(Core_Assets_ImageOperations_CheckerboardPatternFill* SELF, Core_Assets_ColorRgbN8* second_checker_color);

/// @brief Get the first checker color.
/// @param RETURN A pointer to the DX_RGB_U8 color object receiving the color values.
/// @method{Core_Assets_ImageOperations_CheckerboardPatternFill}
Core_Result Core_Assets_ImageOperations_CheckerboardPatternFill_get_second_checker_color(Core_InlineRgbN8* RETURN, Core_Assets_ImageOperations_CheckerboardPatternFill* SELF);

#endif // CORE_ASSETS_IMAGEOPERATIONS_CHECKERBOARDPATTERNFILL_H_INCLUDED
