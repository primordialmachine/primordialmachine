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

static void dx_adl_resolve_destruct(dx_adl_resolve* self) {
  dx_inline_pointer_array_uninitialize(&self->queue);
}

static void dx_adl_resolve_dispatch_construct(dx_adl_resolve_dispatch* self)
{/*Intentionally empty.*/}

int dx_adl_resolve_construct(dx_adl_resolve* self, dx_adl_context* context) {
  dx_rti_type* _type = dx_adl_resolve_get_type();
  if (!_type) {
    return DX_FAILURE;
  }
  if (!self) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  DX_INLINE_POINTER_ARRAY_CONFIGURATION configuration;
  configuration.added_callback = NULL;
  configuration.removed_callback = NULL;
  if (dx_inline_pointer_array_initialize(&self->queue, 0, &configuration)) {
    return DX_FAILURE;
  }
  self->context = context;
  DX_OBJECT(self)->type = _type;
  return DX_SUCCESS;
}

dx_adl_resolve* dx_adl_resolve_create(dx_adl_context* context) {
  dx_rti_type* _type = dx_adl_resolve_get_type();
  if (!_type) {
    return NULL;
  }
  dx_adl_resolve* self = DX_ADL_RESOLVE(dx_object_alloc(sizeof(dx_adl_resolve)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_resolve_construct(self, context)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
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
      symbol->asset = dx_adl_type_handler_read(reader, symbol->node, self->context);
      if (!symbol->asset) {
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
        symbol->asset = dx_adl_type_handler_read(reader, symbol->node, self->context);
        if (!symbol->asset) {
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
