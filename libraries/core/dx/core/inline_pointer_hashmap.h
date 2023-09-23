/// @file dx/core/inline_pointer_hashmap.h
/// @brief A hashmap for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2023 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_INLINE_POINTER_HASHMAP_H_INCLUDED)
#define DX_CORE_INLINE_POINTER_HASHMAP_H_INCLUDED

#include "dx/core/core.h"
typedef struct dx_inline_pointer_array dx_inline_pointer_array;

typedef void* dx_inline_pointer_hashmap_key;
typedef void* dx_inline_pointer_hashmap_value;

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a key was added.
/// @param key A pointer to the key.
typedef void (dx_inline_pointer_hashmap_key_added_callback)(dx_inline_pointer_hashmap_key* key);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a key was removed.
/// @param key A pointer to the key.
typedef void (dx_inline_pointer_hashmap_key_removed_callback)(dx_inline_pointer_hashmap_key*);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a value was added.
/// @param key A pointer to the value.
typedef void (dx_inline_pointer_hashmap_value_added_callback)(dx_inline_pointer_hashmap_value*);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a value was removed.
/// @param key A pointer to the value.
typedef void (dx_inline_pointer_hashmap_value_removed_callback)(dx_inline_pointer_hashmap_value*);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if the hash value of a key must be computed.
/// @param key A pointer to the key
typedef dx_result (dx_inline_pointer_hashmap_hash_key_callback)(dx_size*,dx_inline_pointer_hashmap_key*);
typedef dx_result (dx_inline_pointer_hashmap_compare_keys_callback)(dx_bool*,dx_inline_pointer_hashmap_key*, dx_inline_pointer_hashmap_key*);

typedef struct dx_inline_pointer_hashmap_node dx_inline_pointer_hashmap_node;

struct dx_inline_pointer_hashmap_node {
  dx_inline_pointer_hashmap_node* next;
  dx_size hash_value;
  dx_inline_pointer_hashmap_key key;
  dx_inline_pointer_hashmap_value value;
};

/// @ingroup Core_Aggregates
/// @brief A dynamic hashmap of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct dx_inline_pointer_hashmap dx_inline_pointer_hashmap;

struct dx_inline_pointer_hashmap {
  void* pimpl;
};

/// @ingroup Core_Aggregates
/// @brief Configuration of a pointer hashmap.
typedef struct DX_INLINE_POINTER_HASHMAP_CONFIGURATION {

  /// @brief A pointer to the @a _dx_key_added_callback function or a null pointer.
  dx_inline_pointer_hashmap_key_added_callback* key_added_callback;
  /// @brief A pointer to the @a _dx_key_removed_callback function or  a null pointer.
  dx_inline_pointer_hashmap_key_removed_callback* key_removed_callback;
  
  /// @brief A pointer to the @a _dx_value_added_callback function or a null pointer.
  dx_inline_pointer_hashmap_value_added_callback* value_added_callback;
  /// @brief A pointer to the @a _dx_value_removed_callback function or  a null pointer.
  dx_inline_pointer_hashmap_value_removed_callback* value_removed_callback;
  
  /// @brief A pointer to the @a dx_hash_key_callback function or a null pointer.
  dx_inline_pointer_hashmap_hash_key_callback* hash_key_callback;
  
  /// @brief A pointer to the @a dx_compare_keys_callback function or a null pointer.
  dx_inline_pointer_hashmap_compare_keys_callback* compare_keys_callback;
  
} DX_INLINE_POINTER_HASHMAP_CONFIGURATION;

/// @ingroup Core_Aggregates
/// @brief Initialize this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param configuration A pointer to the configuration.
/// @method-call
dx_result dx_inline_pointer_hashmap_initialize(dx_inline_pointer_hashmap* SELF, DX_INLINE_POINTER_HASHMAP_CONFIGURATION const* configuration);

/// @ingroup Core_Aggregates
/// @brief Uninitialize this hashmap.
/// @param self A pointer to this hashmap.
void dx_inline_pointer_hashmap_uninitialize(dx_inline_pointer_hashmap* SELF);

/// @ingroup Core_Aggregates
/// @brief Clear this hashmap.
/// @param SELF A pointer to this hashmap.
/// @method-call
dx_result dx_inline_pointer_hashmap_clear(dx_inline_pointer_hashmap* SELF);

/// @ingroup Core_Aggregates
/// @brief Add or update an entry in this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @param value The value.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF was a null pointer
/// @error #DX_ERROR_ALLOCATION_FAILED An allocation failed
/// @error #DX_ERROR_EXISTS An entry for the specified key already exists
dx_result dx_inline_pointer_hashmap_set(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_hashmap_key key, dx_inline_pointer_hashmap_value value);

/// @ingroup Core_Aggregates
/// @brief Get the value of an entry in this hashmap.
/// @param RETURN A pointer to a <code>dx_inline_pointer_hashmap_value</code> variable.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @success <code>*RETURN</code> was assigned the value of the entry.
/// @method-call
/// @error #DX_ERROR_NOT_FOUND no entry for the specified key was found
/// @error #DX_ERROR_INVALID_ARGUMENT @a self was a null pointer
dx_result dx_inline_pointer_hashmap_get(dx_inline_pointer_hashmap_value* RETURN, dx_inline_pointer_hashmap const* self, dx_inline_pointer_hashmap_key key);

/// @ingroup Core_Aggregates
/// @brief Remove an entry from this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @method-call
/// @error #DX_ERROR_NOT_FOUND no entry for the specified key was found
/// @error #DX_ERROR_INVALID_ARGUMENT @a self was a null pointer
dx_result dx_inline_pointer_hashmap_remove(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_hashmap_key key);

/// @ingroup Core_Aggregates
/// @brief Get the size, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
dx_result dx_inline_pointer_hashmap_get_size(dx_size* RETURN, dx_inline_pointer_hashmap const* SELF);

/// @ingroup Core_Aggregates
/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
dx_result dx_inline_pointer_hashmap_get_capacity(dx_size* RETURN, dx_inline_pointer_hashmap const* SELF);

/// @ingroup Core_Aggregates
/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
dx_result dx_inline_pointer_hashmap_get_free_capacity(dx_size* RETURN, dx_inline_pointer_hashmap const* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Aggregates
/// @brief An iterator for a pointer hash map.
/// @warning An iterator is invalidated if the hashmap is modified through any other function other than dx_inline_pointer_hashmap_iterator_remove for that particular iterator. 
/// @detail
/// Can be used as follows.
/// @code
/// dx_inline_pointer_hashmap_iterator iterator;
/// dx_inline_pointer_hashmap target;
/// ...
/// if (dx_inline_pointer_hashmap_iterator_initialize(&target, &target)) { ... }
/// dx_set_error(DX_NO_ERROR);
/// dx_bool has_entry;
/// if (dx_inline_pointer_hashmap_iterator_has_entry(&entry, &target)) {
///   ...
/// }
/// while (has_entry) {
///   void* k = NULL,
///       * v = NULL;
///   if (dx_inline_pointer_hashmap_iterator_get_key(&k, &target) || dx_inline_pointer_hashmap_iterator_get_value(&v, &target)) {
///     ...
///   }
///   if (dx_inline_pointer_hashmap_iterator_next(&target)) {
///     ...
///   }
///   if (dx_inline_pointer_hashmap_iterator_has_entry(&entry, &target)) {
///     ...
///   }
/// }
/// dx_inline_pointer_hashmap_iterator_uninitialize(&target);
/// @endcode
typedef struct dx_inline_pointer_hashmap_iterator dx_inline_pointer_hashmap_iterator;

struct dx_inline_pointer_hashmap_iterator {
  void* pimpl;
};

/// @ingroup Core_Aggregates
/// @brief Initialize this pointer hashmap iterator.
/// @param SELF A pointer to the dx_inline_pointer_hashmap_iterator object.
/// @param target A pointer to the pointer hashmap to iterate over.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a target is a null pointer.
dx_result dx_inline_pointer_hashmap_iterator_initialize(dx_inline_pointer_hashmap_iterator* SELF, dx_inline_pointer_hashmap* target);

/// @ingroup Core_Aggregates
/// @brief Uninitialize this pointer hashmap iterator.
/// @param self A pointer to the dx_inline_pointer_hashmap_iterator object.
void dx_inline_pointer_hashmap_iterator_uninitialize(dx_inline_pointer_hashmap_iterator* self);

/// @ingroup Core_Aggregates
/// @brief Advance this iterator.
/// @return #DX_SUCCESS on success. #DX_FAILURE on failure.
dx_result dx_inline_pointer_hashmap_iterator_next(dx_inline_pointer_hashmap_iterator* self);

/// @ingroup Core_Aggregates
/// @param SELF A pointer to this iterator.
/// @success <code>*RETURN</code> was assigned @a true if there is an entry and was assigned @a false if there no entry.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
dx_result dx_inline_pointer_hashmap_iterator_has_entry(dx_bool* RETURN, dx_inline_pointer_hashmap_iterator* SELF);

/// @ingroup Core_Aggregates
/// @param RETURN A pointer to a dx_inline_pointer_hashmap_value variable.
/// @param SELF A pointer to this iterator.
/// @success <code>*RETURN</code> was assigned the value.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
dx_result dx_inline_pointer_hashmap_iterator_get_value(dx_inline_pointer_hashmap_value* RETURN, dx_inline_pointer_hashmap_iterator* SELF);

/// @ingroup Core_Aggregates
/// @param RETURN A pointer to a dx_inline_pointer_hashmap_key variable.
/// @param SELF A pointer to this iterator.
/// @success <code>*RETURN</code> was assigned the key.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN is a null pointer.
dx_result dx_inline_pointer_hashmap_iterator_get_key(dx_inline_pointer_hashmap_key* RETURN, dx_inline_pointer_hashmap_iterator* SELF);

/// @ingroup Core_Aggregates
/// @brief Remove the current entry.
/// @param SELF A pointer to this iterator.
/// @method-call
dx_result dx_inline_pointer_hashmap_iterator_remove(dx_inline_pointer_hashmap_iterator* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Aggregates
/// @brief Append the keys of this pointer hashmap to the specified pointer array.
/// @param SELF A pointer to this pointer hashmap.
/// @param target A pointer to the target array.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a TARGET is a null pointer.
dx_result dx_inline_pointer_hashmap_get_keys(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_array* target);

/// @ingroup Core_Aggregates
/// @brief Append the values of this pointer hashmap to the specified pointer array.
/// @param SELF A pointer to this pointer hashmap.
/// @param target A pointer to the target array.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a TARGET is a null pointer.
dx_result dx_inline_pointer_hashmap_get_values(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_array* target);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_INLINE_POINTER_HASHMAP_H_INCLUDED
