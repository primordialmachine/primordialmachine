#include "dx/adl/type_handlers/viewer.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_result resolve(dx_adl_type_handlers_viewer* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_viewer* SELF, dx_ddl_node* node, dx_adl_context* context);

static dx_asset_optics* _read_optics(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_viewer_controller* _read_viewer_controller(dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.viewer",
                      dx_adl_type_handlers_viewer,
                      dx_adl_type_handler);

static dx_asset_optics* _read_optics(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(optics_orthographic_type)) ||
      dx_string_is_equal_to(received_type, NAME(optics_perspective_type))) {
    dx_adl_type_handler* type_handler = NULL;
    if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, received_type)) {
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return NULL;
    }
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_asset_optics* asset_optics = NULL;
    if (dx_adl_type_handler_read((dx_object**)&asset_optics, type_handler, node, context)) {
      return NULL;
    }
    return asset_optics;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_asset_viewer_controller* _read_viewer_controller(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(viewer_controllers_rotate_y_type))) {
    dx_adl_type_handler* type_handler = NULL;
    if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, received_type)) {
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return NULL;
    }
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_asset_viewer_controller* asset_viewer_controller = NULL;
    if (dx_adl_type_handler_read((dx_object**)&asset_viewer_controller, type_handler, node, context)) {
      return NULL;
    }
    return asset_viewer_controller;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_result resolve(dx_adl_type_handlers_viewer* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_viewer* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_viewer* viewer_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    if (dx_adl_semantical_read_name(&name_value, node, context)) {
      return DX_FAILURE;
    }
  }
  if (dx_asset_viewer_create(&viewer_value, name_value)) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  // source?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(source_key));
    if (!child_node) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
    } else {
      dx_assets_vector_3_f32* value;
      if (dx_asset_definition_language_parser_parse_vector_3_f32(&value, child_node, context)) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
      viewer_value->source = value->value;
      DX_UNREFERENCE(value);
      value = NULL;
    }
  }
  // target?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(target_key));
    if (!child_node) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
    } else {
      dx_assets_vector_3_f32* value;
      if (dx_asset_definition_language_parser_parse_vector_3_f32(&value, child_node, context)) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
      viewer_value->target = value->value;
      DX_UNREFERENCE(value);
      value = NULL;
    }
  }
  // up?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(up_key));
    if (!child_node) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
    } else {
      dx_assets_vector_3_f32* value;
      if (dx_asset_definition_language_parser_parse_vector_3_f32(&value, child_node, context)) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
      viewer_value->up = value->value;
      DX_UNREFERENCE(value);
      value = NULL;
    }
  }
  // optics
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(optics_key));
    if (child_node) {
      dx_asset_optics* optics = _read_optics(child_node, context);
      if (!optics) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
      if (viewer_value->optics) {
        DX_UNREFERENCE(viewer_value->optics);
        viewer_value->optics = NULL;
      }
      viewer_value->optics = optics;
    } else {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
    }
  }
  // controller?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(controller_key));
    if (!child_node) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      } else {
        dx_set_error(DX_NO_ERROR);
      }
    } else {
      dx_asset_viewer_controller* controller = _read_viewer_controller(child_node, context);
      if (!controller) {
        DX_UNREFERENCE(viewer_value);
        viewer_value = NULL;
        return DX_FAILURE;
      }
      viewer_value->controller = controller;
    }
  }
  *RETURN = DX_OBJECT(viewer_value);
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_viewer_construct(dx_adl_type_handlers_viewer* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_viewer_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_viewer_destruct(dx_adl_type_handlers_viewer* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_viewer_dispatch_construct(dx_adl_type_handlers_viewer_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_viewer_create(dx_adl_type_handlers_viewer** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_viewer)
  if (dx_adl_type_handlers_viewer_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
