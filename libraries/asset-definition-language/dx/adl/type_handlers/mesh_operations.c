#include "dx/adl/type_handlers/mesh_operations.h"

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

static dx_result resolve(dx_adl_type_handlers_mesh_operations* SELF, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_mesh_operations* SELF, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh_operations",
                      dx_adl_type_handlers_mesh_operations,
                      dx_adl_type_handler);

static dx_result resolve(dx_adl_type_handlers_mesh_operations* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return DX_SUCCESS;
  }
  symbol->resolved = true;
  return DX_SUCCESS;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_mesh_operations* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return DX_FAILURE;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_operations_set_vertex_colors_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_asset_mesh_operations_set_vertex_colors* asset_mesh_operations_set_vertex_colors = NULL;
    if (dx_asset_mesh_operations_set_vertex_colors_create(&asset_mesh_operations_set_vertex_colors)) {
      return DX_FAILURE;
    }
    *RETURN = DX_OBJECT(asset_mesh_operations_set_vertex_colors);
    return DX_SUCCESS;
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
}

dx_result dx_adl_type_handlers_mesh_operations_construct(dx_adl_type_handlers_mesh_operations* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_mesh_operations_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_mesh_operations_destruct(dx_adl_type_handlers_mesh_operations* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_mesh_operations_dispatch_construct(dx_adl_type_handlers_mesh_operations_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  /// @todo Fixme.
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->resolve = (dx_result(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
}

dx_result dx_adl_type_handlers_mesh_operations_create(dx_adl_type_handlers_mesh_operations** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_mesh_operations)
  if (dx_adl_type_handlers_mesh_operations_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
