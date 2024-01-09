#if !defined(DX_DOCUMENT_H_INCLUDED)
#define DX_DOCUMENT_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief A range of a text given by its start offset in Bytes from the beginning of the text and its length in Bytes.
Core_declareObjectType("dx.text.range",
                       dx_text_range,
                       Core_Object);

static inline dx_text_range* DX_TEXT_RANGE(void* p) {
  return (dx_text_range*)p;
}

struct dx_text_range {
  Core_Object _parent;
  Core_Size start;
  Core_Size length;
};

static inline dx_text_range_Dispatch* DX_TEXT_RANGE_DISPATCH(void* p) {
  return (dx_text_range_Dispatch*)p;
}

struct dx_text_range_Dispatch {
  Core_Object_Dispatch _parent;
};

Core_Result dx_text_range_construct(dx_text_range* SELF, Core_Size start, Core_Size length);

Core_Result dx_text_range_create(dx_text_range** RETURN, Core_Size start, Core_Size length);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Core_declareObjectType("dx.text.document",
                       dx_text_document,
                       Core_Object);

static inline dx_text_document* DX_TEXT_DOCUMENT(void* p) {
  return (dx_text_document*)p;
}

struct dx_text_document {
  Core_Object _parent;
  dx_string_buffer* text;
  dx_object_array *lines;
};

static inline dx_text_document_Dispatch* DX_TEXT_DOCUMENT_DISPATCH(void* p) {
  return (dx_text_document_Dispatch*)p;
}

struct dx_text_document_Dispatch {
  Core_Object_Dispatch _parent;
};

/// @constructor{dx_text_document}
Core_Result dx_text_document_construct(dx_text_document* SELF);

/// @create-operator{dx_text_document}
Core_Result dx_text_document_create(dx_text_document** RETURN);

/// @brief Set the contents of this document from the specified string.
/// @param string A pointer to the string.
/// @method{dx_text_document}
Core_Result dx_text_document_set_text(dx_text_document* SELF, Core_String* string);

/// @brief Get the contents of this document as a string.
/// @param RETURN A pointer to a <code>Core_String*</code> variable.
/// @success <code>*RETURN</code> was assigned a pointer to the string object.
/// The caller acquired a reference to that string object.
/// @method{dx_text_document}
Core_Result dx_text_document_get_text(Core_String** RETURN, dx_text_document* SELF);

/// @brief Append text to this text document.
/// @param text The text.
/// @brief Append a string to the contents of this document.
/// @param string A pointer to the string.
/// @method{dx_text_document}
Core_Result dx_text_document_append_text(dx_text_document* SELF, Core_String* text);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_DOCUMENT_H_INCLUDED
