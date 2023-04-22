#if !defined(DX_WIC_PLUGIN_H_INCLUDED)
#define DX_WIC_PLUGIN_H_INCLUDED

#include <inttypes.h>
#define DX_WIC_PLUGIN_LIBRARY_NAME "wic-plugin"

/// @brief DX_WC_PLUGIN_PIXEL_FORMAT value.
/// Denotes the following pixel format:
/// Three consecutive component values.
/// The first denotes the intensity of the red component.
/// the second denotes the intensity of the green component.
/// The third denotes the intensity of the blue component.
/// The values are natural number values (including zero) within the bounds of \f$[0,255]\f$
/// where \f$0\f$ is the least intensity and \f$255\f$ is the greatest intensity.
#define DX_WIC_PLUGIN_PIXEL_FORMAT_RN8_GN8_BN8 (1)

/// @brief DX_WC_PLUGIN_PIXEL_FORMAT value.
/// Denotes the following pixel format:
/// Three consecutive Bytes.
/// The first denotes the intensity of the blue component.
/// the second denotes the intensity of the green component.
/// The third denotes the intensity of the red component.
/// The values are natural number values (including zero) within the bounds of \f$[0,255]\f$
/// where \f$0\f$ is the least intensity and \f$255\f$ is the greatest intensity.
#define DX_WIC_PLUGIN_PIXEL_FORMAT_BN8_GN8_RN8 (2)

/// @brief Type of values denoting pixel formats.
typedef uint8_t DX_WIC_PLUGIN_PIXEL_FORMAT;

/// @brief
/// DX_WIC_PLUGIN_IMAGE_FORMAT value.
/// Denotes the "TIFF" image format.
#define DX_WIC_PLUGIN_IMAGE_FORMAT_TIFF (1)

/// @brief
/// DX_WIC_PLUGIN_IMAGE_FORMAT value.
/// Denotes the "PNG" image format.
#define DX_WIC_PLUGIN_IMAGE_FORMAT_PNG (2)

/// @brief Type of values denoting image formats.
typedef uint8_t DX_WIC_PLUGIN_IMAGE_FORMAT;

/// @brief The procedure name of the "write image" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_WIC_PLUGIN_WRITE_IMAGE_PROC_NAME "write_image"

/// @param target_image_format The target image format. Must be one of
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_TIFF
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_PNG
typedef int (DX_WIC_PLUGIN_WRITE_IMAGE_PROC)(char const* path,
                                             void const* source_bytes,
                                             DX_WIC_PLUGIN_PIXEL_FORMAT source_pixel_format,
                                             uint32_t source_stride,
                                             uint32_t source_width,
                                             uint32_t source_height,
                                             DX_WIC_PLUGIN_IMAGE_FORMAT target_image_format);

/// @brief The procedure name of the "write image" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_WIC_PLUGIN_READ_IMAGE_PROC_NAME "read_image"

typedef void DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT;
typedef void *(DX_WIC_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC)(DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context, size_t number_of_bytes);
typedef void (DX_WIC_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC)(DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context, void* bytes);

typedef struct DX_WIC_PLUGIN_MEMORY_MANANGEMENT_ARGS {
  DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context;
  DX_WIC_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate;
  DX_WIC_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate;
} DX_WIC_PLUGIN_ALLOCATION_ARGS;

/// @param source_image_format The source image format. Must be one of
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_TIFF
/// - DX_WIC_PLUGIN_SOURCE_IMAGE_FORMAT_PNG
typedef int (DX_WIC_PLUGIN_READ_IMAGE_PROC)(char const* path,
                                            DX_WIC_PLUGIN_IMAGE_FORMAT source_image_format,
                                            void** target_bytes,
                                            DX_WIC_PLUGIN_PIXEL_FORMAT *target_pixel_format,
                                            uint32_t *target_stride,
                                            uint32_t *target_width,
                                            uint32_t* target_height,
                                            DX_WIC_PLUGIN_MEMORY_MANAGEMENT_CONTEXT*context,
                                            DX_WIC_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate,
                                            DX_WIC_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate);

#endif // DX_WIC_PLUGIN_H_INCLUDED
