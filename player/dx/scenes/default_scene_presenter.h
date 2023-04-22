#if !defined(DX_DEFAULT_SCENE_PRESENTER_H_INCLUDED)
#define DX_DEFAULT_SCENE_PRESENTER_H_INCLUDED

#include "dx/scene_presenter.h"

#include "dx/val/command.h"
#include "dx/val/mesh.h"
#include "dx/val/mesh_instance.h"

#include "dx/assets.h"

/// @brief The default scene presenter
DX_DECLARE_OBJECT_TYPE("dx.default_scene_presenter",
                       dx_default_scene_presenter,
                       dx_scene_presenter);

static inline dx_default_scene_presenter* DX_DEFAULT_SCENE_PRESENTER(void *p) {
  return (dx_default_scene_presenter*)p;
}

struct dx_default_scene_presenter {
  dx_scene_presenter _parent;
  /// @brief A pointer to the dx_asset_scene.
  dx_asset_scene* asset_scene;
  /// @brief A pointer to an UTF-8 C string. The path of the ADL file to load.
  dx_string* path;
  /// @brief List of dx_val_mesh_instance objects to render.
  dx_inline_object_array mesh_instances;
  /// @brief List of dx_val_viewer objects.
  dx_inline_object_array viewers;
  /// @brief List of dx_asset_matrial objects.
  dx_inline_object_array asset_material_objects;
  /// @brief The commands to be executed when entering a frame.
  dx_val_command_list* commands;
};

static inline dx_default_scene_presenter_dispatch* DX_DEFAULT_SCENE_PRESENTER_DISPATCH(void* p) {
  return (dx_default_scene_presenter_dispatch*)p;
}

struct dx_default_scene_presenter_dispatch {
  dx_scene_presenter_dispatch _parent;
};

/// @brief Construct this default scene presenter.
/// @param SELF A pointer to this default scene presenter.
/// @param path A pointer to an UTF-8 C string. The path of the ADL file to load.
/// @default-return
/// @default-failure
dx_result dx_default_scene_presenter_construct(dx_default_scene_presenter* SELF, dx_string* path);

dx_result dx_default_scene_presenter_create(dx_default_scene_presenter** RETURN, dx_string* path);

#endif // DX_DEFAULT_SCENE_PRESENTER_H_INCLUDED
