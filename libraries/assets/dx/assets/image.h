#if !defined(DX_ASSETS_IMAGE_H_INCLUDED)
#define DX_ASSETS_IMAGE_H_INCLUDED

#include "dx/core.h"
typedef struct dx_assets_image_operation dx_assets_image_operation;

/// @brief An image asset.
DX_DECLARE_OBJECT_TYPE("dx.assets.image",
                       dx_assets_image,
                       dx_object);

static inline dx_assets_image* DX_ASSETS_IMAGE(void* p) {
  return (dx_assets_image*)p;
}

struct dx_assets_image {
  dx_object _parent;
  
  /// @brief A pointer to the name of this ADL image.
  dx_string* name;
  
  /// @brief The pixel format.
  dx_pixel_format pixel_format;
  
  /// @brief The width, in pixels.
  dx_size width;
  
  /// @brief The height, in pixels.
  dx_size height;
  
  /// @brief A pointer to the pixel data.
  void* pixels;

  /// @brief The operations that eventually create the image.
  dx_inline_object_array operations;
};

static inline dx_assets_image_dispatch* DX_ASSETS_IMAGE_DISPATCH(void* p) {
  return (dx_assets_image_dispatch*)p;
}

struct dx_assets_image_dispatch {
  dx_object_dispatch _parent;
};

/// @details The object is constructed with the specified width, height, and pixel format.
/// @param name A pointer to the ADL name of this image.
/// @param pixel_format The pixel format of the image.
/// @param width The width, in pixels, of the image. Can be @a 0.
/// @param height The height, in pixels, of the image. Can be @a 0.
/// @remarks
/// All pixels are assigned the default pixel value of the specified pixel format.
/// - #DX_PIXEL_FORMAT_RN8_GN8_BN8 default value is the color "Black" (0, 0, 0).
/// - #DX_PIXEL_FORMAT_RN8_GN8_BN8_AN8 default is the color "Opaque Black" (0, 0, 0, 255)
/// - #DX_PIXEL_FORMAT_AN8_RN8_GN8_BN8 default is the color "Opaque Black" (255, 0, 0, 0)
/// - #DX_PIXEL_FORMAT_BN8_GN8_RN8 default value is the color "Black" (0, 0, 0).
/// - #DX_PIXEL_FORMAT_BN8_GN8_RN8_AN8 default is the color "Opaque Black" (0, 0, 0, 255)
/// - #DX_PIXEL_FORMAT_AN8_BN8_GN8_RN8 default is the color "Opaque Black" (255, 0, 0, 0)
/// - #DX_PIXEL_FORMAT_LN8 default is the color "Black" (0)
/// - #DX_PIXEL_FORMAT_LN8_AN8 default is the color "Opaque Black" (0, 255)
/// - #DX_PIXEL_FORMAT_AN8_LN8 default is the color "OpaqueBlack" (255, 0)
/// @constructor{dx_asset_image}
dx_result dx_assets_image_construct(dx_assets_image * SELF,
                                    dx_string* name,
                                    dx_pixel_format pixel_format,
                                    dx_size width,
                                    dx_size height);

dx_result dx_assets_image_create(dx_assets_image** RETURN,
                                 dx_string* name,
                                 dx_pixel_format pixel_format,
                                 dx_size width,
                                 dx_size height);

dx_result dx_assets_image_construct_path(dx_assets_image* SELF,
                                         dx_string* name,
                                         dx_string* path);

dx_result dx_assets_image_create_path(dx_assets_image** RETURN,
                                      dx_string* name,
                                      dx_string* path);

/// @brief Apply to the specified area the specified image operation.
/// @param left The left border of the area to fill.
/// @param top The top border of the area to fill.
/// @param width The width of the area to fill.
/// @param height The height of the area to fill.
/// @param image_operation A pointer to the image operation.
/// @method{dx_asset_image}
dx_result dx_assets_image_apply(dx_assets_image* SELF,
                                dx_size left,
                                dx_size top,
                                dx_size width,
                                dx_size height,
                                dx_assets_image_operation* image_operation);

#endif // DX_ASSETS_IMAGE_H_INCLUDED
