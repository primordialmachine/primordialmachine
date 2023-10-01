#include "dx/core/signals.h"

#include "dx/core/memory.h"

typedef struct _signal _signal;

typedef struct _node _node;

struct _signal {
  _node* nodes;
};

struct _node {
  _node* next;
  dx_weak_reference* weak_reference;
};

dx_result _signal_create(_signal** RETURN) {
  _signal* SELF = NULL;
  if (dx_memory_allocate(&SELF, sizeof(_signal))) {
    return DX_FAILURE;
  }
  SELF->nodes = NULL;
  *RETURN = SELF;
  return DX_SUCCESS;
}

void _signal_destroy(_signal* SELF) {
  while (SELF->nodes) {
    _node* node = SELF->nodes;
    SELF->nodes = node->next;
    DX_UNREFERENCE(node->weak_reference);
    node->weak_reference = NULL;
    dx_memory_deallocate(node);
  }
  dx_memory_deallocate(SELF);
}

dx_result dx_signal_initialize(dx_signal* SELF) {
  if (!SELF) {
    dx_set_error(DX_ERROR_INVALID_ARGUMENT);
    return DX_FAILURE;
  }
  if (_signal_create((_signal**)&SELF->pimpl)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

void dx_signal_uninitialize(dx_signal* SELF) {
  _signal_destroy(SELF->pimpl);
}

dx_result dx_signal_connect_object(dx_connection_id* RETURN, dx_signal* SELF, dx_object* object) {
  dx_weak_reference* weak_reference = NULL;
  if (dx_weak_reference_create(&weak_reference, object)) {
    return DX_FAILURE;
  }
  if (dx_signal_connect_weak_reference(RETURN, SELF, weak_reference)) {
    DX_UNREFERENCE(weak_reference);
    weak_reference = NULL;
    return DX_FAILURE;
  }
  DX_UNREFERENCE(weak_reference);
  weak_reference = NULL;
  return DX_SUCCESS;
}

dx_result dx_signal_connect_weak_reference(dx_connection_id* RETURN, dx_signal* SELF, dx_weak_reference* weak_reference) {
  _node* node = NULL;
  if (dx_memory_allocate(&node, sizeof(_node))) {
    DX_UNREFERENCE(weak_reference);
    weak_reference = NULL;
    return DX_FAILURE;
  }
  DX_REFERENCE(weak_reference);
  node->weak_reference = weak_reference;
  _signal* signal = (_signal*)SELF->pimpl;
  node->next = signal->nodes;
  signal->nodes = node;
  *RETURN = (dx_connection_id)node;
  return DX_SUCCESS;
}

dx_result dx_signal_notify(dx_signal* SELF, dx_object* parameter) {
  return DX_SUCCESS;
}