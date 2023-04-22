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
                       dx_object);

static inline dx_ddl_node* DX_DDL_NODE(void* p) {
  return (dx_ddl_node*)p;
}

struct dx_ddl_node {
  dx_object _parent;
  dx_ddl_node_kind kind;
  union {
    dx_inline_pointer_array list;
    dx_inline_pointer_hashmap map;
    dx_string* string;
    dx_string* number;
  };
};

static inline dx_ddl_node_dispatch* DX_DDL_NODE_DISPATCH(void* p) {
  return (dx_ddl_node_dispatch*)p;
}

struct dx_ddl_node_dispatch {
  dx_object _parent;
};

/// @brief Construct this DDL node with the specified type.
/// @param SELF A pointer to this DDL node.
/// @param kind The kind of this DDL node.
/// @success The node was assigned default values for the specified type.
/// Default values are:
/// - #dx_ddl_node_kind_empty: -
/// - #dx_ddl_node_kind_string: the empty string
/// - #dx_ddl_node_kind_number: the zero integer number
/// - #dx_ddl_node_kind_map: the empty map
/// - #dx_ddl_node_kind_list: the empty list
/// @default-runtime-calling-convention
dx_result dx_ddl_node_construct(dx_ddl_node* SELF, dx_ddl_node_kind kind);

/// @brief Create this DDL node with the specified type.
/// @param RETURN A pointer to a <code>dx_ddl_node*</code> variable.
/// @param kind The kind of this DDL node.
/// @return A pointer to this DDL node. The null pointer on failure.
/// @success
/// The node was assigned default values for the specified type.
/// See dx_ddl_node_construct for details.
/// @error RETURN is a null pointer
/// @default-runtime-calling-convention
dx_result dx_ddl_node_create(dx_ddl_node** RETURN, dx_ddl_node_kind kind);

/// @brief Get the node kind of this DDL node.
/// @param RETURN A pointer to a <code>dx_ddl_node_kind</code> variable.
/// @param SELF A pointer to this DDL node.
/// @success <code>*RETURN</code> was assigned the kind of this DDL node.
/// @error RETURN is a null pointer
/// @default-runtime-calling-convention
dx_result dx_ddl_node_get_kind(dx_ddl_node_kind* RETURN, dx_ddl_node const* SELF);

/// @brief Add or update the mapping from name to value in this DDL map node.
/// @param SELF A pointer to this DDL node.
/// @param name The name.
/// @param value The value.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a name is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a value is a null pointer
/// @error #DX_ERROR_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_map.
/// @default-runtime-calling-convention
dx_result dx_ddl_node_map_set(dx_ddl_node* SELF, dx_string* name, dx_ddl_node* value);

/// @brief Get a mapping from a name to a value in this DDL map node.
/// @param SELF A pointer to this DDL node.
/// @param name The name.
/// @return A pointer to the the value on success.
/// The null pointer on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
/// - #DX_INVALID_ARGUMENT @a name was a null pointer
dx_ddl_node* dx_ddl_node_map_get(dx_ddl_node const* SELF, dx_string* name);

/// @brief Get a mapping from a name to a value in this DDL map node.
/// @param SELF A pointer to this DDL node.
/// @param name The name.
/// @return A pointer to the the value if a key/value entry was found or the null pointer on success.
/// The null pointer is also returned on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
/// - #DX_INVALID_ARGUMENT @a name was a null pointer
static inline dx_ddl_node* dx_ddl_node_map_get_or_null(dx_ddl_node const* SELF, dx_string* name) {
  int old_error = dx_get_error();
  dx_set_error(DX_NO_ERROR);
  dx_ddl_node* node = dx_ddl_node_map_get(SELF, name);
  if (dx_get_error()) {
    if (DX_ERROR_NOT_FOUND == dx_get_error()) {
      dx_set_error(old_error);
    }
    return NULL;
  }
  return node;
}

/// @brief Append an DDL node to this DDL list node.
/// @param SELF A pointer to this DDL node.
/// @param value A pointer to the DDL node to append.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a SELF is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
int dx_ddl_node_list_append(dx_ddl_node* SELF, dx_ddl_node* value);

/// @brief Prepend an DDL node to this DDL list node.
/// @param SELF A pointer to this DDL node.
/// @param value A pointer to the DDL node to prepend.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a SELF is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
int dx_ddl_node_list_prepend(dx_ddl_node* SELF, dx_ddl_node* value);

/// @brief Insert an DDL node into this DDL list node.
/// @param self A pointer to this DDL node.
/// @param value A pointer to the DDL node to insert.
/// @param index The index at which to insert the DDL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
int dx_ddl_node_list_insert(dx_ddl_node* self, dx_ddl_node* value, dx_size index);

/// @brief Get an DDL node at the specified index in this DDL list node.
/// @param self A pointer to this DDL node.
/// @param index The index at which to get the DDL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than or equal to the length of this DDL list node
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
dx_ddl_node* dx_ddl_node_list_get(dx_ddl_node* self, dx_size index);

/// @brief Get an DDL node at the specified index in this DDL list node.
/// @param self A pointer to this DDL node.
/// @param index The index at which to get the DDL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than or equal to the length of this DDL list node
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
dx_size dx_ddl_node_list_get_size(dx_ddl_node* self);

/// @brief Get the string value of this DDL node.
/// @param self A pointer to this DDL node.
/// @return The string value on success. The null pointer on failure.
/// @success The caller acquired a reference to the returned dx_string object.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_string.
dx_string* dx_ddl_node_get_string(dx_ddl_node const* SELF);

/// @brief Set the string value of this DDL node.
/// @param self A pointer to this DDL node.
/// @param string A pointer to the string value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer
/// @error #DX_ERROR_INVALID_ARGUMENT @a string is a null pointer
/// @error #DX_ERROR_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_string.
/// @undefined The string is not well-formatted.
/// @default-runtime-calling-convention
int dx_ddl_node_set_string(dx_ddl_node* SELF, dx_string* string);

/// @brief Get the number value of this DDL node.
/// @param self A pointer to this DDL node.
/// @return The number value on success. The null pointer on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_number.
dx_string* dx_ddl_node_get_number(dx_ddl_node const* self);

/// @brief Set the number value of this DDL node.
/// @param self A pointer to this DDL node.
/// @param number A pointer to the string value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a number is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_number.
/// @undefined The string is not well-formatted.
int dx_ddl_node_set_number(dx_ddl_node* self, dx_string* number);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_DATA_DEFINITION_LANGUAGE_NODE_H_INCLUDED
