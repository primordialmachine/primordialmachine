/// @file dx/ddl/node.h
/// @brief Output of the syntactical analyses of the Data Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DATA_DEFINITION_LANGUAGE_NODE_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_NODE_H_INCLUDED

#include "dx/data_definition_language/node_kind.h"

/// @brief An node for programs of the Data Definition Language (DDL).
DX_DECLARE_OBJECT_TYPE("dx.ddl.node",
                       dx_ddl_node,
                       Core_Object);

static inline dx_ddl_node* DX_DDL_NODE(void* p) {
  return (dx_ddl_node*)p;
}

struct dx_ddl_node {
  Core_Object _parent;
  dx_ddl_node_kind kind;
  union {
    dx_inline_pointer_array list;
    dx_inline_pointer_hashmap map;
    Core_String* string;
    Core_String* number;
  };
};

static inline dx_ddl_node_dispatch* DX_DDL_NODE_DISPATCH(void* p) {
  return (dx_ddl_node_dispatch*)p;
}

struct dx_ddl_node_dispatch {
  Core_Object_Dispatch _parent;
};

/// @details The node is constructed with the specified node kind.
/// @param kind The kind of this node.
/// @success The node was assigned default values for the specified node kind.
/// Depending on the node kind, these default values are
/// - #dx_ddl_node_kind_empty: no value
/// - #dx_ddl_node_kind_string: the empty string
/// - #dx_ddl_node_kind_number: the zero integer number
/// - #dx_ddl_node_kind_map: the empty map
/// - #dx_ddl_node_kind_list: the empty list
/// @constructor{dx_ddl_node}
Core_Result dx_ddl_node_construct(dx_ddl_node* SELF, dx_ddl_node_kind kind);

/// @details The DDL node of the specified type is created with default values.
/// See dx_ddl_node_construct for more information.
/// @param kind The kind of this DDL node.
/// @create-operator{dx_ddl_node}
Core_Result dx_ddl_node_create(dx_ddl_node** RETURN, dx_ddl_node_kind kind);

/// @brief Get the node kind of this DDL node.
/// @param RETURN A pointer to a <code>dx_ddl_node_kind</code> variable.
/// @success <code>*RETURN</code> was assigned the kind of this DDL node.
/// @error #Core_Error_ArgumentInvalid @ a RETURN is a null pointer.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_get_kind(dx_ddl_node_kind* RETURN, dx_ddl_node const* SELF);

/// @brief Add or update the mapping from name to value in this DDL map node.
/// @param name The name.
/// @param value The value.
/// @error #Core_Error_ArgumentInvalid @a name is a null pointer
/// @error #Core_Error_ArgumentInvalid @a value is a null pointer
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_map.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_map_set(dx_ddl_node* SELF, Core_String* name, dx_ddl_node* value);

/// @brief Get a mapping from a name to a value (node) in this DDL map node.
/// @param RETURN A pointer to a <code>dx_ddl_node*</code> variable.
/// @param name The name.
/// @success <code>*RETURN</code> was assigned a pointer to the value (node).
/// The caller acquired a reference to that object.
/// @error #Core_Error_NotFound no entry for the specified key was found
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer
/// @error #Core_Error_ArgumentInvalid @a key is a null pointer
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_map_get(dx_ddl_node** RETURN, dx_ddl_node const* SELF, Core_String* key);

/// @brief Get the size of this DDL map node.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the size of this DDL map node.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_map.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_map_get_size(Core_Size* RETURN, dx_ddl_node* SELF);

/// @brief Append an DDL node to this DDL list node.
/// @param value A pointer to the DDL node to append.
/// @error #Core_Error_ArgumentInvalid @a value is a null pointer.
/// @error #Core_Error_OperationInvalid This node is not of node kind #dx_ddl_node_kind_list.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_list_append(dx_ddl_node* SELF, dx_ddl_node* value);

/// @brief Prepend an DDL node to this DDL list node.
/// @param value A pointer to the DDL node to prepend.
/// @error #Core_Error_ArgumentInvalid @a value is a null pointer.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_list.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_list_prepend(dx_ddl_node* SELF, dx_ddl_node* value);

/// @brief Insert an DDL node into this DDL list node.
/// @param value A pointer to the DDL node to insert.
/// @param index The index at which to insert the DDL node.
/// @error #Core_Error_ArgumentInvalid @a value is a null pointer.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_list.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_list_insert(dx_ddl_node* SELF, dx_ddl_node* value, Core_Size index);

/// @brief Get an DDL node at the specified index in this DDL list node.
/// @param index The index at which to get the DDL node.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a index is greater than or equal to the length of this DDL list node.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_list.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_list_get(dx_ddl_node** RETURN, dx_ddl_node* SELF, Core_Size index);

/// @brief Get the size of this DDL list node.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @success <code>*RETURN</code> was assigned the size of this DDL list node.
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_list.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_list_get_size(Core_Size* RETURN, dx_ddl_node* SELF);

/// @brief Get the string literal (string) of this DDL node.
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @success
/// <code>*RETURN</code> was assigned a pointer to the string literal (string).
/// The caller acquired a reference to that object.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_string.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_get_string(Core_String** RETURN, dx_ddl_node const* SELF);

/// @brief Set the string literal (string) of this DDL node.
/// @param string A pointer to the string literal (string).
/// @error #Core_Error_ArgumentInvalid @a string is a null pointer.
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_string.
/// @undefined The string is not well-formatted.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_set_string(dx_ddl_node* SELF, Core_String* string);

/// @brief Get the number literal (string) of this DDL node.
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @success <code>*RETURN</code> was assigned a pointer to the number literal (string).
/// The caller acquired a reference to that object.
/// @error #Core_Error_ArgumentInvalid @a RETURn is a null pointer
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_number
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_get_number(Core_String** RETURN, dx_ddl_node const* SELF);

/// @brief Set the number literal (string) of this DDL node.
/// @param SELF A pointer to this DDL node.
/// @param number A pointer to the number literal (string).
/// @error #Core_Error_ArgumentInvalid @a number is a null pointer
/// @error #Core_Error_OperationInvalid this node is not of node kind #dx_ddl_node_kind_number.
/// @undefined The string is not well-formatted.
/// @method{dx_ddl_node}
Core_Result dx_ddl_node_set_number(dx_ddl_node* SELF, Core_String* number);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_DATA_DEFINITION_LANGUAGE_NODE_H_INCLUDED
