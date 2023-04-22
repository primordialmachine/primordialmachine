#include "dx/ddl/linemap.h"

DX_DEFINE_OBJECT_TYPE("dx.ddl.linemap",
                      dx_ddl_linemap,
                      dx_object);

static void dx_ddl_linemap_destruct(dx_ddl_linemap* SELF) {
  dx_memory_deallocate(SELF->lines);
  SELF->lines = NULL;
  SELF->number_of_lines = 0;
}

static void dx_ddl_linemap_dispatch_construct(dx_ddl_linemap_dispatch* SELF)
{/*Intentionally empty.*/}

dx_result dx_ddl_linemap_construct(dx_ddl_linemap* SELF, char const* start, char const* end) {
  dx_rti_type* TYPE = dx_ddl_linemap_get_type();
  if (!TYPE) {
    return DX_FAILURE;
  }
  // We convervatively estimate a number of lines equal to the number of input Bytes plus 1.
  dx_size old_line_count = (end - start) + 1;
  dx_size new_line_count = 0;
  {
    dx_size overflow;
    dx_size number_of_bytes = dx_mul_sz(old_line_count, sizeof(LINE), &overflow);
    if (overflow) {
      dx_set_error(DX_ERROR_INVALID_ARGUMENT);
      return DX_FAILURE;
    }
    SELF->lines = NULL;
    if (dx_memory_allocate(&SELF->lines, number_of_bytes)) {
      return DX_FAILURE;
    }
    dx_memory_zero(SELF->lines, number_of_bytes);
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
  SELF->lines = dx_memory_reallocate(SELF->lines, sizeof(LINE) * new_line_count);
  SELF->number_of_lines = new_line_count;
  DX_OBJECT(SELF)->type = TYPE;
  return DX_SUCCESS;
}

dx_result dx_ddl_linemap_get_line_index(dx_size* RETURN, dx_ddl_linemap* SELF, dx_size byte_index) {
  for (dx_size i = SELF->number_of_lines; i > 0; --i) {
    LINE const* line = &(SELF->lines[i - 1]);
    if (line->start <= byte_index && byte_index <= line->start + line->length) {
      *RETURN = i - 1;
      return DX_SUCCESS;
    }
  }
  return DX_FAILURE;
}

dx_result dx_ddl_linemap_get_column_index(dx_size* RETURN, dx_ddl_linemap* SELF, dx_size byte_index) {
  dx_size line_index;
  if (dx_ddl_linemap_get_line_index(&line_index, SELF, byte_index)) {
    return DX_FAILURE;
  }
  *RETURN = byte_index - SELF->lines[line_index].start;
  return DX_SUCCESS;
}

dx_result dx_ddl_linemap_create(dx_ddl_linemap** RETURN, char const* start, char const* end) {
  dx_ddl_linemap* SELF = DX_DDL_LINEMAP(dx_object_alloc(sizeof(dx_ddl_linemap)));
  if (!SELF) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_construct(SELF, start, end)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return DX_FAILURE;
  }
  *RETURN = SELF;
  return DX_SUCCESS;
}

#if defined(DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS) && DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS

#include <string.h>

static dx_result test(char const* p, dx_size n, dx_size expected, dx_result(*check)(dx_ddl_linemap* linemap)) {
  dx_ddl_linemap* linemap = NULL;
  if (dx_ddl_linemap_create(&linemap, p, p + n)) {
    return DX_FAILURE;
  }
  if (expected != linemap->number_of_lines) {
    DX_UNREFERENCE(linemap);
    linemap = NULL;
    return DX_FAILURE;
  }
  if (check) {
    if (check(linemap)) {
      DX_UNREFERENCE(linemap);
      linemap = NULL;
      return DX_FAILURE;
    }
  }
  DX_UNREFERENCE(linemap);
  linemap = NULL;
  return DX_SUCCESS;
}

/// '' yields [0,0]
static dx_result dx_ddl_linemap_check_i0j0(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 0) {
    return DX_FAILURE;
  }
  dx_size li, ci;
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return DX_FAILURE;
  }
  if (li != 0) {
    return DX_FAILURE;
  }
  if (ci != 0) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

/// 'a' and ' ' both yield [0,1]
static dx_result dx_ddl_linemap_check_i0j1(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 1) {
    return DX_FAILURE;
  }
  dx_size li, ci;
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return DX_FAILURE;
  }
  if (li != 0) {
    return DX_FAILURE;
  }
  if (ci != 0) {
    return DX_FAILURE;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 1)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 1)) {
    return DX_FAILURE;
  }
  if (li != 0) {
    return DX_FAILURE;
  }
  if (ci != 1) {
    return DX_FAILURE;
  }
  //
  return DX_SUCCESS;
}

/// '\n' yields [0,1] [1,0]
static dx_result dx_ddl_linemap_check_i0j1_i1j0(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 1) {
    return DX_FAILURE;
  }
  if (linemap->lines[1].start != 1 || linemap->lines[1].length != 0) {
    return DX_FAILURE;
  }
  dx_size li, ci;
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return DX_FAILURE;
  }
  if (li != 0) {
    return DX_FAILURE;
  }
  if (ci != 0) {
    return DX_FAILURE;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 1)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 1)) {
    return DX_FAILURE;
  }
  if (li != 1) {
    return DX_FAILURE;
  }
  if (ci != 0) {
    return DX_FAILURE;
  }
  //
  return DX_SUCCESS;
}

/// '\na' yields [0,1] [1,1]
static dx_result dx_ddl_linemap_check_i0j1_i1j1(dx_ddl_linemap* linemap) {
  if (linemap->lines[0].start != 0 || linemap->lines[0].length != 1) {
    return DX_FAILURE;
  }
  if (linemap->lines[1].start != 1 || linemap->lines[1].length != 1) {
    return DX_FAILURE;
  }
  dx_size li, ci;
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 0)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 0)) {
    return DX_FAILURE;
  }
  if (li != 0) {
    return DX_FAILURE;
  }
  if (ci != 0) {
    return DX_FAILURE;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 1)) {
    dx_set_error(DX_NO_ERROR);
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 1)) {
    return DX_FAILURE;
  }
  if (li != 1) {
    return DX_FAILURE;
  }
  if (ci != 0) {
    return DX_FAILURE;
  }
  //
  if (dx_ddl_linemap_get_line_index(&li, linemap, 2)) {
    return DX_FAILURE;
  }
  if (dx_ddl_linemap_get_column_index(&ci, linemap, 2)) {
    return DX_FAILURE;
  }
  if (li != 1) {
    return DX_FAILURE;
  }
  if (li != 1) {
    return DX_FAILURE;
  }
  //
  return DX_SUCCESS;
}

dx_result dx_ddl_linemap_tests() {
  if (test("", strlen(""), 1, &dx_ddl_linemap_check_i0j0)) {
    return DX_FAILURE;
  }
  if (test(" ", strlen(" "), 1, &dx_ddl_linemap_check_i0j1)) {
    return DX_FAILURE;
  }
  if (test("a", strlen("a"), 1, &dx_ddl_linemap_check_i0j1)) {
    return DX_FAILURE;
  }
  if (test("\n", strlen("\n"), 2, &dx_ddl_linemap_check_i0j1_i1j0)) {
    return DX_FAILURE;
  }
  if (test("\na", strlen("\na"), 2, &dx_ddl_linemap_check_i0j1_i1j1)) {
    return DX_FAILURE;
  }
  return DX_SUCCESS;
}

#endif // DX_DATA_DEFINITION_LANGUAGE_LINEMAP_WITH_TESTS
