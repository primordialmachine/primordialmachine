#if !defined(DX_CORE_STRING_ITERATOR_H_INCLUDED)
#define DX_CORE_STRING_ITERATOR_H_INCLUDED

#include "dx/core/object.h"

/// @brief The base of all string iterators for UTF-8 strings.
/// String iterators of dx.string and dx.string_buffer are all based on this.
/// @extends dx.object
DX_DECLARE_OBJECT_TYPE("dx.string_iterator",
                       dx_string_iterator,
                       dx_object);

static inline dx_string_iterator* DX_STRING_ITERATOR(void* p) {
  return (dx_string_iterator*)p;
}

struct dx_string_iterator {
  dx_object _parent;
};

static inline dx_string_iterator_dispatch* DX_STRING_ITERATOR_DISPATCH(void* p) {
  return (dx_string_iterator_dispatch*)p;
}

struct dx_string_iterator_dispatch {
  dx_object_dispatch _parent;
  dx_result (*has_value)(dx_bool *RETURN, dx_string_iterator* SELF);
  dx_result (*get_value)(uint32_t* RETURN, dx_string_iterator* SELF);
  dx_result (*next)(dx_string_iterator* SELF);

};

dx_result dx_string_iterator_construct(dx_string_iterator* SELF);

static inline dx_result dx_string_iterator_has_value(dx_bool *RETURN, dx_string_iterator* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_string_iterator, has_value, RETURN, SELF);
}

static inline dx_result dx_string_iterator_get_value(uint32_t* RETURN, dx_string_iterator* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_string_iterator, get_value, RETURN, SELF);
}

static inline dx_result dx_string_iterator_next(dx_string_iterator* SELF) {
  DX_OBJECT_VIRTUALCALL(dx_string_iterator, next, SELF);
}

#endif // DX_CORE_STRING_ITERATOR_H_INCLUDED