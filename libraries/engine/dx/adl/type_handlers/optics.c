#include "dx/adl/type_handlers/optics.h"

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

static dx_asset_optics* _read_optics(dx_ddl_node* node, dx_adl_context* context);

static dx_result read(dx_object**, dx_adl_type_handlers_optics*, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.optics",
                      dx_adl_type_handlers_optics,
                      dx_adl_type_handler);

static dx_asset_optics_perspective* _read_optics_perspective(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_optics_perspective* optics_value = NULL;
  if (dx_asset_optics_perspective_create(&optics_value)) {
    return NULL;
  }
  // fieldOfViewY?
  {
    dx_f32 value;
    dx_string* key = NAME(field_of_view_y_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->near = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // aspectRatio?
  {
    dx_f32 value;
    dx_string* key = NAME(aspect_ratio_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      if (!optics_value->aspect_ratio) {
        if (dx_memory_allocate(&optics_value->aspect_ratio, sizeof(dx_f32))) {
          DX_UNREFERENCE(optics_value);
          optics_value = NULL;
          return NULL;
        }
      }
      *optics_value->aspect_ratio = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      if (optics_value->aspect_ratio) {
        dx_memory_deallocate(optics_value->aspect_ratio);
        optics_value->aspect_ratio = NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // near?
  {
    dx_f32 value;
    dx_string* key = NAME(near_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->near = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // far?
  {
    dx_f32 value;
    dx_string* key = NAME(far_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->far = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(optics_value);
        optics_value = NULL;
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  return optics_value;
}

static dx_asset_optics_orthographic* _read_optics_orthographic(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_optics_orthographic* optics_value = NULL;
  if (dx_asset_optics_orthographic_create(&optics_value)) {
    return NULL;
  }
  // near?
  {
    dx_f32 value;
    dx_string* key = NAME(near_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->near = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  // far?
  {
    dx_f32 value;
    dx_string* key = NAME(far_key);
    if (!dx_adl_semantical_read_f32(node, key, &value)) {
      optics_value->far = value;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        return NULL;
      }
      dx_set_error(DX_NO_ERROR);
    }
  }
  return optics_value;
}

static dx_asset_optics* _read_optics(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* type = dx_adl_semantical_read_type(node, context);
  if (!type) {
    return NULL;
  }
  if (dx_string_is_equal_to(type, NAME(optics_perspective_type))) {
    DX_UNREFERENCE(type);
    type = NULL;
    return DX_ASSET_OPTICS(_read_optics_perspective(node, context));
  } else if (dx_string_is_equal_to(type, NAME(optics_orthographic_type))) {
    DX_UNREFERENCE(type);
    type = NULL;
    return DX_ASSET_OPTICS(_read_optics_orthographic(node, context));
  } else {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    DX_UNREFERENCE(type);
    type = NULL;
    return NULL;
  }
  return NULL;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_optics* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_object* temporary = DX_OBJECT(_read_optics(node,context));
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_optics_construct(dx_adl_type_handlers_optics* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_optics_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_optics_destruct(dx_adl_type_handlers_optics* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_optics_dispatch_construct(dx_adl_type_handlers_optics_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
}

dx_result dx_adl_type_handlers_optics_create(dx_adl_type_handlers_optics** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_optics)
  if (dx_adl_type_handlers_optics_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
