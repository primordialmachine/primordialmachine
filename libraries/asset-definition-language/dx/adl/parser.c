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
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
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

dx_result dx_asset_definition_language_parser_parse_vector_3_f32(dx_assets_vector_3_f32** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
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
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return DX_FAILURE;
  }
  if (child_node->kind != dx_ddl_node_kind_map) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  if (dx_asset_definition_language_parser_parse_vector_3_f32(RETURN, child_node, context)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_type(dx_string** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* key = NAME(type_key);
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return DX_FAILURE;
  }
  if (child_node->kind != dx_ddl_node_kind_string) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
    return DX_FAILURE;
  }
  dx_string* type = NULL;
  if (dx_ddl_node_get_string(&type, child_node)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  DX_DEBUG_CHECK_OBJECT_MAGIC_BYTES(type);
  *RETURN = type;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_name(dx_string** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* key = NAME(name_key);
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  dx_string* temporary = NULL;
  if (dx_ddl_node_get_string(&temporary, child_node)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_n8(dx_n8* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_n8(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_sz(dx_size* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return DX_FAILURE;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_sz(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_f32(dx_f32* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return DX_FAILURE;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_f32(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  return DX_SUCCESS;
}

dx_result dx_adl_semantical_read_f64(dx_f64* RETURN, dx_ddl_node* node, dx_string* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return DX_FAILURE;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  if (dx_convert_utf8bytes_to_f64(child_node->number->bytes, child_node->number->number_of_bytes, RETURN)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  return DX_SUCCESS;
}

dx_string* dx_adl_semantical_read_string_field(dx_ddl_node* node, dx_string* key, dx_adl_names* names) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return NULL;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return NULL;
  }
  dx_string* value = NULL;
  if (dx_ddl_node_get_string(&value, child_node)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return NULL;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  return value;
}

static dx_result dx_adl_semantical_read_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_string* expected_type, dx_adl_context* context) {
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    dx_set_error(DX_ERROR_SEMANTICAL_ERROR);
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

dx_result dx_asset_definition_language_parser_parse_color_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return DX_FAILURE;
  }
  dx_string* expected_type = NAME(color_instance_type);
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
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  dx_asset_reference* temporary = NULL;
  if (dx_asset_definition_language_parser_parse_color_instance(&temporary, child_node, context)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_image_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  return dx_adl_semantical_read_instance(RETURN, node, NAME(image_instance_type), context);
}

dx_result dx_asset_definition_language_parser_parse_image_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  dx_asset_reference* temporary = NULL;
  if (dx_asset_definition_language_parser_parse_image_instance(&temporary, child_node, context)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_texture_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  return dx_adl_semantical_read_instance(RETURN, node, NAME(texture_instance_type), context);
}

dx_result dx_asset_definition_language_parser_parse_texture_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  dx_asset_reference* temporary = NULL;
  if (dx_asset_definition_language_parser_parse_texture_instance(&temporary, child_node, context)) {
    DX_UNREFERENCE(child_node);
    child_node = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return DX_SUCCESS;
}

dx_result dx_asset_definition_language_parser_parse_material_instance(dx_asset_reference** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  return dx_adl_semantical_read_instance(RETURN, node, NAME(material_instance_type), context);
}

dx_result dx_adl_semantical_read_material_instance_field(dx_asset_reference** RETURN, dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (DX_ERROR_NOT_FOUND != dx_get_error()) {
      return DX_FAILURE;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_ERROR_SEMANTICAL_ERROR);
      return optional ? DX_SUCCESS : DX_FAILURE;
    }
  }
  return dx_asset_definition_language_parser_parse_material_instance(RETURN, child_node, context);
}
