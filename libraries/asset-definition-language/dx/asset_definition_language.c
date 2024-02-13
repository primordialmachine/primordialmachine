#include "dx/asset_definition_language.h"

#include "dx/adl/diagnostics.h"
#include "dx/adl/parser.h"
#include "dx/adl/names.h"
#include "dx/adl/enter.h"
#include "dx/adl/resolve.h"
#include "dx/assets.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result _read_scene(dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result _read_scene(dx_ddl_node* node, dx_adl_context* context) {
  // get the mesh instances and viewer instances from the symbols
  Core_InlineHashMapPPIterator iterator;
  Core_InlineHashMapPPIterator_initialize(&iterator, &context->definitions->map);
  Core_Boolean has_entry = false;
  if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &iterator)) {
    Core_InlineHashMapPPIterator_uninitialize(&iterator);
    return Core_Failure;
  }
  while (has_entry) {
    dx_adl_symbol* symbol = NULL;
    if (Core_InlineHashMapPPIterator_getValue(&symbol, &iterator)) {
      Core_InlineHashMapPPIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    Core_Object* asset = symbol->asset;
    if (dx_inline_object_array_append(&context->scene->assets, asset)) {
      return Core_Failure;
    }
    Core_InlineHashMapPPIterator_next(&iterator);
    if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &iterator)) {
      Core_InlineHashMapPPIterator_uninitialize(&iterator);
      return Core_Failure;
    }
  }
  Core_InlineHashMapPPIterator_uninitialize(&iterator);
  return Core_Success;
}

static Core_Result dx_adl_compiler_enter_phase(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_definition_language_diagnostics* diagnostics = NULL;
  if (dx_asset_definition_language_diagnostics_create(&diagnostics)) {
    return Core_Failure;
  }
  dx_asset_definition_language_enter* enter = NULL;
  if (dx_asset_definition_language_enter_create(&enter, diagnostics, context)) {
    CORE_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  if (dx_asset_definition_language_enter_run(enter, node, context)) {
    CORE_UNREFERENCE(enter);
    enter = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(enter);
  enter = NULL;
  return Core_Success;
}

static Core_Result dx_adl_compiler_resolve_phase(dx_adl_context* context) {
  dx_adl_resolve* resolve = NULL;
  if (dx_adl_resolve_create(&resolve, context)) {
    return Core_Failure;
  }
  if (dx_adl_resolve_run(resolve)) {
    return Core_Failure;
  }
  CORE_UNREFERENCE(resolve);
  resolve = NULL;
  return Core_Success;
}

Core_Result dx_adl_compile(dx_assets_scene** RETURN, dx_ddl_node* node) {
  dx_adl_context* context = NULL;
  if (dx_adl_context_create(&context)) {
    return Core_Failure;
  }
  dx_assets_scene* scene = NULL;
  if (dx_assets_scene_create(&scene)) {
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  context->scene = scene;
  if (dx_adl_compiler_enter_phase(node, context)) {
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  if (dx_adl_compiler_resolve_phase(context)) {
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  Core_String* expected_type = NAME(scene_type);
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, expected_type, received_type)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  if (!isEqualTo) {
    CORE_UNREFERENCE(context);
    context = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  if (_read_scene(node, context)) {
    CORE_UNREFERENCE(context);
    context = NULL;
    return Core_Failure;
  }
  {
    // Apply image operations to images.
    // Apply mesh operations to meshes.
    /// @todo
    /// This is called the "compute" phase.
    /// The type handlers should provide an appropriate callback.
    Core_InlineHashMapPPIterator iterator;
    Core_InlineHashMapPPIterator_initialize(&iterator, &context->definitions->map);
    Core_Boolean has_entry = false;
    if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &iterator)) {
      CORE_UNREFERENCE(context);
      context = NULL;
      Core_InlineHashMapPPIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    while (has_entry) {
      dx_adl_symbol* symbol = NULL;
      if (Core_InlineHashMapPPIterator_getValue(&symbol, &iterator)) {
        CORE_UNREFERENCE(context);
        context = NULL;
        Core_InlineHashMapPPIterator_uninitialize(&iterator);
        return Core_Failure;
      }
      Core_Boolean isEqualTo[2];
      if (Core_String_isEqualTo(&isEqualTo[0], symbol->type, NAME(image_type)) ||
          Core_String_isEqualTo(&isEqualTo[1], symbol->type, NAME(mesh_type))) {
        CORE_UNREFERENCE(context);
        context = NULL;
        Core_InlineHashMapPPIterator_uninitialize(&iterator);
        return Core_Failure;
      }
      if (isEqualTo[0] || isEqualTo[1]) {
        dx_adl_type_handler* type_handler = NULL;
        if (Core_InlineHashMapPP_get(&type_handler, &context->type_handlers, symbol->type)) {
          CORE_UNREFERENCE(context);
          context = NULL;
          Core_InlineHashMapPPIterator_uninitialize(&iterator);
          return Core_Failure;
        }
        if (!symbol->asset) {
          CORE_UNREFERENCE(context);
          context = NULL;
          Core_InlineHashMapPPIterator_uninitialize(&iterator);
          return Core_Failure;
        }
        if (isEqualTo[0]) {
          Core_Assets_Image* image = CORE_ASSETS_IMAGE(symbol->asset);
          Core_Size n;
          if (dx_inline_object_array_get_size(&n, &image->operations)) {
            CORE_UNREFERENCE(context);
            context = NULL;
            Core_InlineHashMapPPIterator_uninitialize(&iterator);
            return Core_Failure;
          }
          for (Core_Size i = 0; i < n; ++i) {
            Core_Assets_ImageOperation* operation = NULL;
            if (dx_inline_object_array_get_at((Core_Object**)&operation, &image->operations, i)) {
              CORE_UNREFERENCE(context);
              context = NULL;
              Core_InlineHashMapPPIterator_uninitialize(&iterator);
              return Core_Failure;
            }
            if (Core_Assets_Image_apply(image, 0, 0, image->width, image->height, operation)) {
              CORE_UNREFERENCE(context);
              context = NULL;
              Core_InlineHashMapPPIterator_uninitialize(&iterator);
              return Core_Failure;
            }
          }
        } else if (isEqualTo[1]) {
          // The scene still uses the wrong mesh.
          dx_assets_mesh* mesh = DX_ASSETS_MESH(symbol->asset);
          Core_Size n;
          if (dx_inline_object_array_get_size(&n, &mesh->operations)) {
            CORE_UNREFERENCE(context);
            context = NULL;
            Core_InlineHashMapPPIterator_uninitialize(&iterator);
            return Core_Failure;
          }
          for (Core_Size i = 0; i < n; ++i) {
            dx_asset_mesh_operation* operation = NULL;
            if (dx_inline_object_array_get_at((Core_Object**)&operation, &mesh->operations, i)) {
              CORE_UNREFERENCE(context);
              context = NULL;
              Core_InlineHashMapPPIterator_uninitialize(&iterator);
              return Core_Failure;
            }
            if (dx_asset_mesh_operation_apply(operation, mesh)) {
              CORE_UNREFERENCE(context);
              context = NULL;
              Core_InlineHashMapPPIterator_uninitialize(&iterator);
              return Core_Failure;
            }
          }
        } else {
          CORE_UNREFERENCE(context);
          context = NULL;
          Core_InlineHashMapPPIterator_uninitialize(&iterator);
          return Core_Failure;
        }
      }
      Core_InlineHashMapPPIterator_next(&iterator);
      if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &iterator)) {
        CORE_UNREFERENCE(context);
        context = NULL;
        Core_InlineHashMapPPIterator_uninitialize(&iterator);
        return Core_Failure;
      }
    }
    Core_InlineHashMapPPIterator_uninitialize(&iterator);
  }
  CORE_REFERENCE(scene);
  CORE_UNREFERENCE(context);
  context = NULL;
  *RETURN = scene;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
