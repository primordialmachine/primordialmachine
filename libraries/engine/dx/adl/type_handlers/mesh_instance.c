#include "dx/adl/type_handlers/mesh_instance.h"

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

static int resolve(dx_adl_type_handlers_mesh_instance* SELF,
                   dx_adl_symbol* symbol,
                   dx_adl_context* context);

static dx_result read(dx_object** RETURN,
                      dx_adl_type_handlers_mesh_instance* SELF,
                      dx_ddl_node* node,
                      dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh_instance",
                      dx_adl_type_handlers_mesh_instance,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_mesh_instance* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_mesh_instance* mesh_instance = DX_ASSET_MESH_INSTANCE(symbol->asset);
  if (mesh_instance->mesh_reference) {
    dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, mesh_instance->mesh_reference->name);
    if (!referenced_symbol) {
      return 1;
    }
    mesh_instance->mesh_reference->object = referenced_symbol->asset;
    DX_REFERENCE(mesh_instance->mesh_reference->object);
  }
  symbol->resolved = true;
  return 0;
}

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_mesh_instance* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_mesh_instance* mesh_instance = NULL;
  dx_asset_reference* mesh_reference = NULL;
  DX_MAT4* transformation = NULL;
  // transformation?
  {
    dx_string* name = NULL;
    if (dx_string_create(&name, "transformation", sizeof("transformation") - 1)) {
      return DX_FAILURE;
    }
    dx_error old_error = dx_get_error();
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      if (dx_get_error() == DX_ERROR_NOT_FOUND) {
        dx_set_error(old_error);
      } else {
        return DX_FAILURE;
      }
    } else {
      transformation = dx_adl_semantical_read_translation(child_node, context);
      if (!transformation) {
        return DX_FAILURE;
      }
    }
  }
  // reference
  {
    dx_string* name = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
    if (!name) {
      if (transformation) {
        DX_UNREFERENCE(transformation);
        transformation = NULL;
      }
      return DX_FAILURE;
    }
    if (dx_asset_reference_create(&mesh_reference, name)) {
      if (transformation) {
        DX_UNREFERENCE(transformation);
        transformation = NULL;
      }
      DX_UNREFERENCE(name);
      name = NULL;
      return DX_FAILURE;
    }
    DX_UNREFERENCE(name);
    name = NULL;
  }
  mesh_instance = NULL;
  if (dx_asset_mesh_instance_create(&mesh_instance, mesh_reference)) {
    DX_UNREFERENCE(mesh_reference);
    mesh_reference = NULL;
    if (transformation) {
      DX_UNREFERENCE(transformation);
      transformation = NULL;
    }
    return DX_FAILURE;
  }
  DX_UNREFERENCE(mesh_reference);
  mesh_reference = NULL;
  if (transformation) {
    mesh_instance->world_matrix = *transformation;
    dx_memory_deallocate(transformation);
    transformation = NULL;
  }
  *RETURN = DX_OBJECT(mesh_instance);
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_mesh_instance_construct(dx_adl_type_handlers_mesh_instance* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_mesh_instance_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_ADL_TYPE_HANDLER(SELF)->resolve = (int(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*))&resolve;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_mesh_instance_destruct(dx_adl_type_handlers_mesh_instance* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_mesh_instance_dispatch_construct(dx_adl_type_handlers_mesh_instance_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
}

dx_result dx_adl_type_handlers_mesh_instance_create(dx_adl_type_handlers_mesh_instance** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_mesh_instance)
  if (dx_adl_type_handlers_mesh_instance_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
