#include "dx/adl/type_handlers/material.h"

#include "dx/assets.h"
#include "dx/adl/enter.h"
#include "dx/adl/read.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

static dx_asset_material_controller* _read_material_controller(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context);

static int resolve(dx_adl_type_handlers_material* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_type_handlers_material* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.type_handlers.material",
                      dx_adl_type_handlers_material,
                      dx_adl_type_handler);

static dx_asset_material_controller* _read_material_controller(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(material_controllers_ambient_color_type))) {
    dx_adl_type_handler* reader = NULL;
    if (dx_inline_pointer_hashmap_get(&reader, &context->readers, received_type)) {
      DX_UNREFERENCE(received_type);
      received_type = NULL;
      return NULL;
    }
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    return DX_ASSET_MATERIAL_CONTROLLER(dx_adl_type_handler_read(reader, node, context));
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_material* material_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
    if (!name_value) {
      goto END;
    }
  }

  dx_asset_material* material_value_1 = dx_asset_material_create(name_value);
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  if (!material_value_1) {
    goto END;
  }
  // ambientColor?
  {
    dx_asset_color_rgb_n8* ambient_color = dx_adl_semantical_read_color_instance_field_0(node, true, NAME(ambient_color_key), context);
    if (!ambient_color) {
      if (dx_get_error()) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
    } else {
      if (dx_asset_material_set_ambient_color(material_value_1, ambient_color)) {
        DX_UNREFERENCE(ambient_color);
        ambient_color = NULL;
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
      DX_UNREFERENCE(ambient_color);
      ambient_color = NULL;
    }
  }
  // ambientTexture?
  {
    dx_asset_reference* texture_reference = dx_adl_semantical_read_texture_instance_field(node, true, NAME(ambient_texture_key), context);
    if (!texture_reference) {
      if (dx_get_error()) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
    } else {
      if (dx_asset_material_set_ambient_texture(material_value_1, texture_reference)) {
        DX_UNREFERENCE(texture_reference);
        texture_reference = NULL;
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
      DX_UNREFERENCE(texture_reference);
      texture_reference = NULL;
    }
  }
  // controller?
  {
    dx_ddl_node* child_node = dx_ddl_node_map_get(node, NAME(controller_key));
    if (!child_node) {
      if (dx_get_error() != DX_ERROR_NOT_FOUND) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      } else {
        dx_set_error(DX_NO_ERROR);
      }
    } else {
      dx_asset_material_controller* controller = _read_material_controller(child_node, context);
      if (!controller) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
      material_value_1->controller = controller;// ->controller = controller;
    }
  }
  material_value = material_value_1;
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  return material_value;
}

static int resolve(dx_adl_type_handlers_material* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_material* material = DX_ASSET_MATERIAL(symbol->asset);
  if (!material->ambient_texture_reference) {
    symbol->resolved = true;
    return 0;
  }
  if (material->ambient_texture_reference->object) {
    symbol->resolved = true;
    return 0;
  }
  dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, material->ambient_texture_reference->name);
  if (!referenced_symbol) {
    return 1;
  }
  material->ambient_texture_reference->object = referenced_symbol->asset;
  if (!material->ambient_texture_reference->object) {
    return 1;
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_type_handlers_material* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_material(node, context));
}

int dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* self) {
  dx_rti_type* _type = dx_adl_type_handlers_material_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(self))) {
    return 1;
  }
  DX_ADL_TYPE_HANDLER(self)->resolve = (int (*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*))&resolve;
  DX_ADL_TYPE_HANDLER(self)->read = (dx_object*(*)(dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_type_handlers_material_destruct(dx_adl_type_handlers_material* self)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_material_dispatch_construct(dx_adl_type_handlers_material_dispatch* self)
{/*Intentionally empty.*/}

dx_adl_type_handlers_material* dx_adl_type_handlers_material_create() {
  dx_adl_type_handlers_material* self = DX_ADL_TYPE_HANDLERS_MATERIAL(dx_object_alloc(sizeof(dx_adl_type_handlers_material)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_type_handlers_material_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
