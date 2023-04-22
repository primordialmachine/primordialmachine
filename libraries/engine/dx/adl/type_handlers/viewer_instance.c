#include "dx/adl/type_handlers/viewer_instance.h"

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

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.viewer_instance",
                      dx_adl_type_handlers_viewer_instance,
                      dx_adl_type_handler);

static int resolve(dx_adl_type_handlers_viewer_instance* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_viewer_instance* self, dx_ddl_node* node, dx_adl_context* context);

static dx_asset_viewer* _read_viewer(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(viewer_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_type_handler* reader = NULL;
    if (dx_inline_pointer_hashmap_get(&reader, &context->readers, NAME(viewer_type))) {
      return NULL;
    }
    return  DX_ASSET_VIEWER(dx_adl_type_handler_read(reader, node, context));
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
    viewer_reference = dx_asset_reference_create(name);
    DX_UNREFERENCE(name);
    name = NULL;
    if (!viewer_reference) {
      return NULL;
    }
  }
  dx_asset_viewer_instance* viewer_instance_value = dx_asset_viewer_instance_create(viewer_reference);
  DX_UNREFERENCE(viewer_reference);
  viewer_reference = NULL;
  return viewer_instance_value;
}

static int resolve(dx_adl_type_handlers_viewer_instance* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_viewer_instance* viewer_instance = DX_ASSET_VIEWER_INSTANCE(symbol->asset);
  if (viewer_instance->viewer_reference) {
    dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, viewer_instance->viewer_reference->name);
    if (!referenced_symbol) {
      return 1;
    }
    viewer_instance->viewer_reference->object = referenced_symbol->asset;
    DX_REFERENCE(viewer_instance->viewer_reference->object);
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_type_handlers_viewer_instance* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_viewer_instance(node, context));
}

int dx_adl_type_handlers_viewer_instance_construct(dx_adl_type_handlers_viewer_instance* self) {
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(self))) {
    return 1;
  }
  dx_rti_type* _type = dx_adl_type_handlers_viewer_instance_get_type();
  if (!_type) {
    return 1;
  }
  DX_ADL_TYPE_HANDLER(self)->resolve = (int (*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*)) & resolve;
  DX_ADL_TYPE_HANDLER(self)->read = (dx_object * (*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_type_handlers_viewer_instance_destruct(dx_adl_type_handlers_viewer_instance* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_viewer_instance_dispatch_construct(dx_adl_type_handlers_viewer_instance_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_viewer_instance* dx_adl_type_handlers_viewer_instance_create() {
  dx_adl_type_handlers_viewer_instance* self = DX_ADL_TYPE_HANDLERS_VIEWER_INSTANCE(dx_object_alloc(sizeof(dx_adl_type_handlers_viewer_instance)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_viewer_instance_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
