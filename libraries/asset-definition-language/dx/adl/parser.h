/// @file dx/adl/semantical/parser.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
#if !defined(DX_ASSET_DEFINITION_LANGUAGE_PARSER_H_INCLUDED)
#define DX_ASSET_DEFINITION_LANGUAGE_PARSER_H_INCLUDED

#include "dx/data_definition_language.h"
#include "dx/assets.h"
typedef struct dx_asset_definition_language_diagnostics dx_asset_definition_language_diagnostics;
typedef struct dx_adl_context dx_adl_context;
typedef struct dx_adl_names dx_adl_names;

Core_declareObjectType("dx.asset_definition_language.parser",
                       dx_asset_definition_language_parser,
                       Core_Object);

static inline dx_asset_definition_language_parser* DX_ASSET_DEFINITION_LANGUAGE_PARSER(void* p) {
  return (dx_asset_definition_language_parser*)p;
}

struct dx_asset_definition_language_parser {
  Core_Object parent;
  dx_asset_definition_language_diagnostics* diagnostics;
};

static inline dx_asset_definition_language_parser_Dispatch* DX_ASSET_DEFINITION_LANGUAGE_PARSER_DISPATCH(void* p) {
  return (dx_asset_definition_language_parser_Dispatch*)p;
}

struct dx_asset_definition_language_parser_Dispatch {
  Core_Object_Dispatch parent;
};

Core_Result dx_asset_definition_language_parser_construct(dx_asset_definition_language_parser* SELF, dx_asset_definition_language_diagnostics* diagnostics);

Core_Result dx_asset_definition_language_parser_create(dx_asset_definition_language_parser** RETURN, dx_asset_definition_language_diagnostics* diagnostics);

/// @brief Read a type (name string)
/// @code
/// { type : <string> }
/// @endcode
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @param node A pointer to the node.
/// @param context A pointer to the context.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the type (name string).
/// The caller acquired a reference to the string object returned.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_type(Core_String** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse a name (string)
/// @code
/// { name : <string> }
/// @endcode
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @param node A pointer to the node.
/// @param context A pointer to the context.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the name (string).
/// The caller acquired a reference to the string object returned.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_name(Core_String** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// <ADL.Translation> : { type : 'Translation', x : <number>, y : <number>, z : <number> }
/// @endcode
/// @param RETURN A pointer to a <code>dx_assets_matrix_4x4_f32*</code> variable.
/// @param node A pointer to the node.
/// @param context A pointer to the context.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the translation object.
/// The caller acquired a reference to the string object returned.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_translation(dx_assets_matrix_4x4_f32** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// <ADL.ColorInstance> : { ... type : 'ColorInstance', reference : <name> ... }
/// @endcode
/// @param RETURN A pointer to a <code>dx_asset_reference*</code> variable.
/// @param node A pointer to the node.
/// @param context A pointer to the context.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the asset reference object.
/// The caller acquired a reference to the asset reference object returned.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_color_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// <ADL.Vector3> : { type : 'Vector3', x : <number>, y : <number>, z : <number> }
/// @endcode
/// @param node A pointer to the node.
/// @param context A pointer to the context.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the vector object.
/// The caller acquired a reference to the string object returned.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_vector_3_f32(dx_assets_vector_3_f32** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// <key> : <ADL.Vector3>
/// @endcode
/// @param RETURN A pointer to a <code>dx_assets_vector_3_f32*</code> variable.
/// @param node A pointer to the node.
/// @param context A pointer to the context.
/// @param key A pointer to the key (string).
/// @success
/// <code>*RETURN</code> was assigned a pointer to the vector object.
/// The caller acquired a reference to the string object returned.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_vector_3_f32_field(dx_assets_vector_3_f32** RETURN, dx_ddl_node* node, Core_String* key, dx_adl_context* context);

/// @brief Parse
/// @code
/// <key> ':' <number>
/// @endcode
/// @param [out] RETURN A pointer to a <code>Core_Natural8</code> variable.
/// @param node A pointer to a map node.
/// @param key A pointer to the key (string).
/// @method{dx_asset_definition_language_parser}
Core_Result dx_adl_semantical_read_n8(Core_Natural8* RETURN, dx_ddl_node* node, Core_String* key);

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param [out] RETURN A pointer to a <code>Core_Size</code> variable.
/// @param node A pointer to a map node.
/// @param key The key.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_adl_semantical_read_sz(Core_Size* RETURN, dx_ddl_node* node, Core_String* key);

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param [out] RETURN A pointer to a <code>Core_Real32</code> variable.
/// @param node A pointer to a map node.
/// @param key The key.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_adl_semantical_read_f32(Core_Real32* RETURN, dx_ddl_node* node, Core_String* key);

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param node A pointer to a map node.
/// @param key The key.
/// @param [out] target A pointer to a <code>Core_Real64</code> variable.
/// @default-failure
/// @default-return
Core_Result dx_adl_semantical_read_f64(Core_Real64* RETURN, dx_ddl_node* node, Core_String* key);

/// @brief Parse
/// @code
/// <key> ':' <string>
/// @endcode
/// @param node A pointer to a map node.
/// @param key The key.
/// @param [out] target A pointer to a <code>Core_Real64</code> variable.
/// @return A pointer to the value on success. The null pointer on failure.
/// @success The caller acquired a reference to the string object returned.
/// @default-failure
Core_String* dx_adl_semantical_read_string_field(dx_ddl_node* node, Core_String* key, dx_adl_names* names);

/// @brief Parse
/// @code
/// { ... <key> : <ADL.ColorInstance> ... }
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_adl_semantical_read_color_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context);

/// @brief Parse
/// @code
/// <ADL.ImageInstance> : { ... type : 'ImageInstance', reference : <name> ... }
/// @endcode
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_image_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// { ... <key> : <ADL.ImageInstance> ... }
/// @endcode
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_image_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context);

/// @brief Parse
/// @code
/// <ADL.TextureInstance> : { ... type : 'TextureInstance', reference : <name> ... }
/// @endcode
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_texture_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// { ... <key> : <ADL.TextureInstance> ... }
/// @endcode
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_texture_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context);

/// @brief Parse
/// @code
/// <ADL.MaterialInstance> : { ... type : 'MaterialInstance', reference : <name> ... }
/// @endcode
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_asset_definition_language_parser_parse_material_instance(Core_Assets_Ref** RETURN, dx_ddl_node* node, dx_adl_context* context);

/// @brief Parse
/// @code
/// { ... <key> : <ADL.MaterialInstance> ... }
/// @endcode
/// @param node A pointer to the node.
/// @method{dx_asset_definition_language_parser}
Core_Result dx_adl_semantical_read_material_instance_field(Core_Assets_Ref** RETURN, dx_ddl_node* node, bool optional, Core_String* key, dx_adl_context* context);

#endif // DX_ASSET_DEFINITION_LANGUAGE_PARSER_H_INCLUDED
