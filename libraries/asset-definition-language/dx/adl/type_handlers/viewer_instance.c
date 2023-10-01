#include "dx/adl/type_handlers/viewer_instance.h"

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

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.viewer_instance",
                      dx_adl_type_handlers_viewer_instance,
                      dx_adl_type_handler);

static dx_result resolve(dx_adl_type_handlers_viewer_instance* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node, dx_adl_context* context);

static dx_asset_viewer* _read_viewer(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(viewer_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_type_handler* type_handler = NULL;
    if (dx_inline_pointer_hashmap_get(&type_handler, &context->type_handlers, NAME(viewer_type))) {
      return NULL;
    }
    dx_asset_viewer* asset_viewer = NULL;
    if (dx_adl_type_handler_read((dx_object**)&asset_viewer, type_handler, node, context)) {
      return NULL;
    }
    return asset_viewer;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_asset_viewer_instance* _read_viewer_instance(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_reference* viewer_reference = NULL;
  // reference
  {
    dx_string* name = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
    if (!name) {
      return NULL;
    }
    if (dx_asset_reference_create(&viewer_reference, name)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return NULL;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  dx_asset_viewer_instance* viewer_instance_value = NULL;
  if (dx_asset_viewer_instance_create(&viewer_instance_value, viewer_reference)) {
    DX_UNREFERENCE(viewer_reference);
    viewer_reference = NULL;
    return NULL;
  }
  DX_UNREFERENCE(viewer_reference);
  viewer_reference = NULL;
  return viewer_instance_value;
}

static dx_result resolve(dx_adl_type_handlers_viewer_instance* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  dx_asset_viewer_instance* viewer_instance = DX_ASSET_VIEWER_INSTANCE(symbol->asset);
  if (viewer_instance->viewer_reference) {
    dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, viewer_instance->viewer_reference->name);
    if (!referenced_symbol) {
      return DX_FAILURE;
    }
    viewer_instance->viewer_reference->object = referenced_symbol->asset;
    DX_REFERENCE(viewer_instance->viewer_reference->object);
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_viewer_instance* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_object* temporary = DX_OBJECT(_read_viewer_instance(node, context));
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_viewer_instance_construct(dx_adl_type_handlers_viewer_instance* SELF) {
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  dx_rti_type* TYPE = dx_adl_type_handlers_viewer_instance_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_viewer_instance_destruct(dx_adl_type_handlers_viewer_instance* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_viewer_instance_dispatch_construct(dx_adl_type_handlers_viewer_instance_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_viewer_instance_create(dx_adl_type_handlers_viewer_instance** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_viewer_instance)
  if (dx_adl_type_handlers_viewer_instance_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
