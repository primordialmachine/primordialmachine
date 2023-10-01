#include "dx/adl.h"

#include "dx/adl/read.h"
#include "dx/adl/names.h"
#include "dx/adl/enter.h"
#include "dx/adl/resolve.h"
#include "dx/assets.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _read_scene(dx_ddl_node* node, dx_adl_context* context);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int _read_scene(dx_ddl_node* node, dx_adl_context* context) {
  // get the mesh instances and viewer instances from the symbols
  dx_inline_pointer_hashmap_iterator iterator;
  dx_inline_pointer_hashmap_iterator_initialize(&iterator, &context->definitions->map);
  dx_bool has_entry = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
    return 1;
  }
  while (has_entry) {
    dx_adl_symbol* symbol = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&symbol, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return 1;
    }
    if (true/*dx_string_is_equal_to(symbol->type, NAME(mesh_instance_type)) ||
              dx_string_is_equal_to(symbol->type, NAME(viewer_instance_type))*/) {
      dx_object* asset = symbol->asset;
      if (dx_inline_object_array_append(&context->scene->assets, asset)) {
        return 1;
      }
    }
    dx_inline_pointer_hashmap_iterator_next(&iterator);
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return 1;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  return 0;
}

static int dx_adl_compiler_enter_phase(dx_ddl_node* node, dx_adl_context* context) {
  dx_adl_enter* enter = NULL;
  if (dx_adl_enter_create(&enter, context)) {
    return DX_FAILURE;
  }
  if (dx_adl_enter_run(enter, node, context)) {
    DX_UNREFERENCE(enter);
    enter = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(enter);
  enter = NULL;
  return DX_SUCCESS;
}

static int dx_adl_compiler_resolve_phase(dx_adl_context* context) {
  dx_adl_resolve* resolve = NULL;
  if (dx_adl_resolve_create(&resolve, context)) {
    return DX_FAILURE;
  }
  if (dx_adl_resolve_run(resolve)) {
    return DX_FAILURE;
  }
  DX_UNREFERENCE(resolve);
  resolve = NULL;
  return DX_SUCCESS;
}

dx_asset_scene* dx_adl_compile(dx_ddl_node* node) {
  dx_adl_context* context = NULL;
  if (dx_adl_context_create(&context)) {
    return NULL;
  }
  dx_asset_scene* scene = NULL;
  if (dx_asset_scene_create(&scene)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  context->scene = scene;
  if (dx_adl_compiler_enter_phase(node, context)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  if (dx_adl_compiler_resolve_phase(context)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  dx_string* expected_type = NAME(scene_type);
  if (!dx_string_is_equal_to(expected_type, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    DX_UNREFERENCE(context);
    context = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  if (_read_scene(node, context)) {
    DX_UNREFERENCE(context);
    context = NULL;
    return NULL;
  }
  {
    // Apply image operations to images.
    // Apply mesh operations to meshes.
    /// @todo
    /// This is called the "compute" phase.
    /// The type handlers should provide an appropriate callback.
    dx_inline_pointer_hashmap_iterator iterator;
    dx_inline_pointer_hashmap_iterator_initialize(&iterator, &context->definitions->map);
    dx_bool has_entry = false;
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return NULL;
    }
    while (has_entry) {
      dx_adl_symbol* symbol = NULL;
      if (dx_inline_pointer_hashmap_iterator_get_value(&symbol, &iterator)) {
        dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
        return NULL;
      }
      if (dx_string_is_equal_to(symbol->type, NAME(image_type)) ||
        dx_string_is_equal_to(symbol->type, NAME(mesh_type))) {
        dx_adl_type_handler* reader = NULL;
        if (dx_inline_pointer_hashmap_get(&reader, &context->readers, symbol->type)) {
          dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
          return NULL;
        }
        if (!symbol->asset) {
          dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
          return NULL;
        }
        if (dx_string_is_equal_to(symbol->type, NAME(image_type))) {
          dx_asset_image* image = DX_ASSET_IMAGE(symbol->asset);
          dx_size n;
          if (dx_inline_object_array_get_size(&n, &image->operations)) {
            dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
            return NULL;
          }
          for (dx_size i = 0; i < n; ++i) {
            dx_asset_image_operation* operation = NULL;
            if (dx_inline_object_array_get_at((dx_object**) & operation, &image->operations, i)) {
              dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
              return NULL;
            }
            if (dx_asset_image_apply(image, 0, 0, image->width, image->height, operation)) {
              dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
              return NULL;
            }
          }
        } else if (dx_string_is_equal_to(symbol->type, NAME(mesh_type))) {
          // The scene still uses the wrong mesh.
          dx_asset_mesh* mesh = DX_ASSET_MESH(symbol->asset);
          dx_size n;
          if (dx_inline_object_array_get_size(&n, &mesh->operations)) {
            dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
            return NULL;
          }
          for (dx_size i = 0; i < n; ++i) {
            dx_asset_mesh_operation* operation = NULL;
            if (dx_inline_object_array_get_at((dx_object**) & operation, &mesh->operations, i)) {
              return NULL;
            }
            if (dx_asset_mesh_operation_apply(operation, mesh)) {
              dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
              return NULL;
            }
          }
        } else {
          dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
          return NULL;
        }
      }
      dx_inline_pointer_hashmap_iterator_next(&iterator);
      if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
        dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
        return NULL;
      }
    }
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  }
  DX_REFERENCE(scene);
  DX_UNREFERENCE(context);
  context = NULL;
  return scene;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
