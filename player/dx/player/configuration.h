#if !defined(DX_PLAYER_CONFIGURATION_H_INCLUDED)
#define DX_PLAYER_CONFIGURATION_H_INCLUDED

#include "dx/core.h"

/// @brief Value for the configuration variable DX_PLAYER_CONFIGURATION_VISUALS.
/// Indicates Direct3D visuals backend.
#define DX_PLAYER_CONFIGURATION_VISUALS_DIRECT3D (1)

/// @brief Value for the configuration variable DX_PLAYER_CONFIGURATION_VISUALS.
/// Indicates OpenGL visuals backend.
#define DX_PLAYER_CONFIGURATION_VISUALS_OPENGL (2)

/// @brief Value for the configuration variable DX_PLAYER_CONFIGURATION_VISUALS.
/// Indicates Vulkan3D visuals backend.
#define DX_PLAYER_CONFIGURATION_VISUALS_VULKAN3D (3)

/// @brief Configuration variable indicating the visuals backend.
#define DX_PLAYER_CONFIGURATION_VISUALS DX_CONFIGURATION_VISUALS_OPENGL

#endif // DX_PLAYER_CONFIGURATION_H_INCLUDED
