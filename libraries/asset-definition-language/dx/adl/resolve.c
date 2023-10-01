#include "dx/adl/resolve.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"

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

static dx_result setup_queue(dx_adl_resolve* SELF, bool include_unloaded, bool include_unresolved) {
  dx_inline_pointer_array_clear(&SELF->queue);
  dx_inline_pointer_hashmap_iterator iterator;
  dx_inline_pointer_hashmap_iterator_initialize(&iterator, &SELF->context->definitions->map);
  dx_bool has_entry = false;
  if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
    dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
    return DX_FAILURE;
  }
  while (has_entry) {
    dx_adl_symbol* symbol = NULL;
    if (dx_inline_pointer_hashmap_iterator_get_value(&symbol, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return DX_FAILURE;
    }
    if ((!symbol->asset && include_unloaded) || (!symbol->resolved && include_unresolved)) {
      if (dx_inline_pointer_array_append(&SELF->queue, symbol)) {
        dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
        return DX_FAILURE;
      }
    }
    dx_inline_pointer_hashmap_iterator_next(&iterator);
    if (dx_inline_pointer_hashmap_iterator_has_entry(&has_entry, &iterator)) {
      dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
      return DX_FAILURE;
    }
  }
  dx_inline_pointer_hashmap_iterator_uninitialize(&iterator);
  return DX_SUCCESS;
}

dx_result dx_adl_resolve_run(dx_adl_resolve* SELF) {
  // This reads all the top-level aka named aka referencable elements.
  // <root>
  //  <top-level-element>
  if (setup_queue(SELF, true, false)) {
    return DX_FAILURE;
  }
  dx_size n;
  if (dx_inline_pointer_array_get_size(&n, &SELF->queue)) {
    return DX_FAILURE;
  }
  for (dx_size i = 0; i < n; ++i) {
    dx_adl_symbol* symbol = NULL;
    if (dx_inline_pointer_array_get_at(&symbol, &SELF->queue, i)) {
      return DX_FAILURE;
    }
    if (!symbol->asset) {
      dx_adl_type_handler* type_handler = NULL;
      if (dx_inline_pointer_hashmap_get(&type_handler, &SELF->context->type_handlers, symbol->type)) {
        return DX_FAILURE;
      }
      if (dx_adl_type_handler_read(&symbol->asset, type_handler, symbol->node, SELF->context)) {
        return DX_FAILURE;
      } // if
    } //if
  } // for

  // resolve references/read non-top-level elements
  do {
    if (setup_queue(SELF, true, true)) {
      return DX_FAILURE;
    } // if
    if (dx_inline_pointer_array_get_size(&n, &SELF->queue)) {
      return DX_FAILURE;
    }
    for (dx_size i = 0; i < n; ++i) {
      dx_adl_symbol* symbol = NULL;
      if (dx_inline_pointer_array_get_at(&symbol, &SELF->queue, i)) {
        return DX_FAILURE;
      }
      dx_adl_type_handler* type_handler = NULL;
      if (dx_inline_pointer_hashmap_get(&type_handler, &SELF->context->type_handlers, symbol->type)) {
        return DX_FAILURE;
      }
      if (!symbol->asset) {
        continue;/*Proceed with speculative (the program is invalid) execution.*/
      }
      if (!symbol->asset) {
        if (dx_adl_type_handler_read(&symbol->asset, type_handler, symbol->node, SELF->context)) {
          return DX_FAILURE;
        }
      }
      if (dx_adl_type_handler_resolve(type_handler, symbol, SELF->context)) {
        return DX_FAILURE;
      }
    } // for
    if (dx_inline_pointer_array_get_size(&n, &SELF->queue)) {
      return DX_FAILURE;
    }
  } while (n);

  return DX_SUCCESS;
}
