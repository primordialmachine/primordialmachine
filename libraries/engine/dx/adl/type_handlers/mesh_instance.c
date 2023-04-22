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

static int resolve(dx_adl_type_handlers_mesh_instance* self,
                   dx_adl_symbol* symbol,
                   dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_mesh_instance* self,
                       dx_ddl_node* node,
                       dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.mesh_instance",
                      dx_adl_type_handlers_mesh_instance,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_mesh_instance* self, dx_adl_symbol* symbol, dx_adl_context* context) {
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

static dx_object* read(dx_adl_type_handlers_mesh_instance* self, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_mesh_instance* mesh_instance = NULL;
  dx_asset_reference* mesh_reference = NULL;
  DX_MAT4* transformation = NULL;
  // transformation?
  {
    dx_string* name = NULL;
    if (dx_string_create(&name, "transformation", sizeof("transformation") - 1)) {
      goto END;
    }
    dx_error old_error = dx_get_error();
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!child_node) {
      if (dx_get_error() == DX_ERROR_NOT_FOUND) {
        dx_set_error(old_error);
      } else {
        goto END;
      }
    } else {
      transformation = dx_adl_semantical_read_translation(child_node, context);
      if (!transformation) {
        goto END;
      }
    }
  }
  // reference
  {
    dx_string* name = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
    if (!name) {
      goto END;
    }
    mesh_reference = dx_asset_reference_create(name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!mesh_reference) {
      goto END;
    }
  }
  mesh_instance = dx_asset_mesh_instance_create(mesh_reference);
  DX_UNREFERENCE(mesh_reference);
  mesh_reference = NULL;
  if (!mesh_instance) {
    goto END;
  }
  if (transformation) {
    mesh_instance->world_matrix = *transformation;
  }
END:
  if (mesh_reference) {
    DX_UNREFERENCE(mesh_reference);
    mesh_reference = NULL;
  }
  if (transformation) {
    dx_memory_deallocate(transformation);
    transformation = NULL;
  }
  return DX_OBJECT(mesh_instance);
}

int dx_adl_type_handlers_mesh_instance_construct(dx_adl_type_handlers_mesh_instance* self) {
  dx_rti_type* _type = dx_adl_type_handlers_mesh_instance_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(self))) {
    return 1;
  }
  DX_ADL_TYPE_HANDLER(self)->resolve = (int(*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*))&resolve;
  DX_ADL_TYPE_HANDLER(self)->read = (dx_object*(*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_type_handlers_mesh_instance_destruct(dx_adl_type_handlers_mesh_instance* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_mesh_instance_dispatch_construct(dx_adl_type_handlers_mesh_instance_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_mesh_instance* dx_adl_type_handlers_mesh_instance_create() {
  dx_adl_type_handlers_mesh_instance* self = DX_ADL_TYPE_HANDLERS_MESH_INSTANCE(dx_object_alloc(sizeof(dx_adl_type_handlers_mesh_instance)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_mesh_instance_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
