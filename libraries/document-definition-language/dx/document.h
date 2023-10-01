#if !defined(DX_DOCUMENT_H_INCLUDED)
#define DX_DOCUMENT_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A range of a text given by its start offset in Bytes from the beginning of the text and its length in Bytes.
DX_DECLARE_OBJECT_TYPE("dx.text.range",
                       dx_text_range,
                       dx_object);

static inline dx_text_range* DX_TEXT_RANGE(void* p) {
  return (dx_text_range*)p;
}

struct dx_text_range {
  dx_object _parent;
  dx_size start;
  dx_size length;
};

static inline dx_text_range_dispatch* DX_TEXT_RANGE_DISPATCH(void* p) {
  return (dx_text_range_dispatch*)p;
}

struct dx_text_range_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_text_range_construct(dx_text_range* SELF, dx_size start, dx_size length);

dx_result dx_text_range_create(dx_text_range** RETURN, dx_size start, dx_size length);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DECLARE_OBJECT_TYPE("dx.text.document",
                       dx_text_document,
                       dx_object);

static inline dx_text_document* DX_TEXT_DOCUMENT(void* p) {
  return (dx_text_document*)p;
}

struct dx_text_document {
  dx_object _parent;
  dx_string_buffer* text;
  dx_object_array *lines;
};

static inline dx_text_document_dispatch* DX_TEXT_DOCUMENT_DISPATCH(void* p) {
  return (dx_text_document_dispatch*)p;
}

struct dx_text_document_dispatch {
  dx_object_dispatch _parent;
};

dx_result dx_text_document_construct(dx_text_document* SELF);

dx_result dx_text_document_create(dx_text_document** RETURN);

dx_result dx_text_document_set_text(dx_text_document* SELF, dx_string* text);

dx_result dx_text_document_append_text(dx_text_document* SELF, dx_string* text);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_DOCUMENT_H_INCLUDED
