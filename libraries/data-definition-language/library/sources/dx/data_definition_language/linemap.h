#if !defined(DX_DATA_DEFINITION_LANGUAGE_LINEMAP_H_INCLUDED)
#define DX_DATA_DEFINITION_LANGUAGE_LINEMAP_H_INCLUDED

#include "dx/core.h"
#include "dx/data_definition_language/configuration.h"

// A linemap consists of disjoint consecutive pairs of lines the interval is specified by the starting index of the line and its length.
// 
// Example:
// - ''      yields [[0, 0]]
// - ' '     yields [[0, 1]]
// - '\n'    yields [[0, 1][1, 0]]
// - 'a'     yields [[0, 1]]
// - 'aa'    yields [[0, 2]]
// - '\n\r'  yields [[0, 2][2, 0]]
// - '\n\ra' yields [[0, 2][2, 1]]
//
// Line indices are zero based.
// Column indices are zero based.
// Input byte indices are zero based.
DX_DECLARE_OBJECT_TYPE("dx.ddl.linemap",
                       dx_ddl_linemap,
                       Core_Object);

static inline dx_ddl_linemap* DX_DDL_LINEMAP(void* p) {
  return (dx_ddl_linemap*)p;
}

typedef struct LINE {
  Core_Size start;
  Core_Size length;
} LINE;

struct dx_ddl_linemap {
  Core_Object _parent;
  LINE* lines;
  Core_Size number_of_lines;
};

static inline dx_ddl_linemap_dispatch* DX_DDL_LINEMAP_DISPATCH(void* p) {
  return (dx_ddl_linemap_dispatch*)p;
}

struct dx_ddl_linemap_dispatch {
  Core_Object_Dispatch _parent;
};

/// @brief Construct this linemap.
/// @param SELF A pointer to this linemap.
/// @method-call
Core_Result dx_ddl_linemap_construct(dx_ddl_linemap* SELF, char const* start, char const* end);

/// @brief Create this linemap.
/// @param RETURN A pointer to a <code>dx_ddl_linemap*</code> variable.
/// @param start A pointer to the beginning of the first Byte of the input.
/// @param end A pointer to the end of the last Byte of the input.
/// @success <code>*RETURN</code> was assigned a pointer to the linemap.
/// @method-call
Core_Result dx_ddl_linemap_create(dx_ddl_linemap** RETURN, char const* start, char const* end);

/// @brief Given the index of a Byte, get the index of the line the Byte is on.
/// A line termination character is on the line it terminates.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param SELF A pointer to this line map.
/// @param byte_index The index of the Byte.
/// @success <code>*RETURN</code> was assigned the of the index of the line the Byte is on.
/// @method-call
/// @error #DX_INVALID_ARGUMENT byte_index is greater than or equal to the number of Bytes.
Core_Result dx_ddl_linemap_get_line_index(Core_Size* RETURN, dx_ddl_linemap* SELF, Core_Size byte_index);

/// @brief Given the index of a Byte, get the index of the column the Byte is on.
/// @param RETURN A pointer to a <code>Core_Size</code> variable.
/// @param SELF A pointer to this line map.
/// @param byte_index The index of the Byte.
/// @success <code>*RETURN</code> was assigned the of the column the Byte is on.
/// @method-call
Core_Result dx_ddl_linemap_get_column_index(Core_Size* RETURN, dx_ddl_linemap* SELF, Core_Size byte_index);

#if defined(DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS) && DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS

Core_Result dx_ddl_linemap_tests();

#endif // DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS

#endif // DX_DATA_DEFINITION_LANGUAGE_LINEMAP_H_INCLUDED
