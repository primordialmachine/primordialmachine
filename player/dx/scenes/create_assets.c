#include "dx/scenes/create_assets.h"

#include "dx/adl.h"

/// @brief Create a scene asset.
/// @param RETURN A pointer to a <code>dx_asset_scene*</code> variable.
/// @param adl_text A pointer to the ADL program text describing the scene asset.
/// @param adl_text_length The length of the program text pointed to by @a adl_text.
/// @success <code>*RETURN</code> was assigned a pointer to the created scene asset.
/// @procedure-call
static dx_result _create_scene_from_text(dx_asset_scene** RETURN, char const* adl_text, dx_size adl_text_length);

static dx_result _create_scene_from_text(dx_asset_scene** RETURN, char const* adl_text, dx_size adl_text_length) {
  dx_ddl_node* ddl_node = dx_ddl_compile(adl_text, adl_text_length);
  if (!ddl_node) {
    return DX_FAILURE;
  }
  dx_asset_scene* asset_scene = NULL;
  if (dx_adl_compile(&asset_scene, ddl_node)) {
    DX_UNREFERENCE(ddl_node);
    ddl_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(ddl_node);
  ddl_node = NULL;
  *RETURN = asset_scene;
  return DX_SUCCESS;
}

dx_asset_scene* _create_scene_from_file(dx_string* path) {
  char* p;
  dx_size n;
  if (dx_get_file_contents_s(path, &p, &n)) {
    return NULL;
  }
  dx_asset_scene* scene_asset = NULL;
  if (_create_scene_from_text(&scene_asset, p, n)) {
    dx_memory_deallocate(p);
    p = NULL;
    return NULL;
  }
  dx_memory_deallocate(p);
  p = NULL;
  return scene_asset;
}
