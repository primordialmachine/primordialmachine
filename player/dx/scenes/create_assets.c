#include "dx/scenes/create_assets.h"

#include "dx/asset_definition_language.h"
#include "dx/application.h"

/// @brief Create a scene asset.
/// @param RETURN A pointer to a <code>dx_asset_scene*</code> variable.
/// @param adl_text A pointer to the ADL program text describing the scene asset.
/// @param adl_text_length The length of the program text pointed to by @a adl_text.
/// @success <code>*RETURN</code> was assigned a pointer to the created scene asset.
/// @procedure
static Core_Result _create_scene_from_text(dx_assets_scene** RETURN, char const* adl_text, Core_Size adl_text_length);

static Core_Result _create_scene_from_text(dx_assets_scene** RETURN, char const* adl_text, Core_Size adl_text_length) {
  dx_application* application = NULL;
  if (dx_application_get(&application)) {
    return Core_Failure;
  }
  dx_assets_context* context = NULL;
  if (dx_application_get_assets_context(&context, application)) {
    DX_UNREFERENCE(application);
    application = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(application);
  application = NULL;

  dx_ddl_node* ddl_node = dx_ddl_compile(adl_text, adl_text_length);
  if (!ddl_node) {
    DX_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  dx_assets_scene* asset_scene = NULL;
  if (dx_adl_compile(&asset_scene, ddl_node)) {
    DX_UNREFERENCE(ddl_node);
    ddl_node = NULL;
    DX_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  DX_UNREFERENCE(ddl_node);
  ddl_node = NULL;
  DX_UNREFERENCE(context);
  context = NULL;
  *RETURN = asset_scene; 
  return Core_Success;
}

Core_Result _create_scene_from_file(dx_assets_scene** RETURN, Core_String* path) {
  char* p;
  Core_Size n;
  if (dx_get_file_contents_s(path, &p, &n)) {
    return Core_Failure;
  }
  dx_assets_scene* scene_asset = NULL;
  if (_create_scene_from_text(&scene_asset, p, n)) {
    Core_Memory_deallocate(p);
    p = NULL;
    return Core_Failure;
  }
  Core_Memory_deallocate(p);
  p = NULL;
  *RETURN = scene_asset;
  return Core_Success;
}
