#if !defined(CORE_COLLECTIONS_INLINEARRAYLISTN8_H_INCLUDED)
#define CORE_COLLECTIONS_INLINEARRAYLISTN8_H_INCLUDED

#include "dx/core/core.h"

/* http://localhost/core#core-inlinearraylist-element */
typedef Core_Natural8 Core_InlineArrayListN8_Element;

/* http://localhost/core#core-inlinearraylist-addedcallback */
typedef void (Core_InlineArrayListN8_AddedCallback)(Core_InlineArrayListN8_Element* element);

/* http://localhost/core#core-inlinearraylist-removedcallback */
typedef void (Core_InlineArrayListN8_RemovedCallback)(Core_InlineArrayListN8_Element* element);

/* http://localhost/core#core-inlinearray */
typedef struct Core_InlineArrayListN8 Core_InlineArrayListN8;

// @todo Use the PIMPL idiom. See InlineArrayListP.(c|h) for an example.
struct Core_InlineArrayListN8 {

  /// @brief A pointer to an array of @a capacity elements.
  /// The first @a size elements are the contents of the buffer.
  char *elements;

  /// @brief The size, in elements, of this buffer.
  Core_Size size;

  /// @brief The capacity, in elements, of this buffer.
  Core_Size capacity;

}; // struct Core_InlineArrayListN8

/* http://localhost/core#core-inlinearraylist-configuration */
typedef struct Core_InlineArrayListN8_Configuration {

  Core_InlineArrayListN8_AddedCallback* addedCallback;

  Core_InlineArrayListN8_RemovedCallback* removedCallback;

} Core_InlineArrayListN8_Configuration;

/* http://localhost/core#core-inlinearraylist-initialize */
Core_Result Core_InlineArrayListN8_initialize(Core_InlineArrayListN8* SELF, Core_Size initialCapacity, Core_InlineArrayListN8_Configuration const* configuration);

/// @brief "steal" the backing array from this Core_InlineArrayListN8 object.
/// @param elements A pointer to a <code>char</code> variable.
/// @param numberOfElements A pointer to <code>Core_Size</code> variable.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_ArgumentInvalid @a numberOfElements is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success
/// <code>*elements</code> was assigned a pointer to an array of <code>*numberOfElements</code> elements.
/// <code>*numberOfElements</code> was assigned the number of elements.
/// The array pointed to by <code>*elements</code> must be deallocated using <code>Core_Memory_deallocate(void*)</code> when no longer referenced.
/// @method{Core_InlineArrayListN8}
Core_Result Core_InlineArrayListN8_steal(Core_InlineArrayListN8* SELF, char **bytes, Core_Size *number_of_bytes);

/// @brief Set elements.
/// @param elements A pointer to an array of @a numberOfElements elements.
/// @param numberOfElements The number of elements in the array pointed to by @a elements.
/// @error #Core_Error_ArgumentInvalid @a SELF is a null pointer
/// @error #Core_Error_ArgumentInvalid @a elements is a null pointer
/// @error #Core_Error_AllocationFailed an allocation failed
/// @success The elements in the array pointed to by @a elements replaced the previous elements.
/// @method{Core_InlineArrayListN8}
Core_Result Core_InlineArrayListN8_set(Core_InlineArrayListN8* SELF, char const* elements, Core_Size numberOfElements);

/// @brief Get a pointer to the elements of this array.
/// @param RETURN Apointer to a <code>void*</code> variable.
/// @remarks The pointer will invalidate if this array is modified.
/// @success <code>*RETURN</code> was assigned a pointer to the elements of this array.
/// @method{Core_InlineArrayListN8}
Core_Result Core_InlineArrayListN8_getElements(void** RETURN, Core_InlineArrayListN8* SELF);

/* http://localhost/core#core-inlinearraylist-append */
Core_Result Core_InlineArrayListN8_append(Core_InlineArrayListN8* SELF, Core_Natural8 element);

/* http://localhost/core#core-inlinearraylist-appendmany */
Core_Result Core_InlineArrayListN8_appendMany(Core_InlineArrayListN8* SELF, Core_Natural8 const* elements, Core_Size numberOfElements);

/* http://localhost/core#core-inlinearray-clear */
Core_Result Core_InlineArrayListN8_clear(Core_InlineArrayListN8* SELF);

/* http://localhost/core#core-inlinearray-ensurefreecapacity */
Core_Result Core_InlineArrayListN8_ensureFreeCapacity(Core_InlineArrayListN8* SELF, Core_Size required);

/* http://localhost/core#core-inlinearray-get */
Core_Result Core_InlineArrayListN8_get(Core_Natural8* RETURN, Core_InlineArrayListN8* SELF, Core_Size index);

/* http://localhost/core#core-inlinearray-getcapacity */
Core_Result Core_InlineArrayListN8_getCapacity(Core_Size* RETURN, Core_InlineArrayListN8 const* SELF);

/* http://localhost/core#core-inlinearray-getfreecapacity */
Core_Result Core_InlineArrayListN8_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayListN8 const* SELF);

/* http://localhost/core#core-inlinearray-getsize */
Core_Result Core_InlineArrayListN8_getSize(Core_Size* RETURN, Core_InlineArrayListN8 const* SELF);

/* http://localhost/core#core-inlinearray-increasecapacity */
Core_Result Core_InlineArrayListN8_increaseCapacity(Core_InlineArrayListN8* SELF, Core_Size additional);

/* http://localhost/core#core-inlinearray-insert */
Core_Result Core_InlineArrayListN8_insert(Core_InlineArrayListN8* SELF, Core_Size index, Core_Natural8 element);

/* http://localhost/core#core-inlinearray-insertmany */
Core_Result Core_InlineArrayListN8_insertMany(Core_InlineArrayListN8* SELF, Core_Size index, Core_Natural8 const* elements, Core_Size numberOfElements);

/* http://localhost/core#core-inlinearray-prepend */
Core_Result Core_InlineArrayListN8_prepend(Core_InlineArrayListN8* SELF, Core_Natural8 element);

/* http://localhost/core#core-inlinearray-prependmany */
Core_Result Core_InlineArrayListN8_prependMany(Core_InlineArrayListN8* SELF, Core_Natural8 const* elements, Core_Size numberOfElements);

/* http://localhost/core#core-inlinearray-removemany */
Core_Result Core_InlineArrayListN8_removeMany(Core_InlineArrayListN8* SELF, Core_Size start, Core_Size length);

/* http://localhost/core#core-inlinearray-removemanyback */
Core_Result Core_InlineArrayListN8_removeManyBack(Core_InlineArrayListN8* SELF, Core_Size n);

/* http://localhost/core#core-inlinearray-removemanyfront */
Core_Result Core_InlineArrayListN8_removeManyFront(Core_InlineArrayListN8* SELF, Core_Size n);

/* http://localhost/core#core-inlinearray-uninitialize */
void Core_InlineArrayListN8_uninitialize(Core_InlineArrayListN8* SELF);

#endif // CORE_COLLECTIONS_INLINEARRAYLISTN8_H_INCLUDED

