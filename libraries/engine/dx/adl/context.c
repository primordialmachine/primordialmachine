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
#include "dx/adl/type_handlers/optics.h"
#include "dx/adl/type_handlers/texture.h"
#include "dx/adl/type_handlers/viewer.h"
#include "dx/adl/type_handlers/viewer_instance.h"
#include "dx/adl/type_handlers/viewer_controllers.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result compare_keys_callback(dx_bool* RETURN, dx_string** key1, dx_string** key2);

static dx_result hash_key_callback(dx_size* RETURN, dx_string** key);

static void key_added_callback(dx_string** key);

static void key_removed_callback(dx_string** key);

static void value_added_callback(dx_object** value);

static void value_removed_callback(dx_object** value);

static dx_result compare_keys_callback(dx_bool* RETURN, dx_string** key1, dx_string** key2) {
  DX_DEBUG_ASSERT(NULL != key1);
  DX_DEBUG_ASSERT(NULL != *key1);
  DX_DEBUG_ASSERT(NULL != key2);
  DX_DEBUG_ASSERT(NULL != *key2);
  *RETURN = dx_string_is_equal_to(*key1, *key2);
  return DX_SUCCESS;
}

static dx_result hash_key_callback(dx_size* RETURN, dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  *RETURN = dx_string_get_hash_value(*key);
  return DX_SUCCESS;
}

static void key_added_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_REFERENCE(*key);
}

static void key_removed_callback(dx_string** key) {
  DX_DEBUG_ASSERT(NULL != key);
  DX_DEBUG_ASSERT(NULL != *key);
  DX_UNREFERENCE(*key);
}

static void value_added_callback(dx_object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_REFERENCE(*value);
}

static void value_removed_callback(dx_object** value) {
  DX_DEBUG_ASSERT(NULL != value);
  DX_DEBUG_ASSERT(NULL != *value);
  DX_UNREFERENCE(*value);
}

DX_DEFINE_OBJECT_TYPE("dx.adl.context",
                       dx_adl_context,
                       dx_object)

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name((self->names), dx_adl_name_index_##name)

static void dx_adl_context_destruct(dx_adl_context* self) {
  if (self->scene) {
    DX_UNREFERENCE(self->scene);
    self->scene = NULL;
  }
  dx_inline_pointer_hashmap_uninitialize(&self->readers);
  DX_UNREFERENCE(self->definitions);
  self->definitions = NULL;
  if (self->names) {
    DX_UNREFERENCE(self->names);
    self->names = NULL;
  }
}

static void dx_adl_context_dispatch_construct(dx_adl_context_dispatch* self)
{/*Intentionally empty.*/}

int dx_adl_context_construct(dx_adl_context* self) {
  dx_rti_type* _type = dx_adl_context_get_type();
  if (!_type) {
    return 1;
  }
  self->names = dx_adl_names_create();
  if (!self->names) {
    return 1;
  }
  self->scene = NULL;
  {
    self->definitions = dx_asset_definitions_create();
    if (!self->definitions) {
      DX_UNREFERENCE(self->names);
      self->names = NULL;
      return 1;
    }
  }
  {
    static DX_INLINE_POINTER_HASHMAP_CONFIGURATION const configuration = {
      .compare_keys_callback = (dx_inline_pointer_hashmap_compare_keys_callback*)&compare_keys_callback,
      .hash_key_callback = (dx_inline_pointer_hashmap_hash_key_callback*)&hash_key_callback,
      .key_added_callback = (dx_inline_pointer_hashmap_key_added_callback*)&key_added_callback,
      .key_removed_callback = (dx_inline_pointer_hashmap_key_removed_callback*)&key_removed_callback,
      .value_added_callback = (dx_inline_pointer_hashmap_value_added_callback*)&value_added_callback,
      .value_removed_callback = (dx_inline_pointer_hashmap_value_removed_callback*)&value_removed_callback,
    };
    if (dx_inline_pointer_hashmap_initialize(&self->readers, &configuration)) {
      DX_UNREFERENCE(self->definitions);
      self->definitions = NULL;
      DX_UNREFERENCE(self->names);
      self->names = NULL;
      return 1;
    }

    #define DEFINE2(NAME, TYPE) \
    { \
      dx_string* k = _get_name((self->names), dx_adl_name_index_##NAME##_type); \
      dx_adl_type_handler* v = (dx_adl_type_handler*)TYPE##_create(); \
      if (!v) { \
        \
          dx_inline_pointer_hashmap_uninitialize(&self->readers);  \
          DX_UNREFERENCE(self->definitions); \
          self->definitions = NULL; \
          DX_UNREFERENCE(self->names); \
          self->names = NULL; \
          return 1; \
      } \
        if (dx_inline_pointer_hashmap_set(&self->readers, k, v)) { \
          \
            DX_UNREFERENCE(v); \
            v = NULL; \
            dx_inline_pointer_hashmap_uninitialize(&self->readers); \
            DX_UNREFERENCE(self->definitions); \
            self->definitions = NULL; \
            DX_UNREFERENCE(self->names); \
            self->names = NULL; \
            return 1; \
        } \
        DX_UNREFERENCE(v); \
        v = NULL; \
    }

  #define DEFINE(NAME) \
    { \
      dx_string* k = _get_name((self->names), dx_adl_name_index_##NAME##_type); \
      dx_adl_type_handler* v = (dx_adl_type_handler*)dx_adl_type_handlers_##NAME##_create(); \
      if (!v) { \
        dx_inline_pointer_hashmap_uninitialize(&self->readers); \
        DX_UNREFERENCE(self->definitions); \
        self->definitions = NULL; \
        DX_UNREFERENCE(self->names); \
        self->names = NULL; \
        return 1; \
      } \
      if (dx_inline_pointer_hashmap_set(&self->readers, k, v)) { \
        DX_UNREFERENCE(v); \
        v = NULL; \
        dx_inline_pointer_hashmap_uninitialize(&self->readers); \
        DX_UNREFERENCE(self->definitions); \
        self->definitions = NULL; \
        DX_UNREFERENCE(self->names); \
        self->names = NULL; \
        return 1; \
      } \
      DX_UNREFERENCE(v); \
      v = NULL; \
    }

    DEFINE(color)
    DEFINE(image)
    DEFINE(image_operations_checkerboard_pattern_fill)
    DEFINE(image_operations_color_fill)
    DEFINE(material)
    DEFINE2(material_controllers_ambient_color, dx_adl_type_handlers_material_controllers)
    DEFINE(mesh)
    DEFINE(mesh_instance)
    DEFINE2(optics_orthographic, dx_adl_type_handlers_optics)
    DEFINE2(optics_perspective, dx_adl_type_handlers_optics)
    DEFINE(texture)
    DEFINE2(mesh_operations_set_vertex_colors, dx_adl_type_handlers_mesh_operations)
    DEFINE(viewer)
    DEFINE(viewer_instance)
    DEFINE2(viewer_controllers_rotate_y, dx_adl_type_handlers_viewer_controllers)

    #undef DEFINE2
    #undef DEFINE
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_adl_context* dx_adl_context_create() {
  dx_rti_type* type = dx_adl_context_get_type();
  if (!type) {
    return NULL;
  }
  dx_adl_context* self = DX_ADL_CONTEXT(dx_object_alloc(sizeof(dx_adl_context)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_context_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_adl_context_add_type_handler(dx_adl_context* self, dx_string* name, dx_adl_type_handler* type_handler) {
  if (!self || !name || !type_handler) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return 1;
  }
  if (dx_inline_pointer_hashmap_set(&self->readers, (dx_inline_pointer_hashmap_key)name, (dx_inline_pointer_hashmap_value)type_handler)) {
    return 1;
  }
  return 0;
}
