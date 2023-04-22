#if !defined(DX_SCENES_CREATE_ASSETS_H_INCLUDED)
#define DX_SCENES_CREATE_ASSETS_H_INCLUDED

#include "dx/assets/mesh_instance.h"
#include "dx/assets/scene.h"

/// @brief Create a scene asset.
/// @param adl_text A pointer to the ADL program text describing the scene asset.
/// @param adl_text_length The length of the program text pointed to by @a adl_text.
/// @return A pointer to the scene asset on success. The null pointer on failure.
dx_asset_scene* _create_scene_from_text(char const* adl_text, dx_size adl_text_length);

/// @brief Create a scene asset.
/// @param path A path to an ADL program text file describing the scene asset.
/// @return A pointer to the scene asset on success. The null pointer on failure.
dx_asset_scene* _create_scene_from_file(dx_string* path);

#endif // DX_SCENES_CREATE_ASSETS_H_INCLUDED
