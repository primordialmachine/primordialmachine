#include "dx/data_definition_language/linemap.h"

DX_DEFINE_OBJECT_TYPE("dx.ddl.linemap",
                      dx_ddl_linemap,
                      Core_Object);

static void dx_ddl_linemap_destruct(dx_ddl_linemap* SELF) {
  Core_Memory_deallocate(SELF->lines);
  SELF->lines = NULL;
  SELF->number_of_lines = 0;
}

static void dx_ddl_linemap_constructDispatch(dx_ddl_linemap_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_ddl_linemap_construct(dx_ddl_linemap* SELF, char const* start, char const* end) {
  DX_CONSTRUCT_PREFIX(dx_ddl_linemap);
  // We convervatively estimate a number of lines equal to the number of input Bytes plus 1.
  Core_Size old_line_count = (end - start) + 1;
  Core_Size new_line_count = 0;
  {
    Core_Size overflow;
    Core_Size number_of_bytes;
    Core_safeMulSz(&number_of_bytes, old_line_count, sizeof(LINE), &overflow); // must succeed
    if (overflow) {
      Core_setError(Core_Error_ArgumentInvalid);
      return Core_Failure;
    }
    SELF->lines = NULL;
    if (Core_Memory_allocate(&SELF->lines, number_of_bytes)) {
      return Core_Failure;
    }
    Core_Memory_zero(SELF->lines, number_of_bytes);
  }
  LINE* line = &SELF->lines[new_line_count++];
  line->start = 0;
  line->length = 0;
  while (true) {
    if (start + line->start + line->length == end || start[line->start + line->length] == '\n' || start[line->start + line->length] == '\r') {
      if (start + line->start + line->length == end) {
        break;
      } /*else if (*current == '\n' || *current == '\r')*/ {
        char const old = start[line->start + line->length];
        line->length++;
        if (start + line->start + line->length != end && old != start[line->start + line->length] && (start[line->start + line->length] == '\n' || start[line->start + line->length] == '\r')) {
          line->length++;
        }
        LINE* old_line = line;
        line = &SELF->lines[new_line_count++];
        line->start = old_line->start + old_line->length;
      }
    } else {
      line->length++;
    }
  }
  if (Core_Memory_reallocate(&SELF->lines, SELF->lines, sizeof(LINE) * new_line_count)) {
    return Core_Failure;
  }
  SELF->number_of_lines = new_line_count;
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ddl_linemap_get_line_index(Core_Size* RETURN, dx_ddl_linemap* SELF, Core_Size byte_index) {
  for (Core_Size i = SELF->number_of_lines; i > 0; --i) {
    LINE const* line = &(SELF->lines[i - 1]);
    if (line->start <= byte_index && byte_index <= line->start + line->length) {
      *RETURN = i - 1;
      return Core_Success;
    }
  }
  return Core_Failure;
}

Core_Result dx_ddl_linemap_get_column_index(Core_Size* RETURN, dx_ddl_linemap* SELF, Core_Size byte_index) {
  Core_Size line_index;
  if (dx_ddl_linemap_get_line_index(&line_index, SELF, byte_index)) {
    return Core_Failure;
  }
  *RETURN = byte_index - SELF->lines[line_index].start;
  return Core_Success;
}

Core_Result dx_ddl_linemap_create(dx_ddl_linemap** RETURN, char const* start, char const* end) {
  DX_CREATE_PREFIX(dx_ddl_linemap);
  if (dx_ddl_linemap_construct(SELF, start, end)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

#if defined(DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS) && DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS

#include <string.h>

static Core_Result test(char const* p, Core_Size n, Core_Size expected, Core_Result(*check)(dx_ddl_linemap* linemap)) {
  dx_ddl_linemap* linemap = NULL;
  if (dx_ddl_linemap_create(&linemap, p, p + n)) {
    return Core_Failure;
  }
  if (expected != linemap->number_of_lines) {
    DX_UNREFERENCE(linemap);
    linemap = NULL;
    return Core_Failure;
  }
  if (check) {
    if (check(linemap)) {
      DX_UNREFERENCE(linemap);
      linemap = NULL;
      return Core_Failure;
    }
  }
  DX_UNREFERENCE(linemap);
  linemap = NULL;
  return Core_Success;
}

/// '' yields [0,0]
static Core_Result dx_ddl_linemap_check_i0j0(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 0) {
    return Core_Failure;
  }
  Core_Size li, ci;
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return Core_Failure;
  }
  if (li != 0) {
    return Core_Failure;
  }
  if (ci != 0) {
    return Core_Failure;
  }
  return Core_Success;
}

/// 'a' and ' ' both yield [0,1]
static Core_Result dx_ddl_linemap_check_i0j1(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 1) {
    return Core_Failure;
  }
  Core_Size li, ci;
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return Core_Failure;
  }
  if (li != 0) {
    return Core_Failure;
  }
  if (ci != 0) {
    return Core_Failure;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 1)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 1)) {
    return Core_Failure;
  }
  if (li != 0) {
    return Core_Failure;
  }
  if (ci != 1) {
    return Core_Failure;
  }
  //
  return Core_Success;
}

/// '\n' yields [0,1] [1,0]
static Core_Result dx_ddl_linemap_check_i0j1_i1j0(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 1) {
    return Core_Failure;
  }
  if (linemap->lines[1].start != 1 || linemap->lines[1].length != 0) {
    return Core_Failure;
  }
  Core_Size li, ci;
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return Core_Failure;
  }
  if (li != 0) {
    return Core_Failure;
  }
  if (ci != 0) {
    return Core_Failure;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 1)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 1)) {
    return Core_Failure;
  }
  if (li != 1) {
    return Core_Failure;
  }
  if (ci != 0) {
    return Core_Failure;
  }
  //
  return Core_Success;
}

/// '\na' yields [0,1] [1,1]
static Core_Result dx_ddl_linemap_check_i0j1_i1j1(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 1) {
    return Core_Failure;
  }
  if (linemap->lines[1].start != 1 || linemap->lines[1].length != 1) {
    return Core_Failure;
  }
  Core_Size li, ci;
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return Core_Failure;
  }
  if (li != 0) {
    return Core_Failure;
  }
  if (ci != 0) {
    return Core_Failure;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 1)) {
    Core_setError(Core_Error_NoError);
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 1)) {
    return Core_Failure;
  }
  if (li != 1) {
    return Core_Failure;
  }
  if (ci != 0) {
    return Core_Failure;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 2)) {
    return Core_Failure;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 2)) {
    return Core_Failure;
  }
  if (li != 1) {
    return Core_Failure;
  }
  if (li != 1) {
    return Core_Failure;
  }
  //
  return Core_Success;
}

Core_Result dx_ddl_linemap_tests() {
  if (test("", strlen(""), 1, &dx_ddl_linemap_check_i0j0)) {
    return Core_Failure;
  }
  if (test(" ", strlen(" "), 1, &dx_ddl_linemap_check_i0j1)) {
    return Core_Failure;
  }
  if (test("a", strlen("a"), 1, &dx_ddl_linemap_check_i0j1)) {
    return Core_Failure;
  }
  if (test("\n", strlen("\n"), 2, &dx_ddl_linemap_check_i0j1_i1j0)) {
    return Core_Failure;
  }
  if (test("\na", strlen("\na"), 2, &dx_ddl_linemap_check_i0j1_i1j1)) {
    return Core_Failure;
  }
  return Core_Success;
}

#endif // DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS
