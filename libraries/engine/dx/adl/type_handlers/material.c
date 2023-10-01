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

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context);

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
    dx_asset_material_controller* asset_material_controller = NULL;
    if (dx_adl_type_handler_read((dx_object**)&asset_material_controller, reader, node, context)) {
      return NULL;
    }
    return asset_material_controller;
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

  dx_asset_material* material_value_1 = NULL;
  if (dx_asset_material_create(&material_value_1, name_value)) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
    goto END;
  }
  DX_UNREFERENCE(name_value);
  name_value = NULL;
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

static int resolve(dx_adl_type_handlers_material* SELF, dx_adl_symbol* symbol, dx_adl_context* context) {
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

static dx_result read(dx_object** RETURN, dx_adl_type_handlers_material* SELF, dx_ddl_node* node, dx_adl_context* context) {
  dx_object* temporary = DX_OBJECT(_read_material(node, context));
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_adl_type_handlers_material_construct(dx_adl_type_handlers_material* SELF) {
  dx_rti_type* TYPE = dx_adl_type_handlers_material_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (dx_adl_type_handler_construct(DX_ADL_TYPE_HANDLER(SELF))) {
    return DX_FAILURE;
  }
  DX_ADL_TYPE_HANDLER(SELF)->resolve = (int (*)(dx_adl_type_handler*, dx_adl_symbol*, dx_adl_context*))&resolve;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

static void dx_adl_type_handlers_material_destruct(dx_adl_type_handlers_material* SELF)
{/*Intentionally empty.*/}

static void dx_adl_type_handlers_material_dispatch_construct(dx_adl_type_handlers_material_dispatch* SELF) {
  DX_ADL_TYPE_HANDLER_DISPATCH(SELF)->read = (dx_result (*)(dx_object**, dx_adl_type_handler*, dx_ddl_node*, dx_adl_context*)) & read;
}

dx_result dx_adl_type_handlers_material_create(dx_adl_type_handlers_material** RETURN) {
  DX_CREATE_PREFIX(dx_adl_type_handlers_material)
  if (dx_adl_type_handlers_material_construct(SELF)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}
