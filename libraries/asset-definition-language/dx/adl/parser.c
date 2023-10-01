#include "dx/adl/parser.h"

#include "dx/adl/context.h"
#include "dx/adl/names.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}
#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

DX_DEFINE_OBJECT_TYPE("dx.asset_definition_language.parser",
                      dx_asset_definition_language_parser,
                      dx_object);

static void dx_asset_definition_language_parser_destruct(dx_asset_definition_language_parser* SELF) {
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
}

static void dx_asset_definition_language_parser_dispatch_construct(dx_asset_definition_language_parser_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_asset_definition_language_parser_construct(dx_asset_definition_language_parser* SELF, dx_asset_definition_language_diagnostics* diagnostics) {
  dx_rti_type* TYPE = dx_asset_definition_language_parser_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!diagnostics) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  SELF->diagnostics = diagnostics;
  DX_REFERENCE(SELF->diagnostics);
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_create(dx_asset_definition_language_parser** RETURN, dx_asset_definition_language_diagnostics* diagnostics) {
  DX_CREATE_PREFIX(dx_asset_definition_language_parser)
  if (dx_asset_definition_language_parser_construct(SELF, diagnostics)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static dx_result _read_translation(DX_MAT4* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names);

static dx_result _read_vector_3(DX_VEC3* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names);

static dx_result _read_translation(DX_MAT4* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names) {
  if (!target || !node || !context) {
    return DX_FAILURE;
  }
  dx_f32 x, y, z;
  if (dx_adl_semantical_read_f32(&x, node, NAME(x_key))) {
    return DX_FAILURE;
  }
  if (dx_adl_semantical_read_f32(&y, node, NAME(y_key))) {
    return DX_FAILURE;
  }
  if (dx_adl_semantical_read_f32(&z, node, NAME(z_key))) {
    return DX_FAILURE;
  }
  dx_mat4_set_translate(target, x, y, z);
  return DX_SUCCESS;
}

static dx_result _read_vector_3(DX_VEC3* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names) {
  if (!target || !node || !context) {
    return DX_FAILURE;
  }
  dx_f32 x, y, z;
  if (dx_adl_semantical_read_f32(&x, node, NAME(x_key))) {
    return DX_FAILURE;
  }
  if (dx_adl_semantical_read_f32(&y, node, NAME(y_key))) {
    return DX_FAILURE;
  }
  if (dx_adl_semantical_read_f32(&z, node, NAME(z_key))) {
    return DX_FAILURE;
  }
  dx_vec3_set(target, x, y, z);
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_translation(dx_assets_matrix_4x4_f32** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(translation_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    }
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  DX_MAT4 value;
  dx_memory_zero(&value, sizeof(DX_MAT4));
  if (_read_translation(&value, node, context, context->names)) {
    return DX_FAILURE;
  }
  dx_assets_matrix_4x4_f32* temporary = NULL;
  if (dx_assets_matrix_4x4_f32_create(&temporary, &value)) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

DX_MAT4* dx_adl_semantical_read_translation(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
  if (!dx_string_is_equal_to(received_type, NAME(translation_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (!dx_get_error()) {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    }
    return NULL;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  DX_MAT4* target = NULL;
  if (dx_memory_allocate(&target, sizeof(DX_MAT4))) {
    return NULL;
  }
  dx_memory_zero(target, sizeof(DX_MAT4));
  if (_read_translation(target, node, context, context->names)) {
    dx_memory_deallocate(target);
    target = NULL;
    return NULL;
  }
  return target;
}

dx_result dx_asset_definition_language_parser_parse_vector_3_f32(dx_assets_vector_3_f32** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, NAME(vector3_type))) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    if (DX_NO_ERROR == dx_get_error()) {
      dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    }
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  DX_VEC3 value;
  dx_memory_zero(&value, sizeof(DX_VEC3));
  if (_read_vector_3(&value, node, context, context->names)) {
    return DX_FAILURE;
  }
  dx_assets_vector_3_f32* temporary = NULL;
  if (dx_assets_vector_3_f32_create(&temporary, &value)) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_vector_3_f32_field(dx_assets_vector_3_f32** RETURN, dx_ddl_node* node, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_map) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  if (dx_asset_definition_language_parser_parse_vector_3_f32(RETURN, child_node, context)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_string* dx_adl_semantical_read_type(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* key = NAME(type_key);
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    return NULL;
  }
  dx_string* type = NULL;
  if (dx_ddl_node_get_string_2(&type, child_node)) {
    return NULL;
  }
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(type);
  return type;
}

dx_result dx_adl_semantical_read_name(dx_string** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* key = NAME(name_key);
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    return DX_FAILURE;
  }
  return dx_ddl_node_get_string_2(RETURN, child_node);
}

dx_result dx_adl_semantical_read_n8(dx_n8* RETURN, dx_ddl_node* node, dx_string* name) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, name);
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_n8(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_sz(dx_size* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_sz(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_f32(dx_f32* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_f32(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_f64(dx_f64* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_f64(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

dx_string* dx_adl_semantical_read_string_field(dx_ddl_node* node, dx_string* key, dx_adl_names* names) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    return NULL;
  }
  dx_string* value = NULL;
  if (dx_ddl_node_get_string_2(&value, child_node)) {
    return NULL;
  }
  return value;
}

dx_result dx_adl_semantical_read_color_instance_0(dx_assets_color_rgb_n8** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_reference* asset_reference = NULL;
  if (dx_adl_semantical_read_color_instance(&asset_reference, node, context)) {
    return DX_FAILURE;
  }
  // TODO: Check type of definitions. Handle cases of definitions not found and definition of the wrong type.
  dx_adl_symbol* sym = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, asset_reference->name));
  dx_assets_color_rgb_n8* asset_color = DX_ASSETS_COLOR_RGB_N8(sym->asset);
  DX_UNREFERENCE(asset_reference);
  asset_reference = NULL;
  if (!asset_color) {
    return DX_FAILURE;
  }
  DX_REFERENCE(asset_color);
  *RETURN = asset_color;
  return DX_SUCCESS;
}

dx_assets_color_rgb_n8* dx_adl_semantical_read_color_instance_field_0(dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return NULL;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
    }
    return NULL;
  }
  dx_assets_color_rgb_n8* temporary = NULL;
  if (dx_adl_semantical_read_color_instance_0(&temporary, child_node, context)) {
    return NULL;
  }
  return temporary;
}

dx_asset_reference* dx_adl_semantical_read_instance(dx_ddl_node* node, dx_string* expected_type, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return NULL;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_string* value = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
  if (!value) {
    return NULL;
  }
  dx_asset_reference* reference = NULL;
  if (dx_asset_reference_create(&reference, value)) {
    DX_UNREFERENCE(value);
    value = NULL;
    return NULL;
  }
  DX_UNREFERENCE(value);
  value = NULL;
  return reference;
}

dx_result dx_adl_semantical_read_color_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* expected_type = NAME(color_instance_type);
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return DX_FAILURE;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  DX_UNREFERENCE(received_type);
  received_type = NULL;
  dx_string* value = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
  if (!value) {
    return DX_FAILURE;
  }
  dx_asset_reference* reference = NULL;
  if (dx_asset_reference_create(&reference, value)) {
    DX_UNREFERENCE(value);
    value = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(value);
  value = NULL;
  *RETURN = reference;
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_color_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  dx_asset_reference* temporary = NULL;
  if (dx_adl_semantical_read_color_instance(&temporary, child_node, context)) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_image_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_reference* temporary = dx_adl_semantical_read_instance(node, NAME(image_instance_type), context);
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_image_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  return dx_asset_definition_language_parser_parse_image_instance(RETURN, child_node, context);
}

dx_result dx_asset_definition_language_parser_parse_texture_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_reference* temporary = dx_adl_semantical_read_instance(node, NAME(texture_instance_type), context);
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_texture_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  return dx_asset_definition_language_parser_parse_texture_instance(RETURN, child_node, context);
}

dx_result dx_asset_definition_language_parser_parse_material_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_reference* temporary = dx_adl_semantical_read_instance(node, NAME(material_instance_type), context);
  if (!temporary) {
    return DX_FAILURE;
  }
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_material_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  return dx_asset_definition_language_parser_parse_material_instance(RETURN, child_node, context);
}
