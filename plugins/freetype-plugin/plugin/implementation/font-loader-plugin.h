// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
// @copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

#if !defined(DX_FONT_LOADER_PLUGIN_H_INCLUDED)
#define DX_FONT_LOADER_PLUGIN_H_INCLUDED

#include <inttypes.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef uint8_t dx_font_loader_plugin_result;

/// @brief dx_font_loader_plugin_result value indicating success.
/// This symbolic constant evaluates to a zero integer literal.
#define DX_FONT_LOADER_PLUGIN_SUCCESS (0)

/// @brief dx_font_loader_plugin_result value indicating failure.
/// This symbolic constant evaluates to a non-zero integer literal.
/// This is a generic result value that is used if no other result value is suitable.
/// @todo Remove this.
#define DX_FONT_LOADER_PLUGIN_FAILURE (1)

/// @brief dx_font_loader_plugin_result value indicating failure
/// for the reason that the plugin failed to initialize a backend library (e.g., the freetype library) library.
/// This symbolic constant evaluates to a non-zero integer literal.
#define DX_FONT_LOADER_PLUGIN_FAILED_TO_INITIALIZE_BACKEND_LIBRARY (2)

/// @brief dx_font_loader_plugin_result value indicating failure
/// for the reason that the plugin failed to read a font file.
/// This symbolic constant evaluates to a non-zero integer literal
#define DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_FONT_FILE (3)

/// @brief dx_font_loader_plugin_result value indicating failure
/// for the reason that the plugin failed to allocate memory.
/// This symbolic constant evaluates to a non-zero integer literal
#define DX_FONT_LOADER_PLUGIN_ALLOCATION_FAILED (4)

/// @brief dx_font_loader_plugin_result value indicating failure
/// for the reason that the plugin failed to allocate memory.
/// This symbolic constant evaluates to a non-zero integer literal
#define DX_FONT_LOADER_PLUGIN_INVALID_ARGUMENT (5)

/// @brief dx_font_loader_plugin_result value indicating failure
/// for the reason that the plugin failed to read a glyph.
/// This symbolic constant evaluates to a non-zero integer literal.
#define DX_FONT_LOADER_PLUGIN_FAILED_TO_READ_GLYPH (6)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Opaque type of a "memory management" context.
typedef void DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_CONTEXT;

/// @brief Type of a "memory management allocate" function.
typedef void *(DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC)(DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context, size_t number_of_bytes);
typedef void (DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC)(DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_CONTEXT* context, void* bytes);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The opaque type of a font.
typedef void dx_font_loader_plugin_font;

/// @brief The procedure name of the "reference font" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC_NAME "reference_font"

/// @brief The type of a "reference font" function.
typedef void (DX_FONT_LOADER_PLUGIN_REFERENCE_FONT_PROC)(dx_font_loader_plugin_font*);

/// @brief The procedure name of the "unreference font" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC_NAME "unreference_font"

/// @brief  The type of a "unreference font" function.
typedef void (DX_FONT_LOADER_PLUGIN_UNREFERENCE_FONT_PROC)(dx_font_loader_plugin_font*);

/// @brief The procedure name of the "create font" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC_NAME "create_font"

/// @brief The type of a "create font" function.
typedef dx_font_loader_plugin_result (DX_FONT_LOADER_PLUGIN_CREATE_FONT_PROC)(dx_font_loader_plugin_font** font,
                                                                              char const* path,
                                                                              uint8_t font_size,
                                                                              DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_CONTEXT*context,
                                                                              DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_ALLOCATE_PROC* allocate,
                                                                              DX_FONT_LOADER_PLUGIN_MEMORY_MANAGEMENT_DEALLOCATE_PROC* deallocate);


/// @brief The procedure name of the "get font size" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC_NAME "get_font_size"

/// @brief The type of a "get font size" function.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_FONT_SIZE_PROC)(float* _return, dx_font_loader_plugin_font* font);

/// @brief Procedure name of the "get baseline distance" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC_NAME "get_baseline_distance"

/// @brief The type of a "get baseline distance" function.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_BASELINE_DISTANCE_PROC)(float* _return, dx_font_loader_plugin_font* font);

/// @brief Procedure name of the "get ascender" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_ASCENDER_PROC_NAME "get_ascender"

/// @brief The type of a "get ascender" function.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_ASCENDER_PROC)(float* _return, dx_font_loader_plugin_font* font);

/// @brief Procedure name of the "get descender" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_DESCENDER_PROC_NAME "get_descender"

/// @brief The type of a "get descender" function.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_DESCENDER_PROC)(float* _return, dx_font_loader_plugin_font* font);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief An UTF-8 codepoint.
typedef uint32_t dx_font_loader_plugin_utf8_codepoint;

/// @brief The opaque type of a glyph.
typedef void dx_font_loader_plugin_glyph;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The procedure name of the "reference glyph" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC_NAME "reference_glyph"

/// @brief The type of a "reference font" function.
typedef void (DX_FONT_LOADER_PLUGIN_REFERENCE_GLYPH_PROC)(dx_font_loader_plugin_glyph*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief The procedure name of the "unreference glyph" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC_NAME "unreference_glyph"

/// @brief  The type of a "unreference glyph" function.
typedef void (DX_FONT_LOADER_PLUGIN_UNREFERENCE_GLYPH_PROC)(dx_font_loader_plugin_glyph*);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Procedure name of the "get glyph" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC_NAME "get_glyph"

/// @brief The type of a "get glyph" function.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_GLYPH_PROC)(dx_font_loader_plugin_glyph* _return, dx_font_loader_plugin_font* font, dx_font_loader_plugin_utf8_codepoint codepoint);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Procedure name of the "get glyph size" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_GLYPH_SIZE_PROC_NAME "get_glyph_size"

/// @brief The type of a "get glyph size" function.
/// @success
/// - <code>*size_x</code> was assigned the size, in pixels, of the glyph along the x-axis.
/// - <code>*size_y</code> was assigned the size, in pixels, of the glyph along the y-axis.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_GLYPH_SIZE_PROC)(dx_font_loader_plugin_glyph* glyph, uint32_t *size_x, uint32_t *size_y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Procedure name of the "get glyph pixels" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC_NAME "get_glyph_pixels"

/// @brief The type of a "get glyph pixels" function.
/// @success
/// - <code>*pixels</code> was assigned a pointer to the pixels of the glyph.
///   The number of pixels is the product of the size of the glyph along the x-axis with the size of the glyph along the y-axis.
///   Each pixel is a uin8t_t grayscale value where a value of 0 means "black" and a value of 255 means "white".
///   The pointer remains valid as long as the dx_font_loader_plugin_glyph object remains valid.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_GLYPH_PIXELS_PROC)(dx_font_loader_plugin_glyph* glyph, void **pixels);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Procedure name of the "get glyph advance" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC_NAME "get_glyph_advance"

/// @brief The type of a "get glyph advance" function.
/// @success
/// - <code>*advance_x</code> was assigned the advance, in pixels, along the x-axis.
/// - <code>*advance_y</code> was assigned the advance, in pixels, along the y-axis.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_GLYPH_ADVANCE_PROC)(dx_font_loader_plugin_glyph* glyph, float *advance_x, float* advance_y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Procedure name of the "get glyph bearing" function.
/// This is usually passed to "GetProcAddress" or "dlsym".
#define DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC_NAME "get_glyph_bearing"

/// @brief The type of a "get glyph bearing" function.
/// @success
/// - <code>*bearing_x</code> was assigned the bearing, in pixels, along the x-axis.
/// - <code>*bearing_y</code> was assigned the bearing, in pixels, along the y-axis.
typedef dx_font_loader_plugin_result(DX_FONT_LOADER_PLUGIN_GET_GLYPH_BEARING_PROC)(dx_font_loader_plugin_glyph* glyph, float* bearing_x, float* bearing_y);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_FONT_LOADER_PLUGIN_H_INCLUDED
