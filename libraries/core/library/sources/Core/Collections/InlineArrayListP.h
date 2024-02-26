/// @file dx/core/inline_pointer_array.h
/// @brief A deque for pointers.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.

#if !defined(CORE_COLLECTIONS_INLINEARRAYLISTP_H_INCLUDED)
#define CORE_COLLECTIONS_INLINEARRAYLISTP_H_INCLUDED

#include "dx/core/core.h"

/* http://localhost/core#core-inlinearraylist-element */
typedef void* Core_InlineArrayListP_Element;

/* http://localhost/core#core-inlinearraylist-addedcallback */
typedef void (Core_InlineArrayListP_AddedCallback)(Core_InlineArrayListP_Element* element);

/* http://localhost/core#core-inlinearraylist-removedcallback */
typedef void (Core_InlineArrayListP_RemovedCallback)(Core_InlineArrayListP_Element* element);

/* http://localhost/core#core-inlinearraylist */
typedef struct Core_InlineArrayListP Core_InlineArrayListP;

struct Core_InlineArrayListP {
  void* pimpl;
}; // struct Core_InlineArrayListP

/* http://localhost/core#core-inlinearraylist-configuration */
typedef struct Core_InlineArrayListP_Configuration {

  Core_InlineArrayListP_AddedCallback* addedCallback;

  Core_InlineArrayListP_RemovedCallback* removedCallback;

} Core_InlineArrayListP_Configuration;

/* http://localhost/core#core-inlinearraylist-initialize */
Core_Result Core_InlineArrayListP_initialize(Core_InlineArrayListP* SELF, Core_Size initialCapacity, Core_InlineArrayListP_Configuration const* configuration);

/* http://localhost/core#core-inlinearraylist-append */
Core_Result Core_InlineArrayListP_append(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element pointer);

/* http://localhost/core#core-inlinearraylist-appendmany */
Core_Result Core_InlineArrayListP_appendMany(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements);

/* http://localhost/core#core-inlinearraylist-clear */
Core_Result Core_InlineArrayListP_clear(Core_InlineArrayListP* SELF);

/* http://localhost/core#core-inlinearraylist-ensurefreecapacity */
Core_Result Core_InlineArrayListP_ensureFreeCapacity(Core_InlineArrayListP* SELF, Core_Size requiredFreeCapacity);

/* http://localhost/core#core-inlinearraylist-get */
Core_Result Core_InlineArrayListP_get(Core_InlineArrayListP_Element* RETURN, Core_InlineArrayListP const* SELF, Core_Size index);

/* http://localhost/core#core-inlinearraylist-getcapacity */
Core_Result Core_InlineArrayListP_getCapacity(Core_Size* RETURN, Core_InlineArrayListP const* SELF);

/* http://localhost/core#core-inlinearraylist-getfreecapacity */
Core_Result Core_InlineArrayListP_getFreeCapacity(Core_Size* RETURN, Core_InlineArrayListP const* SELF);

/* http://localhost/core#core-inlinearraylist-getsize */
Core_Result Core_InlineArrayListP_getSize(Core_Size* RETURN, Core_InlineArrayListP const* SELF);

/* http://localhost/core#core-inlinearraylist-increasecapacity */
Core_Result Core_InlineArrayListP_increaseCapacity(Core_InlineArrayListP* SELF, Core_Size additional_capacity);

/* http://localhost/core#core-inlinearraylist-insert */
Core_Result Core_InlineArrayListP_insert(Core_InlineArrayListP* SELF, Core_Size index, Core_InlineArrayListP_Element element);

/* http://localhost/core#core-inlinearraylist-insertmany */
Core_Result Core_InlineArrayListP_insertMany(Core_InlineArrayListP* SELF, Core_Size index, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements);

/* http://localhost/core#core-inlinearraylist-prepend */
Core_Result Core_InlineArrayListP_prepend(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element pointer);

/* http://localhost/core#core-inlinearraylist-prependmany */
Core_Result Core_InlineArrayListP_prependMany(Core_InlineArrayListP* SELF, Core_InlineArrayListP_Element const* elements, Core_Size numberOfElements);

/* http://localhost/core#core-inlinearraylist-removemany */
Core_Result Core_InlineArrayListP_removeMany (Core_InlineArrayListP* SELF, Core_Size start, Core_Size length);

/* http://localhost/core#core-inlinearraylist-removemanyback */
Core_Result Core_InlineArrayListP_removeManyBack(Core_InlineArrayListP* SELF, Core_Size n);

/* http://localhost/core#core-inlinearraylist-removemanyfront */
Core_Result Core_InlineArrayListP_removeManyFront(Core_InlineArrayListP* SELF, Core_Size n);

/* http://localhost/core#core-inlinearraylist-uninitialize */
void Core_InlineArrayListP_uninitialize(Core_InlineArrayListP* SELF);

#endif // CORE_COLLECTIONS_INLINEARRAYLISTP_H_INCLUDED
