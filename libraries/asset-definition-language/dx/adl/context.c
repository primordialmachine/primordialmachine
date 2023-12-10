#include "dx/adl/context.h"

#include "dx/adl/type_handlers/color.h"
#include "dx/adl/type_handlers/image.h"
#include "dx/adl/type_handlers/image_operations_checkerboard_pattern_fill.h"
#include "dx/adl/type_handlers/image_operations_color_fill.h"
#include "dx/adl/type_handlers/material.h"
#include "dx/adl/type_handlers/material_controllers.h"
#include "dx/adl/type_handlers/mesh_instance.h"
#include "dx/adl/type_handlers/mesh_operations.h"
#include "dx/adl/type_handlers/mesh.h"
#include "dx/adl/type_handlers/optics_orthographic.h"
#include "dx/adl/type_handlers/optics_perspective.h"
#include "dx/adl/type_handlers/texture.h"
#include "dx/adl/type_handlers/viewer.h"
#include "dx/adl/type_handlers/viewer_instance.h"
#include "dx/adl/type_handlers/viewer_controllers_rotate_y.h"

#include "dx/adl/diagnostics.h"
#include "dx/adl/parser.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name((self->names), dx_adl_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static Core_Result compare_keys_callback(Core_Boolean* RETURN, Core_String** key1, Core_String** key2);

static Core_Result hash_key_callback(Core_Size* RETURN, Core_String** key);

static void key_added_callback(Core_String** key);

static void key_removed_callback(Core_String** key);

static void value_added_callback(Core_Object** value);

static void value_removed_callback(Core_Object** value);

static Core_Result initialize_type_handlers(dx_adl_context* SELF);

static Core_Result uninitialize_type_handlers(dx_adl_context* SELF);

static Core_Result compare_keys_callback(Core_Boolean* RETURN, Core_String** a, Core_String** b) {
  return Core_String_isEqualTo(RETURN, *a, *b);
}

static Core_Result hash_key_callback(Core_Size* RETURN, Core_String** a) {
  return Core_String_getHashValue(RETURN, *a);
}

static void key_added_callback(Core_String** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_REFERENCE(*key);
}

static void key_removed_callback(Core_String** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_UNREFERENCE(*key);
}

static void value_added_callback(Core_Object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_REFERENCE(*value);
}

static void value_removed_callback(Core_Object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_UNREFERENCE(*value);
}

static Core_Result initialize_type_handlers(dx_adl_context* SELF) {
  static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
    .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&compare_keys_callback,
    .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&hash_key_callback,
    .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&key_added_callback,
    .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&key_removed_callback,
    .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&value_added_callback,
    .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&value_removed_callback,
  };
  if (dx_inline_pointer_hashmap_initialize(&SELF->type_handlers, &configuration)) {
    return Core_Failure;
  }

  #define DEFINE1(NAME) \
    { \
      Core_String* k = _get_name((SELF->names), dx_adl_name_index_##NAME##_type); \
      dx_adl_type_handler* v = NULL; \
      if (dx_adl_type_handlers_##NAME##_create((dx_adl_type_handlers_##NAME**)&v)) { \
        dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers); \
        return Core_Failure; \
      } \
      if (dx_inline_pointer_hashmap_set(&SELF->type_handlers, k, v)) { \
        DX_UNREFERENCE(v); \
        v = NULL; \
        dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers); \
        return Core_Failure; \
      } \
      DX_UNREFERENCE(v); \
      v = NULL; \
    }

  #define DEFINE2(NAME, TYPE) \
    { \
      Core_String* k = _get_name((SELF->names), dx_adl_name_index_##NAME##_type); \
      dx_adl_type_handler* v = NULL; \
      if (TYPE##_create((TYPE**)&v)) { \
        \
          dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers);  \
          return Core_Failure; \
      } \
        if (dx_inline_pointer_hashmap_set(&SELF->type_handlers, k, v)) { \
          \
            DX_UNREFERENCE(v); \
            v = NULL; \
            dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers); \
            return Core_Failure; \
        } \
        DX_UNREFERENCE(v); \
        v = NULL; \
    }

  DEFINE1(color)
  DEFINE1(image)
  DEFINE1(image_operations_checkerboard_pattern_fill)
  DEFINE1(image_operations_color_fill)
  DEFINE1(material)
  DEFINE2(material_controllers_ambient_color, dx_adl_type_handlers_material_controllers)
  DEFINE1(mesh)
  DEFINE1(mesh_instance)
  DEFINE1(optics_orthographic)
  DEFINE1(optics_perspective)
  DEFINE1(texture)
  DEFINE2(mesh_operations_set_vertex_colors, dx_adl_type_handlers_mesh_operations)
  DEFINE1(viewer)
  DEFINE1(viewer_instance)
  DEFINE2(viewer_controllers_rotate_y, dx_adl_type_handlers_viewer_controllers_rotate_y)

  #undef DEFINE2
  #undef DEFINE1
  
  return Core_Success;
}

static Core_Result uninitialize_type_handlers(dx_adl_context* SELF) {
  dx_inline_pointer_hashmap_uninitialize(&SELF->type_handlers);
  return Core_Success;
}


DX_DEFINE_OBJECT_TYPE("dx.adl.context",
                       dx_adl_context,
                       Core_Object);

static void dx_adl_context_destruct(dx_adl_context* SELF) {
  if (SELF->scene) {
    DX_UNREFERENCE(SELF->scene);
    SELF->scene = NULL;
  }
  uninitialize_type_handlers(SELF);
  DX_UNREFERENCE(SELF->parser);
  SELF->parser = NULL;
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
  DX_UNREFERENCE(SELF->definitions);
  SELF->definitions = NULL;
  DX_UNREFERENCE(SELF->names);
  SELF->names = NULL;
}

static void dx_adl_context_constructDispatch(dx_adl_context_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_adl_context_construct(dx_adl_context* SELF) {
  DX_CONSTRUCT_PREFIX(dx_adl_context);
  if (dx_adl_names_create(&SELF->names)) {
    return Core_Failure;
  }
  SELF->scene = NULL;
  if (dx_asset_definitions_create(&SELF->definitions)) {
    DX_UNREFERENCE(SELF->names);
    SELF->names = NULL;
    return Core_Failure;
  }
  if (dx_asset_definition_language_diagnostics_create(&SELF->diagnostics))  {
    DX_UNREFERENCE(SELF->definitions);
    SELF->definitions = NULL;
    DX_UNREFERENCE(SELF->names);
    SELF->names = NULL;
    return Core_Failure;
  }
  if (dx_asset_definition_language_parser_create(&SELF->parser, SELF->diagnostics)) {
    DX_UNREFERENCE(SELF->diagnostics);
    SELF->diagnostics = NULL;
    DX_UNREFERENCE(SELF->definitions);
    SELF->definitions = NULL;
    DX_UNREFERENCE(SELF->names);
    SELF->names = NULL;
    return Core_Failure;
  }
  if (initialize_type_handlers(SELF)) {
    DX_UNREFERENCE(SELF->parser);
    SELF->parser = NULL;
    DX_UNREFERENCE(SELF->diagnostics);
    SELF->diagnostics = NULL;
    DX_UNREFERENCE(SELF->definitions);
    SELF->definitions = NULL;
    DX_UNREFERENCE(SELF->names);
    SELF->names = NULL;
    return Core_Failure;
  }
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_adl_context_create(dx_adl_context** RETURN) {
  DX_CREATE_PREFIX(dx_adl_context);
  if (dx_adl_context_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_adl_context_add_type_handler(dx_adl_context* SELF, Core_String* name, dx_adl_type_handler* type_handler) {
  if (!SELF || !name || !type_handler) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  if (dx_inline_pointer_hashmap_set(&SELF->type_handlers, (dx_inline_pointer_hashmap_key)name, (dx_inline_pointer_hashmap_value)type_handler)) {
    return Core_Failure;
  }
  return Core_Success;
}
