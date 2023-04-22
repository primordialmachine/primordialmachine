#include "dx/adl/type_handlers/mesh_operations.h"

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

static int resolve(dx_adl_type_handlers_mesh_operations* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_mesh_operations* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh_operations",
                      dx_adl_type_handlers_mesh_operations,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_mesh_operations* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_type_handlers_mesh_operations* self, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(mesh_operations_set_vertex_colors_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_OBJECT(dx_asset_mesh_operations_set_vertex_colors_create());
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
}

int dx_adl_type_handlers_mesh_operations_construct(dx_adl_type_handlers_mesh_operations* self) {
  dx_rti_type* _type = dx_adl_type_handlers_mesh_operations_get_type();
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

static void dx_adl_type_handlers_mesh_operations_destruct(dx_adl_type_handlers_mesh_operations* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_mesh_operations_dispatch_construct(dx_adl_type_handlers_mesh_operations_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_mesh_operations* dx_adl_type_handlers_mesh_operations_create() {
  dx_adl_type_handlers_mesh_operations* self = DX_ADL_TYPE_HANDLERS_MESH_OPERATIONS(dx_object_alloc(sizeof(dx_adl_type_handlers_mesh_operations)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_mesh_operations_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
