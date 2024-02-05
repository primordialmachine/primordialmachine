/// @file dx/core/inline_pointer_hashmap.h
/// @brief A hashmap for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(DX_CORE_INLINE_POINTER_HASHMAP_H_INCLUDED)
#define DX_CORE_INLINE_POINTER_HASHMAP_H_INCLUDED

#include "dx/core/core.h"
typedef struct Core_InlineArrayListP Core_InlineArrayListP;

typedef void* Core_InlineHashMapPP_Key;
typedef void* Core_InlineHashMapPP_Value;

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a key was added.
/// @param key A pointer to the key.
typedef void (Core_InlineHashMapPP_KeyAddedCallback)(Core_InlineHashMapPP_Key* key);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a key was removed.
/// @param key A pointer to the key.
typedef void (Core_InlineHashMapPP_KeyRemovedCallback)(Core_InlineHashMapPP_Key*);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a value was added.
/// @param key A pointer to the value.
typedef void (Core_InlineHashMapPP_ValueAddedCallback)(Core_InlineHashMapPP_Value*);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if a value was removed.
/// @param key A pointer to the value.
typedef void (Core_InlineHashMapPP_ValueRemovedCallback)(Core_InlineHashMapPP_Value*);

/// @ingroup Core_Aggregates
/// @brief Type of a callback function invoked if the hash value of a key must be computed.
/// @param key A pointer to the key
typedef Core_Result (Core_InlineHashMapPP_HashKeyCallback)(Core_Size*, Core_InlineHashMapPP_Key*);
typedef Core_Result (Core_InlineHashMapPP_CompareKeysCallback)(Core_Boolean*, Core_InlineHashMapPP_Key*, Core_InlineHashMapPP_Key*);

/// @ingroup Core_Aggregates
/// @brief A dynamic hashmap of pointers (also null pointers).
/// Supports callbacks for notifications on additions and removals of pointers.
typedef struct Core_InlineHashMapPP {
  void* pimpl;
} Core_InlineHashMapPP;

/// @ingroup Core_Aggregates
/// @brief Configuration of a pointer hashmap.
typedef struct Core_InlineHashMapPP_Configuration {

  /// @brief A pointer to the @a _dx_key_added_callback function or a null pointer.
  Core_InlineHashMapPP_KeyAddedCallback* keyAddedCallback;
  /// @brief A pointer to the @a _dx_key_removed_callback function or  a null pointer.
  Core_InlineHashMapPP_KeyRemovedCallback* keyRemovedCallback;
  
  /// @brief A pointer to the @a _dx_value_added_callback function or a null pointer.
  Core_InlineHashMapPP_ValueAddedCallback* valueAddedCallback;
  /// @brief A pointer to the @a _dx_value_removed_callback function or  a null pointer.
  Core_InlineHashMapPP_ValueRemovedCallback* valueRemovedCallback;
  
  /// @brief A pointer to the @a dx_hash_key_callback function or a null pointer.
  Core_InlineHashMapPP_HashKeyCallback* hashKeyCallback;
  
  /// @brief A pointer to the @a dx_compare_keys_callback function or a null pointer.
  Core_InlineHashMapPP_CompareKeysCallback* compareKeysCallback;
  
} Core_InlineHashMapPP_Configuration;

/// @ingroup Core_Aggregates
/// @brief Initialize this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param configuration A pointer to the configuration.
/// @method-call
Core_Result Core_InlineHashMapPP_initialize(Core_InlineHashMapPP* SELF, Core_InlineHashMapPP_Configuration const* configuration);

/// @ingroup Core_Aggregates
/// @brief Uninitialize this hashmap.
/// @param self A pointer to this hashmap.
void Core_InlineHashMapPP_uninitialize(Core_InlineHashMapPP* SELF);

/// @ingroup Core_Aggregates
/// @brief Clear this hashmap.
/// @param SELF A pointer to this hashmap.
/// @method-call
Core_Result Core_InlineHashMapPP_clear(Core_InlineHashMapPP* SELF);

/// @ingroup Core_Aggregates
/// @brief Add or update an entry in this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @param value The value.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF was a null pointer
/// @error #Core_Error_AllocationFailed An allocation failed
/// @error #Core_Error_Exists An entry for the specified key already exists
Core_Result Core_InlineHashMapPP_set(Core_InlineHashMapPP* SELF, Core_InlineHashMapPP_Key key, Core_InlineHashMapPP_Value value);

/// @ingroup Core_Aggregates
/// @brief Get the value of an entry in this hashmap.
/// @param RETURN A pointer to a <code>Core_InlineHashMapPP_value</code> variable.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @success <code>*RETURN</code> was assigned the value of the entry.
/// @method-call
/// @error #Core_Error_NotFound no entry for the specified key was found
/// @error #Core_Error_ArgumentInvalid @a self was a null pointer
Core_Result Core_InlineHashMapPP_get(Core_InlineHashMapPP_Value* RETURN, Core_InlineHashMapPP const* SELF, Core_InlineHashMapPP_Key key);

/// @ingroup Core_Aggregates
/// @brief Remove an entry from this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @method-call
/// @error #Core_Error_NotFound no entry for the specified key was found
/// @error #Core_Error_ArgumentInvalid @a self was a null pointer
Core_Result Core_InlineHashMapPP_remove(Core_InlineHashMapPP* SELF, Core_InlineHashMapPP_Key key);

/// @ingroup Core_Aggregates
/// @brief Get the size, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
Core_Result Core_InlineHashMapPP_getSize(Core_Size* RETURN, Core_InlineHashMapPP const* SELF);

/// @ingroup Core_Aggregates
/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
Core_Result Core_InlineHashMapPP_getCapacity(Core_Size* RETURN, Core_InlineHashMapPP const* SELF);

/// @ingroup Core_Aggregates
/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
Core_Result Core_InlineHashMapPP_getFreeCapacity(Core_Size* RETURN, Core_InlineHashMapPP const* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Aggregates
/// @brief An iterator for a pointer hash map.
/// @warning An iterator is invalidated if the hashmap is modified through any other function other than Core_InlineHashMapPPIterator_remove for that particular iterator. 
/// @detail
/// Can be used as follows.
/// @code
/// Core_InlineHashMapPPIterator iterator;
/// Core_InlineHashMapPP target;
/// ...
/// if (Core_InlineHashMapPPIterator_initialize(&target, &target)) { ... }
/// Core_setError(Core_Error_NoError);
/// Core_Boolean has_entry;
/// if (Core_InlineHashMapPPIterator_hasEntry(&entry, &target)) {
///   ...
/// }
/// while (hasEntry) {
///   void* k = NULL,
///       * v = NULL;
///   if (Core_InlineHashMapPPIterator_getKey(&k, &target) || Core_InlineHashMapPPIterator_getValue(&v, &target)) {
///     ...
///   }
///   if (Core_InlineHashMapPPIterator_next(&target)) {
///     ...
///   }
///   if (Core_InlineHashMapPPIterator_hasEntry(&entry, &target)) {
///     ...
///   }
/// }
/// Core_InlineHashMapPPIterator_uninitialize(&target);
/// @endcode
typedef struct Core_InlineHashMapPPIterator {
  void* pimpl;
} Core_InlineHashMapPPIterator;

/// @ingroup Core_Aggregates
/// @brief Initialize this pointer hashmap iterator.
/// @param SELF A pointer to the Core_InlineHashMapPPIterator object.
/// @param target A pointer to the pointer hashmap to iterate over.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a target is a null pointer.
Core_Result Core_InlineHashMapPPIterator_initialize(Core_InlineHashMapPPIterator* SELF, Core_InlineHashMapPP* target);

/// @ingroup Core_Aggregates
/// @brief Uninitialize this pointer hashmap iterator.
/// @param self A pointer to the Core_InlineHashMapPPIterator object.
void Core_InlineHashMapPPIterator_uninitialize(Core_InlineHashMapPPIterator* self);

/// @ingroup Core_Aggregates
/// @brief Advance this iterator.
/// @return #Core_Success on success. #Core_Failure on failure.
Core_Result Core_InlineHashMapPPIterator_next(Core_InlineHashMapPPIterator* self);

/// @ingroup Core_Aggregates
/// @param SELF A pointer to this iterator.
/// @success <code>*RETURN</code> was assigned @a true if there is an entry and was assigned @a false if there no entry.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
Core_Result Core_InlineHashMapPPIterator_hasEntry(Core_Boolean* RETURN, Core_InlineHashMapPPIterator* SELF);

/// @ingroup Core_Aggregates
/// @param RETURN A pointer to a Core_InlineHashMapPP_value variable.
/// @param SELF A pointer to this iterator.
/// @success <code>*RETURN</code> was assigned the value.
/// @method{}
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
Core_Result Core_InlineHashMapPPIterator_getValue(Core_InlineHashMapPP_Value* RETURN, Core_InlineHashMapPPIterator* SELF);

/// @ingroup Core_Aggregates
/// @param RETURN A pointer to a Core_InlineHashMapPP_key variable.
/// @param SELF A pointer to this iterator.
/// @success <code>*RETURN</code> was assigned the key.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN is a null pointer.
Core_Result Core_InlineHashMapPPIterator_getKey(Core_InlineHashMapPP_Key* RETURN, Core_InlineHashMapPPIterator* SELF);

/// @ingroup Core_Aggregates
/// @brief Remove the current entry.
/// @param SELF A pointer to this iterator.
/// @method-call
Core_Result Core_InlineHashMapPPIterator_remove(Core_InlineHashMapPPIterator* SELF);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup Core_Aggregates
/// @brief Append the keys of this pointer hashmap to the specified pointer array.
/// @param SELF A pointer to this pointer hashmap.
/// @param target A pointer to the target array.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a TARGET is a null pointer.
Core_Result Core_InlineHashMapPP_getKeys(Core_InlineHashMapPP* SELF, Core_InlineArrayListP* target);

/// @ingroup Core_Aggregates
/// @brief Append the values of this pointer hashmap to the specified pointer array.
/// @param SELF A pointer to this pointer hashmap.
/// @param target A pointer to the target array.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a TARGET is a null pointer.
Core_Result Core_InlineHashMapPP_getValues(Core_InlineHashMapPP* SELF, Core_InlineArrayListP* target);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_INLINE_POINTER_HASHMAP_H_INCLUDED
