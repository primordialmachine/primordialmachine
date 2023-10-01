#if !defined(DX_PROGRAM_SOURCE_H_INCLUDED)
#define DX_PROGRAM_SOURCE_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_ENUMERATION_TYPE("dx.val_program_text_type",
                            dx_val_program_text_type);

enum dx_val_program_text_type {
  DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT = 1,
  DX_VAL_PROGRAM_TEXT_TYPE_VERTEX = 2,
  DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT = 3,
};

/// @brief Representation of the source texts of a (parts of) programs.
/// The objects are tagged unions.
/// - If the tag #DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT or the tag #DX_VAL_PROGRAM_TEXT_TYPE_VERTEX is specified,
///   then dx_val_program_text::bytes and dx_program_text::number_of_bytes contain the (not necessarily well-formed) program text
///   of a fragment program and vertex program, respectively.
/// - If the tag #DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT is specified,
///   then dx_val_program_text::fragment_program_text and dx_val_program_text::vertex_program_text contain references to
///   dx_val_program_text objects of the type #DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT and #DX_VAL_PROGRAM_TEXT_TYPE_VERTEX, respectively.
DX_DECLARE_OBJECT_TYPE("dx.val.program_text",
                       dx_val_program_text,
                       dx_object);

static inline dx_val_program_text* DX_VAL_PROGRAM_TEXT(void* p) {
  return (dx_val_program_text*)p;
}

struct dx_val_program_text {
  dx_object _parent;
  dx_val_program_text* parent;
  dx_val_program_text_type type;
  union {
    // For DX_VAL_PROGRAM_TEXT_TYPE_VERTEX or DX_VAL_PROGRAM_TEXT_TYPE_FRAGMENT
    dx_string* program_text;
    // For DX_VAL_PROGRAM_TEXT_TYPE_VERTEX_FRAGMENT
    struct {
      dx_val_program_text* vertex_program_text;
      dx_val_program_text* fragment_program_text;
    };
  };
};

static inline dx_val_program_text_dispatch* DX_VAL_PROGRAM_TEXT_DISPATCH(void* p) {
  return (dx_val_program_text_dispatch*)p;
}

struct dx_val_program_text_dispatch {
  dx_object_dispatch _parent;
};

/// @param SELF A pointer to this program text object.
/// @param type The type of the program text.
/// @param path The path to the file to load the program text from.
/// @default-failure
/// @default-return
dx_result dx_val_program_text_construct_from_file(dx_val_program_text* SELF, dx_val_program_text_type type, dx_string* path);

/// @param SELF A pointer to this program text object.
/// @param vertex_program_text Pointer to a program text object of type DX_VAL_PROGRAM_SOURCE_TYPE_VERTEX.
/// @param fragment_program_text Pointer to a program text object of type DX_VAL_PROGRAM_SOURCE_TYPE_FRAGMENT.
/// @default-failure
/// @default-return
dx_result dx_val_program_text_construct(dx_val_program_text* SELF, dx_val_program_text *vertex_program_text, dx_val_program_text* fragment_program_text);

dx_result dx_val_program_text_create_from_file(dx_val_program_text** RETURN, dx_val_program_text_type type, dx_string* path);

dx_result dx_val_program_text_create(dx_val_program_text** RETURN, dx_val_program_text *vertex_program_text, dx_val_program_text* fragment_program_text);

/// @brief Prefix the program with a define of the specified name.
/// @code
/// #define [name] (1)
/// @endcode
/// @param SELF A pointer to this program text.
/// @param name A pointer to the name.
/// @default-failure
/// @default-return
dx_result dx_val_program_text_add_define(dx_val_program_text* SELF, dx_string* name);

#endif // DX_PROGRAM_SOURCE_H_INCLUDED
