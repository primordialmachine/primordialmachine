#include "dx/adl/type_handlers/viewer_controllers.h"

#include "dx/assets.h"
#include "dx/adl/read.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

static int resolve(dx_adl_type_handlers_viewer_controllers* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_viewer_controllers* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.viewer_controllers",
                      dx_adl_type_handlers_viewer_controllers,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_viewer_controllers* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_type_handlers_viewer_controllers* self, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(viewer_controllers_rotate_y_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;

    dx_f32 degrees_per_second;
    DX_VEC3* source = NULL,
           * target = NULL,
           * up = NULL;
    // source
    {
      source = dx_adl_semantical_read_vector_3_field(node, NAME(source_key), context);
      if (!source) {
        return NULL;
      }
    }
    // target
    {
      target = dx_adl_semantical_read_vector_3_field(node, NAME(target_key), context);
      if (!target) {
        dx_memory_deallocate(source);
        return NULL;
      }
    }
    // up
    {
      up = dx_adl_semantical_read_vector_3_field(node, NAME(up_key), context);
      if (!up) {
        dx_memory_deallocate(target);
        dx_memory_deallocate(source);
        return NULL;
      }
    }
    // degrees per second
    {
      if (dx_adl_semantical_read_f32(node, NAME(degrees_per_second_key), &degrees_per_second)) {
        dx_memory_deallocate(up);
        dx_memory_deallocate(target);
        dx_memory_deallocate(source);
        return NULL;
      }
    }
    dx_asset_viewer_controllers_rotate_y* asset = dx_asset_viewer_controllers_rotate_y_create();
    if (!asset) {
      dx_memory_deallocate(up);
      dx_memory_deallocate(target);
      dx_memory_deallocate(source);
      return NULL;
    }
    asset->source = *source;
    asset->target = *target;
    asset->up = *up;
    asset->degrees_per_second = degrees_per_second;
    return DX_OBJECT(asset);
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
}

int dx_adl_type_handlers_viewer_controllers_construct(dx_adl_type_handlers_viewer_controllers* self) {
  dx_rti_type* _type = dx_adl_type_handlers_viewer_controllers_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(self))) {
    return 1;
  }
  DX_ADL_TYPE_HANDLER(self)->resolve = (int(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
  DX_ADL_TYPE_HANDLER(self)->read = (dx_object * (*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_type_handlers_viewer_controllers_destruct(dx_adl_type_handlers_viewer_controllers* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_viewer_controllers_dispatch_construct(dx_adl_type_handlers_viewer_controllers_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_viewer_controllers* dx_adl_type_handlers_viewer_controllers_create() {
  dx_adl_type_handlers_viewer_controllers* self = DX_ADL_TYPE_HANDLERS_VIEWER_CONTROLLERS(dx_object_alloc(sizeof(dx_adl_type_handlers_viewer_controllers)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_viewer_controllers_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
