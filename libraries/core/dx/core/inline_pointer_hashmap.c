#include "dx/core/inline_pointer_hashmap.h"

#include "dx/core/memory.h"
#include "dx/core/safe_mul_nx.h"
#include "dx/core/inline_pointer_array.h"
#include "dx/core/_get_best_array_size.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct _dx_impl_node _dx_impl_node;

typedef struct _dx_impl _dx_impl;

/// @brief Set the capacity of this implemenation.
/// @param self A pointer to this implementation.
/// @param new_capacity The new capacity. Must not be @a 0.
/// @return The zero value on success. A non-zero value on failure.
/// @failure The function has set the error variable. In particular, the following error codes are set:
/// @error #DX_ERROR_INVALID_ARGUMENT @a self is a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a new_capacity is smaller than @a 1 or greater than the greatest capacity.
/// @error #DX_ERROR_ALLOCATION_FAILED an allocation failed.
static dx_result _dx_impl_set_capacity(_dx_impl* self, dx_size new_capacity);

static dx_result _dx_impl_maybe_resize(_dx_impl* self);

/// @brief Initialize this implementation.
/// @param self A pointer to an uninitialized _dx_impl object.
/// @param configuration A pointer to a DX_POINTER_HASHMAP_CONFIGURATION object.
/// @return #DX_SUCCESS on success. #DX_FAILURE on failure.
static inline dx_result _dx_impl_initialize(_dx_impl* self, DX_INLINE_POINTER_HASHMAP_CONFIGURATION const* configuration);

/// @brief Uninitialize this implementation.
/// @param self A pointer to an initialized _dx_impl object.
static inline void _dx_impl_uninitialize(_dx_impl* self);

/// @brief Clear this implementation.
/// @param self A pointer to this implementation.
/// @return #DX_SUCCESS on success. #DX_FAILURE on failure.
static inline dx_result _dx_impl_clear(_dx_impl* self);

/// @brief Add or update an entry in this hashmap.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @param value The value.
/// @param replace 
/// @error #DX_ERROR_ALREADY_EXISTS @a replace is DX_FALSE and an entry for the specified key already exists
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF was a null pointer
static inline dx_result _dx_impl_set(_dx_impl* SELF, dx_inline_pointer_hashmap_key key, dx_inline_pointer_hashmap_value value, bool replace);

/// @brief Get the value of an entry in this hashmap.
/// @param RETURN A pointer to a <code>dx_inline_pointer_hashmap_value*</code> variable.
/// @param SELF A pointer to this hashmap.
/// @param key The key.
/// @success <code>*RETURN</code> was assigned a pointer to the value.
/// @method-call
/// @error #DX_ERROR_NOT_FOUND no entry for the specified key was found
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF was a null pointer
static inline dx_result _dx_impl_get(dx_inline_pointer_hashmap_value* RETURN, _dx_impl* SELF, dx_inline_pointer_hashmap_key key);

/// @brief Remove an entry from this hashmap.
/// @param self A pointer to this hashmap.
/// @param key The key.
/// @method-call
/// @error #DX_ERROR_NOT_FOUND no entry for the specified key was found
/// @error #DX_ERROR_INVALID_ARGUMENT @a self was a null pointer.
static inline dx_result _dx_impl_remove(_dx_impl* SELF, dx_inline_pointer_hashmap_key key);

/// @brief Get the size, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the size.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN was a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF was a null pointer.
static inline dx_result _dx_impl_get_size(dx_size* RETURN, _dx_impl const* SELF);

/// @brief Get the capacity, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN was a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF was a null pointer.
static inline dx_result _dx_impl_get_capacity(dx_size* RETURN, _dx_impl const* SELF);

/// @brief Get the free capacity, in elements.
/// @param RETURN A pointer to a dx_size variable.
/// @param SELF A pointer to this hashmap.
/// @success <code>*RETURN</code> was assigned the free capacity.
/// @method-call
/// @error #DX_ERROR_INVALID_ARGUMENT @a RETURN was a null pointer.
/// @error #DX_ERROR_INVALID_ARGUMENT @a SELF was a null pointer.
static inline dx_result _dx_impl_get_free_capacity(dx_size* RETURN, _dx_impl const* SELF);

struct _dx_impl_node {
  _dx_impl_node* next;
  /// @brief The hash value.
  dx_size hash_value;
  /// @brief The key.
  dx_inline_pointer_hashmap_key key;
  /// @brief The value.
  dx_inline_pointer_hashmap_value value;
};

typedef _dx_impl_node* _dx_impl_bucket;


struct _dx_impl {
  
  _dx_impl_bucket* buckets;
  /// @brief Size of this hashmap.
  dx_size size;
  /// @brief The capacity of this hashmap.
  dx_size capacity;
  
  /// @brief A pointer to the @a _dx_key_added_callback_impl1 function or a null pointer.
  dx_inline_pointer_hashmap_key_added_callback* key_added_callback;
  /// @brief A pointer to the @a _dx_key_removed_callback_impl1 function or  a null pointer.
  dx_inline_pointer_hashmap_key_removed_callback* key_removed_callback;
  
  /// @brief A pointer to the @a _dx_value_added_callback_impl1 function or a null pointer.
  dx_inline_pointer_hashmap_value_added_callback* value_added_callback;
  /// @brief A pointer to the @a _dx_value_removed_callback_impl1 function or  a null pointer.
  dx_inline_pointer_hashmap_value_removed_callback* value_removed_callback;
  
  /// @brief A pointer to the @a dx_hash_key_callback function or a null pointer.
  dx_inline_pointer_hashmap_hash_key_callback* hash_key_callback;
  
  /// @brief A pointer to the @a dx_compare_keys_callback function or a null pointer.
  dx_inline_pointer_hashmap_compare_keys_callback* compare_keys_callback;

};

#define _DX_IMPL_LEAST_CAPACITY 1

#define _DX_IMPL_GREATEST_CAPACITY (DX_SIZE_GREATEST / sizeof(_dx_impl_bucket))

static _dx_impl_node** _dx_impl_allocate_bucket_array(dx_size n) {
  dx_size overflow;
  dx_size n_bytes = dx_mul_sz(n, sizeof(_dx_impl_bucket), &overflow);
  if (overflow) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return NULL;
  }
  _dx_impl_bucket* buckets = NULL;
  if (dx_memory_allocate(&buckets, n_bytes)) {
    return NULL;
  }
  for (dx_size i = 0; i < n; ++i) {
    buckets[i] = NULL;
  }
  return buckets;
}

static dx_result _dx_impl_set_capacity(_dx_impl* self, dx_size new_capacity) {
  if (!self || !new_capacity) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size old_capacity = self->capacity;
  _dx_impl_bucket* old_buckets = self->buckets;
  _dx_impl_bucket* new_buckets = _dx_impl_allocate_bucket_array(new_capacity);
  if (!new_buckets) {
    dx_set_error(DX_ERROR_ALLOCATION_FAILED);
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < old_capacity; ++i) {
    _dx_impl_bucket* old_bucket = &old_buckets[i];
    while (*old_bucket) {
      _dx_impl_node* node = *old_bucket;
      *old_bucket = node->next;
      dx_size new_hash_index = node->hash_value % new_capacity;
      node->next = new_buckets[new_hash_index];
      new_buckets[new_hash_index] = node;
    }
  }
  dx_memory_deallocate(old_buckets);
  self->capacity = new_capacity;
  self->buckets = new_buckets;
  return DX_SUCCESS;
}

static dx_result _dx_impl_maybe_resize(_dx_impl* self) {
  if (self->size > self->capacity) {
    dx_size new_capacity;
    if (dx_get_best_array_size(&new_capacity, self->capacity, 1, _DX_IMPL_LEAST_CAPACITY, _DX_IMPL_GREATEST_CAPACITY, true)) {
      return DX_FAILURE;
    }
    return _dx_impl_set_capacity(self, new_capacity);
  }
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_initialize(_dx_impl* self, DX_INLINE_POINTER_HASHMAP_CONFIGURATION const* configuration) {
  if (!self || !configuration) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  static dx_size const INITIAL_CAPACITY = 8;
  self->buckets = _dx_impl_allocate_bucket_array(INITIAL_CAPACITY);
  if (!self->buckets) {
    return DX_FAILURE;
  }
  self->size = 0;
  self->capacity = INITIAL_CAPACITY;
  self->key_added_callback = configuration->key_added_callback;
  self->key_removed_callback = configuration->key_removed_callback;
  self->hash_key_callback = configuration->hash_key_callback;
  self->compare_keys_callback = configuration->compare_keys_callback;
  
  self->value_added_callback = configuration->value_added_callback;
  self->value_removed_callback = configuration->value_removed_callback;
  return DX_SUCCESS;
}

static inline void _dx_impl_uninitialize(_dx_impl* self) {
  DX_DEBUG_ASSERT(NULL != self);
  _dx_impl_clear(self);
  DX_DEBUG_ASSERT(NULL != self->buckets);
  dx_memory_deallocate(self->buckets);
  self->buckets = NULL;
}

static inline dx_result _dx_impl_clear(_dx_impl* self) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  for (dx_size i = 0, n = self->capacity; i < n; ++i) {
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
      dx_memory_deallocate(node);
    }
  }
  self->size = 0;
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_set(_dx_impl* SELF, dx_inline_pointer_hashmap_key key, dx_inline_pointer_hashmap_value value, dx_bool replace) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size hash_value;
  SELF->hash_key_callback(&hash_value, &key);
  dx_size hash_index = hash_value % SELF->capacity;
  _dx_impl_node* node;
  for (node = SELF->buckets[hash_index]; NULL != node; node = node->next) {
    if (node->hash_value == hash_value) {
      dx_bool are_equal;
      SELF->compare_keys_callback(&are_equal, &node->key, &key);
      if (are_equal) {
        break;
      }
    }
  }
  if (node) {
    if (!replace) {
      dx_set_error(DX_ERROR_EXISTS);
      return DX_FAILURE;
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
    if (dx_memory_allocate(&node, sizeof(_dx_impl_node))) {
      return DX_FAILURE;
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
    dx_error old_error = dx_get_error();
    if (_dx_impl_maybe_resize(SELF)) {
      dx_set_error(old_error);
    }
  }
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_get(dx_inline_pointer_hashmap_value* RETURN, _dx_impl* SELF, dx_inline_pointer_hashmap_key key) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size hash_value;
  SELF->hash_key_callback(&hash_value, &key);
  dx_size hash_index = hash_value % SELF->capacity;
  _dx_impl_node* node;
  for (node = SELF->buckets[hash_index]; NULL != node; node = node->next) {
    if (node->hash_value == hash_value) {
      dx_bool are_equal;
      SELF->compare_keys_callback(&are_equal, &node->key, &key);
      if (are_equal) {
        *RETURN = node->value;
        return DX_SUCCESS;
      }
    }
  }
  dx_set_error(DX_ERROR_NOT_FOUND);
  return DX_FAILURE;
}

static inline dx_result _dx_impl_remove(_dx_impl* SELF, dx_inline_pointer_hashmap_key key) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  dx_size hash_value;
  SELF->hash_key_callback(&hash_value, &key);
  dx_size hash_index = hash_value % SELF->capacity;
  _dx_impl_node** previous;
  _dx_impl_node* current;
  for (previous = &SELF->buckets[hash_index], current = SELF->buckets[hash_index]; NULL != current; previous = &current->next, current = current->next) {
    if (current->hash_value == hash_value) {
      dx_bool are_equal;
      SELF->compare_keys_callback(&are_equal, &current->key, &key);
      if (are_equal) {
        *previous = current->next;
        if (SELF->value_removed_callback) {
          SELF->value_removed_callback(&current->value);
        }
        if (SELF->key_removed_callback) {
          SELF->key_removed_callback(&current->key);
        }
        dx_memory_deallocate(current);
        return DX_SUCCESS;
      }
    }
  }
  dx_set_error(DX_ERROR_NOT_FOUND);
  return DX_FAILURE;
}

static inline dx_result _dx_impl_get_size(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->size;
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_get_capacity(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity;
  return DX_SUCCESS;
}

static inline dx_result _dx_impl_get_free_capacity(dx_size* RETURN, _dx_impl const* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  *RETURN = SELF->capacity - SELF->size;
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static inline _dx_impl* _DX_IMPL(void* p) {
  return (_dx_impl*)p;
}

dx_result dx_inline_pointer_hashmap_initialize(dx_inline_pointer_hashmap* self, DX_INLINE_POINTER_HASHMAP_CONFIGURATION const* configuration) {
  if (!self || !configuration) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  self->pimpl = NULL;
  if (dx_memory_allocate(&self->pimpl, sizeof(_dx_impl))) {
    return DX_FAILURE;
  }
  if (_dx_impl_initialize(_DX_IMPL(self->pimpl), configuration)) {
    dx_memory_deallocate(self->pimpl);
    self->pimpl = NULL;
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

void dx_inline_pointer_hashmap_uninitialize(dx_inline_pointer_hashmap* self) {
  DX_DEBUG_ASSERT(NULL != self);
  DX_DEBUG_ASSERT(NULL != self->pimpl);
  _dx_impl_uninitialize(_DX_IMPL(self->pimpl));
  dx_memory_deallocate(self->pimpl);
  self->pimpl = NULL;
}

dx_result dx_inline_pointer_hashmap_clear(dx_inline_pointer_hashmap* self) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_clear(_DX_IMPL(self->pimpl));
}

dx_result dx_inline_pointer_hashmap_set(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_hashmap_key key, dx_inline_pointer_hashmap_value value) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_set(_DX_IMPL(SELF->pimpl), key, value, false);
}

dx_result dx_inline_pointer_hashmap_get(dx_inline_pointer_hashmap_value* RETURN, dx_inline_pointer_hashmap const* self, dx_inline_pointer_hashmap_key key) {
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_get(RETURN, _DX_IMPL(self->pimpl), key);
}

dx_result dx_inline_pointer_hashmap_remove(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_hashmap_key key) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_remove(_DX_IMPL(SELF->pimpl), key);
}

dx_result dx_inline_pointer_hashmap_get_size(dx_size* RETURN, dx_inline_pointer_hashmap const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_get_size(RETURN, _DX_IMPL(SELF->pimpl));
}

dx_result dx_inline_pointer_hashmap_get_capacity(dx_size* RETURN, dx_inline_pointer_hashmap const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  return _dx_impl_get_capacity(RETURN, _DX_IMPL(SELF->pimpl));
}

dx_result dx_inline_pointer_hashmap_get_free_capacity(dx_size* RETURN, dx_inline_pointer_hashmap const* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
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
  dx_size bucket;
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

dx_result dx_inline_pointer_hashmap_iterator_initialize(dx_inline_pointer_hashmap_iterator* SELF, dx_inline_pointer_hashmap* target) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  _dx_impl_iterator* pimpl = NULL;
  if (dx_memory_allocate(&pimpl, sizeof(_dx_impl_iterator))) {
    return DX_FAILURE;
  }
  pimpl->target = _DX_IMPL(target->pimpl);
  pimpl->bucket = 0;
  pimpl->previous = NULL;
  pimpl->current = NULL;
  _dx_impl_increment(pimpl);
  SELF->pimpl = pimpl;
  return DX_SUCCESS;
}

void dx_inline_pointer_hashmap_iterator_uninitialize(dx_inline_pointer_hashmap_iterator* SELF) {
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  dx_memory_deallocate(pimpl);
}

dx_result dx_inline_pointer_hashmap_iterator_next(dx_inline_pointer_hashmap_iterator* SELF) {
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  _dx_impl_increment(pimpl);
  return DX_SUCCESS;
}

dx_result dx_inline_pointer_hashmap_iterator_has_entry(dx_bool* RETURN, dx_inline_pointer_hashmap_iterator* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  *RETURN = NULL != pimpl->current;
  return DX_SUCCESS;
}

dx_result dx_inline_pointer_hashmap_iterator_get_value(dx_inline_pointer_hashmap_value* RETURN, dx_inline_pointer_hashmap_iterator* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  if (!pimpl->current) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return DX_FAILURE;
  }
  *RETURN = pimpl->current->value;
  return DX_SUCCESS;
}

dx_result dx_inline_pointer_hashmap_iterator_get_key(dx_inline_pointer_hashmap_key* RETURN, dx_inline_pointer_hashmap_iterator* SELF) {
  if (!RETURN || !SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  if (!pimpl->current) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return DX_FAILURE;
  }
  *RETURN = pimpl->current->key;
  return DX_SUCCESS;
}

dx_result dx_inline_pointer_hashmap_iterator_remove(dx_inline_pointer_hashmap_iterator* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  _dx_impl_iterator* pimpl = _DX_IMPL_ITERATOR(SELF->pimpl);
  if (!pimpl->current) {
    dx_set_error(DX_ERROR_INVALID_OPERATION);
    return DX_FAILURE;
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
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

dx_result dx_inline_pointer_hashmap_get_keys(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_array* target) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  dx_inline_pointer_hashmap_iterator it;
  if (dx_inline_pointer_array_ensure_free_capacity(target, pimpl->size)) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_iterator_initialize(&it, SELF)) {
    return DX_FAILURE;
  }
  dx_bool has_entry;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &it)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&it);
    return DX_FAILURE;
  }
  while (has_entry) {
    dx_inline_pointer_hashmap_key key = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_key(&key, &it)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
    if (dx_inline_pointer_array_append(target, key)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
    if (dx_inline_pointer_hashmap_iterator_next(&it)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &it)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&it);
  return DX_SUCCESS;
}

dx_result dx_inline_pointer_hashmap_get_values(dx_inline_pointer_hashmap* SELF, dx_inline_pointer_array* target) {
  _dx_impl* pimpl = _DX_IMPL(SELF->pimpl);
  dx_inline_pointer_hashmap_iterator it;
  if (dx_inline_pointer_array_ensure_free_capacity(target, pimpl->size)) {
    return DX_FAILURE;
  }
  if (dx_inline_pointer_hashmap_iterator_initialize(&it, SELF)) {
    return DX_FAILURE;
  }
  dx_bool has_entry;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &it)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&it);
    return DX_FAILURE;
  }
  while (has_entry) {
    dx_inline_pointer_hashmap_value value = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&value, &it)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
    if (dx_inline_pointer_array_append(target, value)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
    if (dx_inline_pointer_hashmap_iterator_next(&it)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &it)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&it);
      return DX_FAILURE;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&it);
  return DX_SUCCESS;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
