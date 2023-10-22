#include "dx/adl/type_handlers/viewer_controllers.h"

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

static dx_result resolve(dx_adl_type_handlers_viewer_controllers* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_viewer_controllers* SELF, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.viewer_controllers",
                      dx_adl_type_handlers_viewer_controllers,
                      dx_adl_type_handler);

static dx_result resolve(dx_adl_type_handlers_viewer_controllers* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_viewer_controllers* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return DX_FAILURE;
  }
  if (dx_string_is_equal_to(received_type, NAME(viewer_controllers_rotate_y_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;

    dx_f32 degrees_per_second;
    dx_assets_vector_3_f32* source = NULL,
                          * target = NULL,
                          * up = NULL;
    // source
    {
      if (dx_asset_definition_language_parser_parse_vector_3_f32_field(&source, node, NAME(source_key), context)) {
        return DX_FAILURE;
      }
    }
    // target
    {
      if (dx_asset_definition_language_parser_parse_vector_3_f32_field(&target, node, NAME(target_key), context)) {
        DX_UNREFERENCE(source);
        source = NULL;
        return DX_FAILURE;
      }
    }
    // up
    {
      if (dx_asset_definition_language_parser_parse_vector_3_f32_field(&up, node, NAME(up_key), context)) {
        DX_UNREFERENCE(target);
        target = NULL;
        DX_UNREFERENCE(source);
        source = NULL;
        return DX_FAILURE;
      }
    }
    // degrees per second
    {
      if (dx_adl_semantical_read_f32(&degrees_per_second, node, NAME(degrees_per_second_key))) {
        DX_UNREFERENCE(up);
        up = NULL;
        DX_UNREFERENCE(target);
        target = NULL;
        DX_UNREFERENCE(source);
        source = NULL;
        return DX_FAILURE;
      }
    }
    dx_asset_viewer_controllers_rotate_y* asset = NULL;
    if (dx_asset_viewer_controllers_rotate_y_create(&asset)) {
      DX_UNREFERENCE(up);
      up = NULL;
      DX_UNREFERENCE(target);
      target = NULL;
      DX_UNREFERENCE(source);
      source = NULL;
      return DX_FAILURE;
    }
    asset->source = source->value;
    DX_UNREFERENCE(source);
    source = NULL;
    asset->target = target->value;
    DX_UNREFERENCE(target);
    target = NULL;
    asset->up = up->value;
    DX_UNREFERENCE(up);
    up = NULL;
    asset->degrees_per_second = degrees_per_second;
    *RETURN = DX_OBJECT(asset);
    return DX_SUCCESS;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
}

dx_result dx_adl_type_handlers_viewer_controllers_construct(dx_adl_type_handlers_viewer_controllers* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_viewer_controllers_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = _type;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_viewer_controllers_destruct(dx_adl_type_handlers_viewer_controllers* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_viewer_controllers_dispatch_construct(dx_adl_type_handlers_viewer_controllers_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_viewer_controllers_create(dx_adl_type_handlers_viewer_controllers** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_viewer_controllers)
  if (dx_adl_type_handlers_viewer_controllers_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}