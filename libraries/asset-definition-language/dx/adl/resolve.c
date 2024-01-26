#include "dx/adl/resolve.h"

#include "dx/assets.h"
#include "dx/adl/parser.h"

static inline Core_String* _get_name(dx_adl_names* names, Core_Size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_ADL_NAMES_NUMBER_OF_NAMES);
  Core_String* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(self->context->names, dx_adl_name_index_##name)

Core_defineObjectType("dx.adl.resolve",
                      dx_adl_resolve,
                      Core_Object);

static void dx_adl_resolve_destruct(dx_adl_resolve* SELF) {
  Core_InlineArrayListP_uninitialize(&SELF->queue);
}

static void dx_adl_resolve_constructDispatch(dx_adl_resolve_Dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_adl_resolve_construct(dx_adl_resolve* SELF, dx_adl_context* context) {
  DX_CONSTRUCT_PREFIX(dx_adl_resolve);
  Core_InlineArrayListP_Configuration configuration;
  configuration.addedCallback = NULL;
  configuration.removedCallback = NULL;
  if (Core_InlineArrayListP_initialize(&SELF->queue, 0, &configuration)) {
    return Core_Failure;
  }
  SELF->context = context;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_adl_resolve_create(dx_adl_resolve** RETURN, dx_adl_context* context) {
  DX_CREATE_PREFIX(dx_adl_resolve);
  if (dx_adl_resolve_construct(SELF, context)) {
    CORE_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

static Core_Result setup_queue(dx_adl_resolve* SELF, bool include_unloaded, bool include_unresolved) {
  Core_InlineArrayListP_clear(&SELF->queue);
  Core_InlinePointerHashmapIterator iterator;
  Core_InlinePointerHashmapIterator_initialize(&iterator, &SELF->context->definitions->map);
  Core_Boolean has_entry = false;
  if (Core_InlinePointerHashmapIterator_hasEntry(&has_entry, &iterator)) {
    Core_InlinePointerHashmapIterator_uninitialize(&iterator);
    return Core_Failure;
  }
  while (has_entry) {
    dx_adl_symbol* symbol = NULL;
    if (Core_InlinePointerHashmapIterator_getValue(&symbol, &iterator)) {
      Core_InlinePointerHashmapIterator_uninitialize(&iterator);
      return Core_Failure;
    }
    if ((!symbol->asset && include_unloaded) || (!symbol->resolved && include_unresolved)) {
      if (Core_InlineArrayListP_append(&SELF->queue, symbol)) {
        Core_InlinePointerHashmapIterator_uninitialize(&iterator);
        return Core_Failure;
      }
    }
    Core_InlinePointerHashmapIterator_next(&iterator);
    if (Core_InlinePointerHashmapIterator_hasEntry(&has_entry, &iterator)) {
      Core_InlinePointerHashmapIterator_uninitialize(&iterator);
      return Core_Failure;
    }
  }
  Core_InlinePointerHashmapIterator_uninitialize(&iterator);
  return Core_Success;
}

Core_Result dx_adl_resolve_run(dx_adl_resolve* SELF) {
  // This reads all the top-level aka named aka referencable elements.
  // <root>
  //  <top-level-element>
  if (setup_queue(SELF, true, false)) {
    return Core_Failure;
  }
  Core_Size n;
  if (Core_InlineArrayListP_getSize(&n, &SELF->queue)) {
    return Core_Failure;
  }
  for (Core_Size i = 0; i < n; ++i) {
    dx_adl_symbol* symbol = NULL;
    if (Core_InlineArrayListP_get(&symbol, &SELF->queue, i)) {
      return Core_Failure;
    }
    if (!symbol->asset) {
      dx_adl_type_handler* type_handler = NULL;
      if (Core_InlinePointerHashmap_get(&type_handler, &SELF->context->type_handlers, symbol->type)) {
        return Core_Failure;
      }
      if (dx_adl_type_handler_read(&symbol->asset, type_handler, symbol->node, SELF->context)) {
        return Core_Failure;
      } // if
    } //if
  } // for

  // resolve references/read non-top-level elements
  do {
    if (setup_queue(SELF, true, true)) {
      return Core_Failure;
    } // if
    if (Core_InlineArrayListP_getSize(&n, &SELF->queue)) {
      return Core_Failure;
    }
    for (Core_Size i = 0; i < n; ++i) {
      dx_adl_symbol* symbol = NULL;
      if (Core_InlineArrayListP_get(&symbol, &SELF->queue, i)) {
        return Core_Failure;
      }
      dx_adl_type_handler* type_handler = NULL;
      if (Core_InlinePointerHashmap_get(&type_handler, &SELF->context->type_handlers, symbol->type)) {
        return Core_Failure;
      }
      if (!symbol->asset) {
        continue;/*Proceed with speculative (the program is invalid) execution.*/
      }
      if (!symbol->asset) {
        if (dx_adl_type_handler_read(&symbol->asset, type_handler, symbol->node, SELF->context)) {
          return Core_Failure;
        }
      }
      if (dx_adl_type_handler_resolve(type_handler, symbol, SELF->context)) {
        return Core_Failure;
      }
    } // for
    if (Core_InlineArrayListP_getSize(&n, &SELF->queue)) {
      return Core_Failure;
    }
  } while (n);

  return Core_Success;
}
