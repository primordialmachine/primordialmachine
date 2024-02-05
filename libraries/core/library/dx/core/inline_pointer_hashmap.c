#include "dx/core/inline_pointer_hashmap.h"

#include "Core/Memory.h"
#include "Core/safeMulNx.h"
#include "Core/Collections/InlineArrayListP.h"
#include "dx/core/_get_best_array_size.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_impl_node _dx_impl_node;

typedef struct _dx_impl _dx_impl;

/// @brief Set the capacity of this implemenation.
/// @param SELF A pointer to this implementation.
/// @param new_capacity The new capacity. Must not be @a 0.
/// @return The zero value on success. A non-zero value on failure.
/// @failure The function has set the error variable. In particular, the following error codes are set:
/// @error #Core_Error_ArgumentInvalid @a self is a null pointer.
/// @error #Core_Error_ArgumentInvalid @a new_capacity is smaller than @a 1 or greater than the greatest capacity.
/// @error #Core_Error_AllocationFailed an allocation failed.
static Core_Result _dx_impl_set_capacity(_dx_impl* SELF, Core_Size new_capacity);

static Core_Result _dx_impl_maybe_resize(_dx_impl* SELF);

/// @brief Initialize this implementation.
/// @param SELF A pointer to an uninitialized _dx_impl object.
/// @param configuration A pointer to a DX_POINTER_HASHMAP_CONFIGURATION object.
/// @return #Core_Success on success. #Core_Failure on failure.
static inline Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_InlineHashMapPP_Configuration const* configuration);

/// @brief Uninitialize this implementation.
/// @param SELF A pointer to an initialized _dx_impl object.
static inline void _dx_impl_uninitialize(_dx_impl* self);

/// @brief Clear this implementation.
/// @param SELF A pointer to this implementation.
/// @return #Core_Success on success. #Core_Failure on failure.
static inline Core_Result _dx_impl_clear(_dx_impl* self);

/// @brief Add or update an entry in this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @param value The value.
/// @param replace 
/// @error #DX_ERROR_ALREADY_EXISTS @a replace is Core_False and an entry for the specified key already exists
/// @error #Core_Error_ArgumentInvalid @a SELF was a null pointer
static inline Core_Result _dx_impl_set(_dx_impl* SELF, Core_InlineHashMapPP_Key key, Core_InlineHashMapPP_Value value, bool replace);

/// @brief Get the value of an entry in this hashmap.
/// @param RETURN A pointer to a <code>Core_InlineHashMapPP_value*</code> variable.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @success <code>*RETURN</code> was assigned a pointer to the value.
/// @method-call
/// @error #Core_Error_NotFound no entry for the specified key was found
/// @error #Core_Error_ArgumentInvalid @a SELF was a null pointer
static inline Core_Result _dx_impl_get(Core_InlineHashMapPP_Value* RETURN, _dx_impl* SELF, Core_InlineHashMapPP_Key key);

/// @brief Remove an entry from this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @method-call
/// @error #Core_Error_NotFound no entry for the specified key was found
/// @error #Core_Error_ArgumentInvalid @a self was a null pointer.
static inline Core_Result _dx_impl_remove(_dx_impl* SELF, Core_InlineHashMapPP_Key key);

/// @brief Get the size, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN was a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF was a null pointer.
static inline Core_Result _dx_impl_get_size(Core_Size* RETURN, _dx_impl const* SELF);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN was a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF was a null pointer.
static inline Core_Result _dx_impl_get_capacity(Core_Size* RETURN, _dx_impl const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a Core_Size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #Core_Error_ArgumentInvalid @a RETURN was a null pointer.
/// @error #Core_Error_ArgumentInvalid @a SELF was a null pointer.
static inline Core_Result _dx_impl_get_free_capacity(Core_Size* RETURN, _dx_impl const* SELF);

struct _dx_impl_node {
  _dx_impl_node* next;
  /// @brief The hash value.
  Core_Size hash_value;
  /// @brief The key.
  Core_InlineHashMapPP_Key key;
  /// @brief The value.
  Core_InlineHashMapPP_Value value;
};

typedef _dx_impl_node* _dx_impl_bucket;


struct _dx_impl {
  
  _dx_impl_bucket* buckets;
  /// @brief Size of this hashmap.
  Core_Size size;
  /// @brief The capacity of this hashmap.
  Core_Size capacity;
  
  /// @brief A pointer to the @a _dx_key_added_callback_impl1 function or a null pointer.
  Core_InlineHashMapPP_KeyAddedCallback* key_added_callback;
  /// @brief A pointer to the @a _dx_key_removed_callback_impl1 function or  a null pointer.
  Core_InlineHashMapPP_KeyRemovedCallback* key_removed_callback;
  
  /// @brief A pointer to the @a _dx_value_added_callback_impl1 function or a null pointer.
  Core_InlineHashMapPP_ValueAddedCallback* value_added_callback;
  /// @brief A pointer to the @a _dx_value_removed_callback_impl1 function or  a null pointer.
  Core_InlineHashMapPP_ValueRemovedCallback* value_removed_callback;
  
  /// @brief A pointer to the @a dx_hash_key_callback function or a null pointer.
  Core_InlineHashMapPP_HashKeyCallback* hash_key_callback;
  
  /// @brief A pointer to the @a dx_compare_keys_callback function or a null pointer.
  Core_InlineHashMapPP_CompareKeysCallback* compare_keys_callback;

};

#define _DX_IMPL_LEAST_CAPACITY 1

#define _DX_IMPL_GREATEST_CAPACITY (Core_Size_Greatest / sizeof(_dx_impl_bucket))

static _dx_impl_node** _dx_impl_allocate_bucket_array(Core_Size n) {
  Core_Size overflow;
  Core_Size n_bytes;
  Core_safeMulSz(&n_bytes, n, sizeof(_dx_impl_bucket), &overflow); // must succeed
  if (overflow) {
    Core_setError(Core_Error_AllocationFailed);
    return NULL;
  }
  _dx_impl_bucket* buckets = NULL;
  if (Core_Memory_allocate((void**)&buckets, n_bytes)) {
    return NULL;
  }
  for (Core_Size i = 0; i < n; ++i) {
    buckets[i] = NULL;
  }
  return buckets;
}

static Core_Result _dx_impl_set_capacity(_dx_impl* SELF, Core_Size new_capacity) {
  if (!SELF || !new_capacity) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size old_capacity = SELF->capacity;
  _dx_impl_bucket* old_buckets = SELF->buckets;
  _dx_impl_bucket* new_buckets = _dx_impl_allocate_bucket_array(new_capacity);
  if (!new_buckets) {
    Core_setError(Core_Error_AllocationFailed);
    return Core_Failure;
  }
  for (Core_Size i = 0; i < old_capacity; ++i) {
    _dx_impl_bucket* old_bucket = &old_buckets[i];
    while (*old_bucket) {
      _dx_impl_node* node = *old_bucket;
      *old_bucket = node->next;
      Core_Size new_hash_index = node->hash_value % new_capacity;
      node->next = new_buckets[new_hash_index];
      new_buckets[new_hash_index] = node;
    }
  }
  Core_Memory_deallocate(old_buckets);
  SELF->capacity = new_capacity;
  SELF->buckets = new_buckets;
  return Core_Success;
}

static Core_Result _dx_impl_maybe_resize(_dx_impl* SELF) {
  if (SELF->size > SELF->capacity) {
    Core_Size new_capacity;
    if (dx_get_best_array_size(&new_capacity, SELF->capacity, 1, _DX_IMPL_LEAST_CAPACITY, _DX_IMPL_GREATEST_CAPACITY, true)) {
      return Core_Failure;
    }
    return _dx_impl_set_capacity(SELF, new_capacity);
  }
  return Core_Success;
}

static inline Core_Result _dx_impl_initialize(_dx_impl* SELF, Core_InlineHashMapPP_Configuration const* configuration) {
  if (!SELF || !configuration) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  static Core_Size const INITIAL_CAPACITY = 8;
  SELF->buckets = _dx_impl_allocate_bucket_array(INITIAL_CAPACITY);
  if (!SELF->buckets) {
    return Core_Failure;
  }
  SELF->size = 0;
  SELF->capacity = INITIAL_CAPACITY;
  SELF->key_added_callback = configuration->keyAddedCallback;
  SELF->key_removed_callback = configuration->keyRemovedCallback;
  SELF->hash_key_callback = configuration->hashKeyCallback;
  SELF->compare_keys_callback = configuration->compareKeysCallback;
  SELF->value_added_callback = configuration->valueAddedCallback;
  SELF->value_removed_callback = configuration->valueRemovedCallback;
  return Core_Success;
}

static inline void _dx_impl_uninitialize(_dx_impl* SELF) {
  DX_DEBUG_ASSERT(NULL != SELF);
  _dx_impl_clear(SELF);
  DX_DEBUG_ASSERT(NULL != SELF->buckets);
  Core_Memory_deallocate(SELF->buckets);
  SELF->buckets = NULL;
}

static inline Core_Result _dx_impl_clear(_dx_impl* self) {
  if (!self) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  for (Core_Size i = 0, n = self->capacity; i < n; ++i) {
    _dx_impl_node** bucket = &self->buckets[i];
    while (*bucket) {
      _dx_impl_node* node = *bucket;
      *bucket = node->next;
      if (self->value_removed_callback) {
        self->value_removed_callback(&node->value);
      }
      if (self->key_removed_callback) {
        self->key_removed_callback(&node->key);
      }
      Core_Memory_deallocate(node);
    }
  }
  self->size = 0;
  return Core_Success;
}

static inline Core_Result _dx_impl_set(_dx_impl* SELF, Core_InlineHashMapPP_Key key, Core_InlineHashMapPP_Value value, Core_Boolean replace) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size hash_value;
  SELF->hash_key_callback(&hash_value, &key);
  Core_Size hash_index = hash_value % SELF->capacity;
  _dx_impl_node* node;
  for (node = SELF->buckets[hash_index]; NULL != node; node = node->next) {
    if (node->hash_value == hash_value) {
      Core_Boolean are_equal;
      SELF->compare_keys_callback(&are_equal, &node->key, &key);
      if (are_equal) {
        break;
      }
    }
  }
  if (node) {
    if (!replace) {
      Core_setError(Core_Error_Exists);
      return Core_Failure;
    }
    //
    if (SELF->key_added_callback) {
      SELF->key_added_callback(&key);
    }
    if (SELF->key_removed_callback) {
      SELF->key_removed_callback(&node->key);
    }
    node->key = key;
    //
    if (SELF->value_added_callback) {
      SELF->value_added_callback(&value);
    }
    if (SELF->value_removed_callback) {
      SELF->value_removed_callback(&node->value);
    }
    node->value = value;
  } else {
    if (Core_Memory_allocate(&node, sizeof(_dx_impl_node))) {
      return Core_Failure;
    }
    if (SELF->key_added_callback) {
      SELF->key_added_callback(&key);
    }
    node->key = key;
    if (SELF->value_added_callback) {
      SELF->value_added_callback(&value);
    }
    node->value = value;
    node->hash_value = hash_value;
    node->next = SELF->buckets[hash_index];
    SELF->buckets[hash_index] = node;
    SELF->size++;
    Core_Error old_error = Core_getError();
    if (_dx_impl_maybe_resize(SELF)) {
      Core_setError(old_error);
    }
  }
  return Core_Success;
}

static inline Core_Result _dx_impl_get(Core_InlineHashMapPP_Value* RETURN, _dx_impl* SELF, Core_InlineHashMapPP_Key key) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size hash_value;
  SELF->hash_key_callback(&hash_value, &key);
  Core_Size hash_index = hash_value % SELF->capacity;
  _dx_impl_node* node;
  for (node = SELF->buckets[hash_index]; NULL != node; node = node->next) {
    if (node->hash_value == hash_value) {
      Core_Boolean are_equal;
      SELF->compare_keys_callback(&are_equal, &node->key, &key);
      if (are_equal) {
        *RETURN = node->value;
        return Core_Success;
      }
    }
  }
  Core_setError(Core_Error_NotFound);
  return Core_Failure;
}

static inline Core_Result _dx_impl_remove(_dx_impl* SELF, Core_InlineHashMapPP_Key key) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  Core_Size hash_value;
  SELF->hash_key_callback(&hash_value, &key);
  Core_Size hash_index = hash_value % SELF->capacity;
  _dx_impl_node** previous;
  _dx_impl_node* current;
  for (previous = &SELF->buckets[hash_index], current = SELF->buckets[hash_index]; NULL != current; previous = &current->next, current = current->next) {
    if (current->hash_value == hash_value) {
      Core_Boolean are_equal;
      SELF->compare_keys_callback(&are_equal, &current->key, &key);
      if (are_equal) {
        *previous = current->next;
        if (SELF->value_removed_callback) {
          SELF->value_removed_callback(&current->value);
        }
        if (SELF->key_removed_callback) {
          SELF->key_removed_callback(&current->key);
        }
        Core_Memory_deallocate(current);
        return Core_Success;
      }
    }
  }
  Core_setError(Core_Error_NotFound);
  return Core_Failure;
}

static inline Core_Result _dx_impl_get_size(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->size;
  return Core_Success;
}

static inline Core_Result _dx_impl_get_capacity(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity;
  return Core_Success;
}

static inline Core_Result _dx_impl_get_free_capacity(Core_Size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  *RETURN = SELF->capacity - SELF->size;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

Core_Result Core_InlineHashMapPP_initialize(Core_InlineHashMapPP* SELF, Core_InlineHashMapPP_Configuration const* configuration) {
  if (!SELF || !configuration) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->pimpl = NULL;
  if (Core_Memory_allocate(&SELF->pimpl, sizeof(_dx_impl))) {
    return Core_Failure;
  }
  if (_dx_impl_initialize(_DX_IMPL(SELF->pimpl), configuration)) {
    Core_Memory_deallocate(SELF->pimpl);
    SELF->pimpl = NULL;
    return Core_Failure;
  }
  return Core_Success;
}

void Core_InlineHashMapPP_uninitialize(Core_InlineHashMapPP* self) {
  DX_DEBUG_ASSERT(NULL != self);
  DX_DEBUG_ASSERT(NULL != self->pimpl);
  _dx_impl_uninitialize(_DX_IMPL(self->pimpl));
  Core_Memory_deallocate(self->pimpl);
  self->pimpl = NULL;
}

Core_Result Core_InlineHashMapPP_clear(Core_InlineHashMapPP* self) {
  if (!self) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_clear(_DX_IMPL(self->pimpl));
}

Core_Result Core_InlineHashMapPP_set(Core_InlineHashMapPP* SELF, Core_InlineHashMapPP_Key key, Core_InlineHashMapPP_Value value) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_set(_DX_IMPL(SELF->pimpl), key, value, false);
}

Core_Result Core_InlineHashMapPP_get(Core_InlineHashMapPP_Value* RETURN, Core_InlineHashMapPP const* SELF, Core_InlineHashMapPP_Key key) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_get(RETURN, _DX_IMPL(SELF->pimpl), key);
}

Core_Result Core_InlineHashMapPP_remove(Core_InlineHashMapPP* SELF, Core_InlineHashMapPP_Key key) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_remove(_DX_IMPL(SELF->pimpl), key);
}

Core_Result Core_InlineHashMapPP_getSize(Core_Size* RETURN, Core_InlineHashMapPP const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_get_size(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result Core_InlineHashMapPP_getCapacity(Core_Size* RETURN, Core_InlineHashMapPP const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_get_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

Core_Result Core_InlineHashMapPP_getFreeCapacity(Core_Size* RETURN, Core_InlineHashMapPP const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return _dx_impl_get_free_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_impl_iterator _dx_impl_iterator;
static inline _dx_impl_iterator* _DX_IMPL_ITERATOR(void* p) {
 return (_dx_impl_iterator*)p; 
}
struct _dx_impl_iterator {
  _dx_impl* target;
  Core_Size bucket;
  _dx_impl_node** previous;
  _dx_impl_node* current;
};

static inline void _dx_impl_increment(_dx_impl_iterator* self);

static inline void _dx_impl_increment(_dx_impl_iterator* self) {
  if (self->current) {
    self->previous = &self->current->next;
    self->current = self->current->next;
    if (!self->current) {
      self->bucket++; // move to next bucket
      while (self->bucket < self->target->capacity) {
        self->previous = &self->target->buckets[self->bucket];
        self->current = self->target->buckets[self->bucket];
        if (self->current) {
          return;
        }
        self->bucket++;
      }
    }
  } else {
    if (self->bucket < self->target->capacity) {
      do {
        self->previous = &self->target->buckets[self->bucket];
        self->current = self->target->buckets[self->bucket];
        if (self->current != NULL) {
          break;
        }
        self->bucket++;
      } while (self->bucket < self->target->capacity);
    }
  }
}

Core_Result Core_InlineHashMapPPIterator_initialize(Core_InlineHashMapPPIterator* SELF, Core_InlineHashMapPP* target) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _dx_impl_iterator* pimpl = NULL;
  if (Core_Memory_allocate(&pimpl, sizeof(_dx_impl_iterator))) {
    return Core_Failure;
  }
  pimpl->target = _DX_IMPL(target->pimpl);
  pimpl->bucket = 0;
  pimpl->previous = NULL;
  pimpl->current = NULL;
  _dx_impl_increment(pimpl);
  SELF->pimpl = pimpl;
  return Core_Success;
}

void Core_InlineHashMapPPIterator_uninitialize(Core_InlineHashMapPPIterator* SELF) {
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  Core_Memory_deallocate(pimpl);
}

Core_Result Core_InlineHashMapPPIterator_next(Core_InlineHashMapPPIterator* SELF) {
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  _dx_impl_increment(pimpl);
  return Core_Success;
}

Core_Result Core_InlineHashMapPPIterator_hasEntry(Core_Boolean* RETURN, Core_InlineHashMapPPIterator* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  *RETURN = NULL != pimpl->current;
  return Core_Success;
}

Core_Result Core_InlineHashMapPPIterator_getValue(Core_InlineHashMapPP_Value* RETURN, Core_InlineHashMapPPIterator* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  if (!pimpl->current) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = pimpl->current->value;
  return Core_Success;
}

Core_Result Core_InlineHashMapPPIterator_getKey(Core_InlineHashMapPP_Key* RETURN, Core_InlineHashMapPPIterator* SELF) {
  if (!RETURN || !SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  if (!pimpl->current) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  *RETURN = pimpl->current->key;
  return Core_Success;
}

Core_Result Core_InlineHashMapPPIterator_remove(Core_InlineHashMapPPIterator* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  if (!pimpl->current) {
    Core_setError(Core_Error_OperationInvalid);
    return Core_Failure;
  }
  _dx_impl_node** previous = pimpl->previous;
  _dx_impl_node* current = pimpl->current;
  // Predecessor points to successor.
  *previous = current->next;
  // Decrement size by 1.
  pimpl->target->size--;
  // Update iterator.
  pimpl->previous = previous;
  pimpl->current = *previous;
  if (!pimpl->current) {
    // Only if the removed node was the last in the bucket and hence pimpl->current is now a null pointer.
    _dx_impl_increment(pimpl);
  }
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_Result Core_InlineHashMapPP_getKeys(Core_InlineHashMapPP* SELF, Core_InlineArrayListP* target) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  Core_InlineHashMapPPIterator it;
  if (Core_InlineArrayListP_ensureFreeCapacity(target, pimpl->size)) {
    return Core_Failure;
  }
  if (Core_InlineHashMapPPIterator_initialize(&it, SELF)) {
    return Core_Failure;
  }
  Core_Boolean has_entry;
  if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &it)) {
    Core_InlineHashMapPPIterator_uninitialize(&it);
    return Core_Failure;
  }
  while (has_entry) {
    Core_InlineHashMapPP_Key key = NULL;
    if (Core_InlineHashMapPPIterator_getKey(&key, &it)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
    if (Core_InlineArrayListP_append(target, key)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
    if (Core_InlineHashMapPPIterator_next(&it)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
    if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &it)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
  }
  Core_InlineHashMapPPIterator_uninitialize(&it);
  return Core_Success;
}

Core_Result Core_InlineHashMapPP_getValues(Core_InlineHashMapPP* SELF, Core_InlineArrayListP* target) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  Core_InlineHashMapPPIterator it;
  if (Core_InlineArrayListP_ensureFreeCapacity(target, pimpl->size)) {
    return Core_Failure;
  }
  if (Core_InlineHashMapPPIterator_initialize(&it, SELF)) {
    return Core_Failure;
  }
  Core_Boolean has_entry;
  if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &it)) {
    Core_InlineHashMapPPIterator_uninitialize(&it);
    return Core_Failure;
  }
  while (has_entry) {
    Core_InlineHashMapPP_Value value = NULL;
    if (Core_InlineHashMapPPIterator_getValue(&value, &it)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
    if (Core_InlineArrayListP_append(target, value)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
    if (Core_InlineHashMapPPIterator_next(&it)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
    if (Core_InlineHashMapPPIterator_hasEntry(&has_entry, &it)) {
      Core_InlineHashMapPPIterator_uninitialize(&it);
      return Core_Failure;
    }
  }
  Core_InlineHashMapPPIterator_uninitialize(&it);
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
