// Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_ASSETS_IMAGE_H_INCLUDED)
#define CORE_ASSETS_IMAGE_H_INCLUDED

#include "Core/Assets/Def.h"
#include "Core/Visuals/SoftwarePixelBuffer.h"
typedef struct Core_Assets_ImageOperation Core_Assets_ImageOperation;

/* http://localhost/assets#core-assets-image */
Core_declareObjectType("Core.Assets.Image",
                       Core_Assets_Image,
                       Core_Assets_Def);

static inline Core_Assets_Image* CORE_ASSETS_IMAGE(void* p) {
  return (Core_Assets_Image*)p;
}

struct Core_Assets_Image {
  Core_Assets_Def _parent;
  
  /// @brief A pointer to the name of this image asset.
  Core_String* name;

  /// @brief The backing software pixel buffer.
  Core_SoftwarePixelBuffer backing;

  /// @brief The operations that eventually create the image.
  dx_inline_object_array operations;
};

static inline Core_Assets_Image_Dispatch* CORE_ASSETS_IMAGE_DISPATCH(void* p) {
  return (Core_Assets_Image_Dispatch*)p;
}

struct Core_Assets_Image_Dispatch {
  Core_Assets_Def_Dispatch _parent;
};

/// @details
/// The image is constructed with the specified width, height, and pixel format.
/// @param name
/// A pointer to the ADL name of this image.
/// @param pixelFormat
/// The pixel format of the image.
/// @param width
/// The width, in pixels, of the image. Can be @a 0.
/// @param height
/// The height, in pixels, of the image. Can be @a 0.
/// @remarks
/// All pixels are assigned the default pixel value of the specified pixel format.
/// - #Core_PixelFormat_Abgr8      default is the color "Opaque Black" (255, 0, 0, 0)
/// - #Core_PixelFormat_Al8        default is the color "OpaqueBlack" (255, 0)
/// - #Core_PixelFormat_Argb8      default is the color "Opaque Black" (255, 0, 0, 0)
/// - #Core_PixelFormat_Bgr8       default is the color "Black" (0, 0, 0).
/// - #Core_PixelFormat_Bgra8      default is the color "Opaque Black" (0, 0, 0, 255)
/// - #Core_PixelFormat_L8         default is the color "Black" (0)
/// - #Core_PixelFormat_La8        default is the color "Opaque Black" (0, 255)
/// - #Core_PixelFormat_Rgb8       default is the color "Black" (0, 0, 0).
/// - #Core_PixelFormat_Rgba8      default is the color "Opaque Black" (0, 0, 0, 255)
/// @constructor{dx_asset_image}
Core_Result Core_Assets_Image_construct(Core_Assets_Image * SELF,
                                        Core_String* name,
                                        Core_PixelFormat pixelFormat,
                                        Core_Size width,
                                        Core_Size height);

Core_Result Core_Assets_Image_create(Core_Assets_Image** RETURN,
                                     Core_String* name,
                                     Core_PixelFormat pixelFormat,
                                     Core_Size width,
                                     Core_Size height);

Core_Result Core_Assets_Image_construct_path(Core_Assets_Image* SELF,
                                             Core_String* name,
                                             Core_String* path);

Core_Result Core_Assets_Image_create_path(Core_Assets_Image** RETURN,
                                          Core_String* name,
                                          Core_String* path);

/// @brief Apply to the specified area the specified image operation.
/// @param left The left border of the area to fill.
/// @param top The top border of the area to fill.
/// @param width The width of the area to fill.
/// @param height The height of the area to fill.
/// @param image_operation A pointer to the image operation.
/// @method{dx_asset_image}
Core_Result Core_Assets_Image_apply(Core_Assets_Image* SELF,
                                    Core_Size left,
                                    Core_Size top,
                                    Core_Size width,
                                    Core_Size height,
                                    Core_Assets_ImageOperation* image_operation);

#endif // CORE_ASSETS_IMAGE_H_INCLUDED
