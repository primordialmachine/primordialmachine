#if !defined(DX_SCENES_CREATE_ASSETS_H_INCLUDED)
#define DX_SCENES_CREATE_ASSETS_H_INCLUDED

#include "dx/assets/mesh_instance.h"
#include "dx/assets/scene.h"

/// @brief Create a scene asset.
/// @param path A path to an ADL program text file describing the scene asset.
/// @return A pointer to the scene asset on success. The null pointer on failure.
Core_Result _create_scene_from_file(dx_assets_scene** RETURN, Core_String* path);

#endif // DX_SCENES_CREATE_ASSETS_H_INCLUDED
