#if !defined(DX_ASSETS_IMAGE_H_INCLUDED)
#define DX_ASSETS_IMAGE_H_INCLUDED

#include "dx/core.h"
typedef struct dx_assets_image_operation dx_assets_image_operation;

/// @brief An image asset.
DX_DECLARE_OBJECT_TYPE("Core.Assets.Image",
                       dx_assets_image,
                       Core_Object);

static inline dx_assets_image* DX_ASSETS_IMAGE(void* p) {
  return (dx_assets_image*)p;
}

struct dx_assets_image {
  Core_Object _parent;
  
  /// @brief A pointer to the name of this ADL image.
  Core_String* name;
  
  /// @brief The pixel format.
  Core_PixelFormat pixel_format;
  
  /// @brief The width, in pixels.
  Core_Size width;
  
  /// @brief The height, in pixels.
  Core_Size height;
  
  /// @brief A pointer to the pixel data.
  void* pixels;

  /// @brief The operations that eventually create the image.
  dx_inline_object_array operations;
};

static inline dx_assets_image_dispatch* DX_ASSETS_IMAGE_DISPATCH(void* p) {
  return (dx_assets_image_dispatch*)p;
}

struct dx_assets_image_dispatch {
  Core_Object_Dispatch _parent;
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
/// - #Core_PixelFormat_Bgr8       default value is the color "Black" (0, 0, 0).
/// - #Core_PixelFormat_Bgra8      default is the color "Opaque Black" (0, 0, 0, 255)
/// - #Core_PixelFormat_L8         default is the color "Black" (0)
/// - #Core_PixelFormat_La8        default is the color "Opaque Black" (0, 255)
/// - #Core_PixelFormat_Rgb8       default value is the color "Black" (0, 0, 0).
/// - #Core_PixelFormat_Rgba8      default is the color "Opaque Black" (0, 0, 0, 255)
/// @constructor{dx_asset_image}
Core_Result dx_assets_image_construct(dx_assets_image * SELF,
                                      Core_String* name,
                                      Core_PixelFormat pixel_format,
                                      Core_Size width,
                                      Core_Size height);

Core_Result dx_assets_image_create(dx_assets_image** RETURN,
                                   Core_String* name,
                                   Core_PixelFormat pixel_format,
                                   Core_Size width,
                                   Core_Size height);

Core_Result dx_assets_image_construct_path(dx_assets_image* SELF,
                                           Core_String* name,
                                           Core_String* path);

Core_Result dx_assets_image_create_path(dx_assets_image** RETURN,
                                        Core_String* name,
                                        Core_String* path);

/// @brief Apply to the specified area the specified image operation.
/// @param left The left border of the area to fill.
/// @param top The top border of the area to fill.
/// @param width The width of the area to fill.
/// @param height The height of the area to fill.
/// @param image_operation A pointer to the image operation.
/// @method{dx_asset_image}
Core_Result dx_assets_image_apply(dx_assets_image* SELF,
                                  Core_Size left,
                                  Core_Size top,
                                  Core_Size width,
                                  Core_Size height,
                                  dx_assets_image_operation* image_operation);

#endif // DX_ASSETS_IMAGE_H_INCLUDED
