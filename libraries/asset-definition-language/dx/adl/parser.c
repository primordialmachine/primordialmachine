#include "dx/adl/parser.h"

#include "dx/adl/context.h"
#include "dx/adl/names.h"

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_adl_name_index_##name)

Core_defineObjectType("dx.asset_definition_language.parser",
                      dx_asset_definition_language_parser,
                      Core_Object);

static void dx_asset_definition_language_parser_destruct(dx_asset_definition_language_parser* SELF) {
  CORE_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
}

static void dx_asset_definition_language_parser_constructDispatch(dx_asset_definition_language_parser_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_asset_definition_language_parser_construct(dx_asset_definition_language_parser* SELF, dx_asset_definition_language_diagnostics* diagnostics) {
  DX_CONSTRUCT_PREFIX(dx_asset_definition_language_parser);
  if (!diagnostics) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->diagnostics = diagnostics;
  CORE_REFERENCE(SELF->diagnostics);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_create(dx_asset_definition_language_parser** RETURN, dx_asset_definition_language_diagnostics* diagnostics) {
  DX_CREATE_PREFIX(dx_asset_definition_language_parser);
  if (dx_asset_definition_language_parser_construct(SELF, diagnostics)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result _read_translation(DX_MAT4* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names);

static Core_Result _read_vector_3(DX_VEC3* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names);

static Core_Result _read_translation(DX_MAT4* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names) {
  if (!target || !node || !context) {
    return Core_Failure;
  }
  Core_Real32 x, y, z;
  if (dx_adl_semantical_read_f32(&x, node, NAME(x_key))) {
    return Core_Failure;
  }
  if (dx_adl_semantical_read_f32(&y, node, NAME(y_key))) {
    return Core_Failure;
  }
  if (dx_adl_semantical_read_f32(&z, node, NAME(z_key))) {
    return Core_Failure;
  }
  dx_mat4_set_translate(target, x, y, z);
  return Core_Success;
}

static Core_Result _read_vector_3(DX_VEC3* target, dx_ddl_node* node, dx_adl_context* context, dx_adl_names* names) {
  if (!target || !node || !context) {
    return Core_Failure;
  }
  Core_Real32 x, y, z;
  if (dx_adl_semantical_read_f32(&x, node, NAME(x_key))) {
    return Core_Failure;
  }
  if (dx_adl_semantical_read_f32(&y, node, NAME(y_key))) {
    return Core_Failure;
  }
  if (dx_adl_semantical_read_f32(&z, node, NAME(z_key))) {
    return Core_Failure;
  }
  dx_vec3_set(target, x, y, z);
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_translation(Core_Assets_Matrix4x4R32** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(translation_type))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  if (!isEqualTo) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    Core_setError(Core_Error_LexicalAnalysisFailed);
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  DX_MAT4 value;
  Core_Memory_zero(&value, sizeof(DX_MAT4));
  if (_read_translation(&value, node, context, context->names)) {
    return Core_Failure;
  }
  Core_Assets_Matrix4x4R32* temporary = NULL;
  if (Core_Assets_Matrix4x4R32_create(&temporary, &value)) {
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_vector_3_f32(Core_Assets_Vector3R32** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, NAME(vector3_type))) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  if (!isEqualTo) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  DX_VEC3 value;
  Core_Memory_zero(&value, sizeof(DX_VEC3));
  if (_read_vector_3(&value, node, context, context->names)) {
    return Core_Failure;
  }
  Core_Assets_Vector3R32* temporary = NULL;
  if (Core_Assets_Vector3R32_create(&temporary, &value)) {
    return Core_Failure;
  }
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_vector_3_f32_field(Core_Assets_Vector3R32** RETURN, dx_ddl_node* node, Core_String* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return Core_Failure;
  }
  if (child_node->kind != dx_ddl_node_kind_map) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  if (dx_asset_definition_language_parser_parse_vector_3_f32(RETURN, child_node, context)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_type(Core_String** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* key = NAME(type_key);
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return Core_Failure;
  }
  if (child_node->kind != dx_ddl_node_kind_string) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_String* type = NULL;
  if (dx_ddl_node_get_string(&type, child_node)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  Core_Debug_checkObjectMagicBytes(type);
  *RETURN = type;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_name(Core_String** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* key = NAME(name_key);
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  Core_String* temporary = NULL;
  if (dx_ddl_node_get_string(&temporary, child_node)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_adl_semantical_read_n8(Core_Natural8* RETURN, dx_ddl_node* node, Core_String* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  if (Core_convertStringToN8(RETURN, child_node->number->bytes, child_node->number->numberOfBytes)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return Core_Success;
}

Core_Result dx_adl_semantical_read_sz(Core_Size* RETURN, dx_ddl_node* node, Core_String* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return Core_Failure;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  if (Core_convertStringToSz(RETURN, child_node->number->bytes, child_node->number->numberOfBytes)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return Core_Success;
}

Core_Result dx_adl_semantical_read_f32(Core_Real32* RETURN, dx_ddl_node* node, Core_String* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return Core_Failure;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  if (Core_convertStringToR32(RETURN, child_node->number->bytes, child_node->number->numberOfBytes)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return Core_Success;
}

Core_Result dx_adl_semantical_read_f64(Core_Real64* RETURN, dx_ddl_node* node, Core_String* key) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return Core_Failure;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_number) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  if (Core_convertStringToR64(RETURN, child_node->number->bytes, child_node->number->numberOfBytes)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return Core_Success;
}

Core_String* dx_adl_semantical_read_string_field(dx_ddl_node* node, Core_String* key, dx_adl_names* names) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    return NULL;
  }
  if (!child_node || child_node->kind != dx_ddl_node_kind_string) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return NULL;
  }
  Core_String* value = NULL;
  if (dx_ddl_node_get_string(&value, child_node)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return NULL;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  return value;
}

static Core_Result dx_adl_semantical_read_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, Core_String* expected_type, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, expected_type)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  if (!isEqualTo) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_String* value = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
  if (!value) {
    return Core_Failure;
  }
  Core_Assets_Ref* reference = NULL;
  if (Core_Assets_Ref_create(&reference, value)) {
    CORE_UNREFERENCE(value);
    value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(value);
  value = NULL;
  *RETURN = reference;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_color_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  Core_String* received_type = NULL;
  if (dx_asset_definition_language_parser_parse_type(&received_type, node, context)) {
    return Core_Failure;
  }
  Core_String* expected_type = NAME(color_instance_type);
  Core_Boolean isEqualTo = Core_False;
  if (Core_String_isEqualTo(&isEqualTo, received_type, expected_type)) {
    CORE_UNREFERENCE(received_type);
    received_type = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(received_type);
  received_type = NULL;
  if (!isEqualTo) {
    Core_setError(Core_Error_SemanticalAnalysisFailed);
    return Core_Failure;
  }
  Core_String* value = dx_adl_semantical_read_string_field(node, NAME(reference_key), context->names);
  if (!value) {
    return Core_Failure;
  }
  Core_Assets_Ref* reference = NULL;
  if (Core_Assets_Ref_create(&reference, value)) {
    CORE_UNREFERENCE(value);
    value = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(value);
  value = NULL;
  *RETURN = reference;
  return Core_Success;
}

Core_Result dx_adl_semantical_read_color_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(optional ? Core_Error_NoError : Core_Error_SemanticalAnalysisFailed);
      return optional ? Core_Success : Core_Failure;
    }
  }
  Core_Assets_Ref* temporary = NULL;
  if (dx_asset_definition_language_parser_parse_color_instance(&temporary, child_node, context)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_image_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  return dx_adl_semantical_read_instance(RETURN, node, NAME(image_instance_type), context);
}

Core_Result dx_asset_definition_language_parser_parse_image_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(optional ? Core_Error_NoError : Core_Error_SemanticalAnalysisFailed);
      return optional ? Core_Success : Core_Failure;
    }
  }
  Core_Assets_Ref* temporary = NULL;
  if (dx_asset_definition_language_parser_parse_image_instance(&temporary, child_node, context)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_texture_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  return dx_adl_semantical_read_instance(RETURN, node, NAME(texture_instance_type), context);
}

Core_Result dx_asset_definition_language_parser_parse_texture_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(optional ? Core_Error_NoError : Core_Error_SemanticalAnalysisFailed);
      return optional ? Core_Success : Core_Failure;
    }
  }
  Core_Assets_Ref* temporary = NULL;
  if (dx_asset_definition_language_parser_parse_texture_instance(&temporary, child_node, context)) {
    CORE_UNREFERENCE(child_node);
    child_node = NULL;
    return Core_Failure;
  }
  CORE_UNREFERENCE(child_node);
  child_node = NULL;
  *RETURN = temporary;
  return Core_Success;
}

Core_Result dx_asset_definition_language_parser_parse_material_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context) {
  return dx_adl_semantical_read_instance(RETURN, node, NAME(material_instance_type), context);
}

Core_Result dx_adl_semantical_read_material_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context) {
  dx_ddl_node* child_node = NULL;
  if (dx_ddl_node_map_get(&child_node, node, key)) {
    if (Core_Error_NotFound != Core_getError()) {
      return Core_Failure;
    } else {
      Core_setError(optional ? Core_Error_NoError : Core_Error_SemanticalAnalysisFailed);
      return optional ? Core_Success : Core_Failure;
    }
  }
  return dx_asset_definition_language_parser_parse_material_instance(RETURN, child_node, context);
}
