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
    CORE_UNREFERENCE(application);
    application = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(application);
  application = NULL;

  dx_ddl_node* ddl_node = dx_ddl_compile(adl_text, adl_text_length);
  if (!ddl_node) {
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  dx_assets_scene* asset_scene = NULL;
  if (dx_adl_compile(&asset_scene, ddl_node)) {
    CORE_UNREFERENCE(ddl_node);
    ddl_node = NULL;
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(ddl_node);
  ddl_node = NULL;
  CORE_UNREFERENCE(context);
  context = NULL;
  *RETURN = asset_scene; 
  return Core_Success;
}

Core_Result _create_scene_from_file(dx_assets_scene** RETURN, Core_String* path) {
  char* bytes = NULL;
  Core_Size number_of_bytes = 0;
  if (Core_getFileContents(path, &bytes, &number_of_bytes)) {
    return Core_Failure;
  }
  dx_assets_scene* scene_asset = NULL;
  if (_create_scene_from_text(&scene_asset, bytes, number_of_bytes)) {
    Core_Memory_deallocate(bytes);
    bytes = NULL;
    return Core_Failure;
  }
  Core_Memory_deallocate(bytes);
  bytes = NULL;
  *RETURN = scene_asset;
  return Core_Success;
}
