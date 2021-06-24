#if !defined(MACHINE_VIDEO_H_INCLUDED)
#define MACHINE_VIDEO_H_INCLUDED

#include "_Math.h"
#include "_Video.h"
#include "./UtilitiesGL.h"
typedef struct Machine_Images_Image Machine_Images_Image;
typedef struct Machine_Texture Machine_Texture;
typedef struct Machine_ShaderProgram Machine_ShaderProgram;

/// @brief Increment the video module by @a 1.
/// FAILS if the reference count is INT_MAX.
/// @return @a 0 on success, a non-zero value on failure.
/// @remarks
/// Initializing/Uninitializing the video module also
/// initializes/uninitializes the following modules:
/// - images
/// - fonts
int Machine_Video_startup();

/// @brief Decrement the video module by @a 1.
/// UNDEFINED if the reference count is @a 0.
void Machine_Video_shutdown();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Assign a texture unit.
/// @param unit The index of the texture unit.
/// @param texture A pointer to the texture or the null pointer.
void Machine_Video_bindTexture(size_t unit, Machine_Texture* texture);

/// @brief Create a texture from an image.
/// @param image The image.
/// @return The texture.
Machine_Texture* Machine_Video_createTextureFromImage(Machine_Images_Image* image);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Assign a shader program.
/// @param shaderProgram The shader program.
void Machine_Video_bindShaderProgram(Machine_ShaderProgram* shaderProgram);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Set the clear color.
/// @param clearColor The clear color.
void Machine_Video_setClearColor(Machine_Math_Vector4 *clearColor);

/// @brief Get the clear color.
/// @return The clear color.
Machine_Math_Vector4* Machine_Video_getClearColor();


/// @brief Set the blend function for the incoming color.
/// @param incomingBlendFunction The blend function for the incoming color.
void Machine_Video_setIncomingBlendFunction(Machine_BlendFunction incomingBlendFunction);

/// @brief Get the blend function for the incoming color.
/// @return The blend function for the incoming color.
Machine_BlendFunction Machine_Video_getIncomingBlendFunction();


/// @brief Set the blend function for the existing color.
/// @param existingBlendFunction The blend function for the existing color.
void Machine_Video_setExistingBlendFunction(Machine_BlendFunction existingBlendFunction);

/// @brief Get the blend function for the existing color.
/// @return The blend function for the existing color.
Machine_BlendFunction Machine_Video_getExistingBlendFunction();


/// @brief Set if depth write is enabled/disabled.
/// @param depthWriteEnabled @a true enables depth write, @a false disables depth write.
void Machine_Video_setDepthWriteEnabled(bool depthWriteEnabled);

/// @brief Get if depth write is enabled/disabled.
/// @return @a true if depth write is enabled, @a false if depth write is disabled.
bool Machine_Video_getDepthWriteEnabled();


/// @brief Set the depth test function.
/// @param depthTestFunction The depth test function.
void Machine_Video_setDepthTestFunction(Machine_DepthTestFunction depthTestFunction);

/// @brief Get the depth test function.
/// @return The depth test function.
Machine_DepthTestFunction Machine_Video_getDepthTestFunction();


/// @brief Set the clear depth.
/// @param clearDepth The clear depth.
void Machine_Video_setClearDepth(float clearDepth);

/// @brief Get the clear depth.
/// @return The clear depth.
float Machine_Video_getClearDepth();


GLFWwindow* Machine_Video_getMainWindow();


#endif // MACHINE_VIDEO_H_INCLUDED
