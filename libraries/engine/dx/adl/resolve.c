#include "dx/adl/resolve.h"

#include "dx/assets.h"
#include "dx/adl/read.h"

static inline dx_string* _get_name(dx_adl_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(self->context->names, dx_adl_name_index_##name)

DX_DEFINE_OBJECT_TYPE("dx.adl.resolve",
                      dx_adl_resolve,
                      dx_object);

static void dx_adl_resolve_destruct(dx_adl_resolve* SELF) {
  dx_inline_pointer_array_uninitialize(&SELF->queue);
}

static void dx_adl_resolve_dispatch_construct(dx_adl_resolve_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_adl_resolve_construct(dx_adl_resolve* SELF, dx_adl_context* context) {
  dx_rti_type* TYPE = dx_adl_resolve_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration;
  configuration.added_callback = NULL;
  configuration.removed_callback = NULL;
  if (dx_inline_pointer_array_initialize(&SELF->queue, 0, &configuration)) {
    return DX_FAILURE;
  }
  SELF->context = context;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_adl_resolve_create(dx_adl_resolve** RETURN, dx_adl_context* context) {
  DX_CREATE_PREFIX(dx_adl_resolve)
  if (dx_adl_resolve_construct(SELF, context)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

static int setup_queue(dx_adl_resolve* self, bool include_unloaded, bool include_unresolved) {
  dx_inline_pointer_array_clear(&self->queue);
  dx_inline_pointer_hashmap_iterator iterator;
  dx_inline_pointer_hashmap_iterator_initialize(&iterator, &self->context->definitions->map);
  dx_bool has_entry = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
    return 1;
  }
  while (has_entry) {
    dx_adl_symbol* symbol = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&symbol, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return 1;
    }
    if ((!symbol->asset && include_unloaded) || (!symbol->resolved && include_unresolved)) {
      if (dx_inline_pointer_array_append(&self->queue, symbol)) {
        dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
        return 1;
      }
    }
    dx_inline_pointer_hashmap_iterator_next(&iterator);
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return 1;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  return 0;
}

int dx_adl_resolve_run(dx_adl_resolve* self) {
  // This reads all the top-level aka named aka referencable elements.
  // <root>
  //  <top-level-element>
  if (setup_queue(self, true, false)) {
    return 1;
  }
  dx_size n;
  if (dx_inline_pointer_array_get_size(&n, &self->queue)) {
    return 1;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_adl_symbol* symbol = NULL;
    if (dx_inline_pointer_array_get_at(&symbol, &self->queue, i)) {
      return 1;
    }
    if (!symbol->asset) {
      dx_adl_type_handler* reader = NULL;
      if (dx_inline_pointer_hashmap_get(&reader, &self->context->readers, symbol->type)) {
        return 1;
      }
      if (dx_adl_type_handler_read(&symbol->asset, reader, symbol->node, self->context)) {
        return 1;
      } // if
    } //if
  } // for

  // resolve references/read non-top-level elements
  do {
    if (setup_queue(self, true, true)) {
      return 1;
    } // if
    if (dx_inline_pointer_array_get_size(&n, &self->queue)) {
      return 1;
    }
    for (dx_size i = 0; i < n; ++i) {
      dx_adl_symbol* symbol = NULL;
      if (dx_inline_pointer_array_get_at(&symbol, &self->queue, i)) {
        return 1;
      }
      dx_adl_type_handler* reader = NULL;
      if (dx_inline_pointer_hashmap_get(&reader, &self->context->readers, symbol->type)) {
        return 1;
      }
      if (!symbol->asset) {
        continue;/*Proceed with speculative (the program is invalid) execution.*/
      }
      if (!symbol->asset) {
        if (dx_adl_type_handler_read(&symbol->asset, reader, symbol->node, self->context)) {
          return 1;
        }
      }
      if (dx_adl_type_handler_resolve(reader, symbol, self->context)) {
        return 1;
      }
    } // for
    if (dx_inline_pointer_array_get_size(&n, &self->queue)) {
      return 1;
    }
  } while (n);

  return 0;
}
