#include "dx/data_definition_language/parser.h"

#include <string.h>

DX_DEFINE_OBJECT_TYPE("dx.ddl.parser",
                      dx_ddl_parser,
                      Core_Object);

/// @brief Get if the current word is of the specified word type.
/// @param SELF A pointer to this parser.
/// @param word_kind The word type.
/// @return @a true if the current word is of the specified word type. @a false if it is not. @a false is also returned on failure.
/// @method-call
/// @internal
static bool dx_ddl_parser_is_word_kind(dx_ddl_parser const* SELF, dx_data_definition_language_word_kind word_kind);

/// @code
/// value := STRING | NUMBER | map | list
/// @endcode
static dx_ddl_node* dx_ddl_parser_on_value(dx_ddl_parser* p);

// @code
// map := LEFT_CURLY_BRACKET map_body RIGHT_CURLY_BRACKET
// map_body := e
//           | map_body_element map_body_rest
// map_body_rest := COMMA map_body_element map_body_rest
//                | COMMA
//                | e
// map_body_element := NAME COLON value
// @endcode
static dx_ddl_node* dx_ddl_parser_on_map(dx_ddl_parser* p);

static int dx_ddl_parser_on_map_0(dx_ddl_parser* p, dx_ddl_node* map_node);

// @code
// list := LEFT_SQUARE_BRACKET list_body RIGHT_SQUARE_BRACKET
// list_body := e
//            | list_element list_body_rest
// list_body_rest := COMMA list_element list_body_rest
//                 | COMMA
//                 | e
// @endcode
static dx_ddl_node* dx_ddl_parser_on_list(dx_ddl_parser* p);

static int dx_ddl_parser_on_list_0(dx_ddl_parser* p, dx_ddl_node* list_node);

static Core_Result dx_ddl_parser_next(dx_ddl_parser* SELF);

static bool dx_ddl_parser_is_word_kind(dx_ddl_parser const* SELF, dx_data_definition_language_word_kind word_kind) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return false;
  }
  dx_data_definition_language_word_kind word_kind_received;
  if (dx_ddl_parser_get_word_kind(&word_kind_received, SELF)) {
    return false;
  }
  return Core_getError() ? false : word_kind_received == word_kind;
}

static dx_ddl_node* dx_ddl_parser_on_value(dx_ddl_parser* p) {
  dx_data_definition_language_word_kind word_kind;
  if (dx_ddl_parser_get_word_kind(&word_kind, p)) {
    return NULL;
  }
  switch (word_kind) {
    case dx_data_definition_language_word_kind_number: {
      dx_ddl_node* node = NULL;
      if (dx_ddl_node_create(&node, dx_ddl_node_kind_number)) {
        return NULL;
      }
      Core_String* number_literal = NULL;
      if (Core_String_create(&number_literal, p->scanner->text.elements, p->scanner->text.size)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      if (dx_ddl_node_set_number(node, number_literal)) {
        DX_UNREFERENCE(number_literal);
        number_literal = NULL;
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      DX_UNREFERENCE(number_literal);
      number_literal = NULL;
      if (dx_ddl_parser_next(p)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      return node;
    } break;
    case dx_data_definition_language_word_kind_string: {
      dx_ddl_node* node = NULL;
      if (dx_ddl_node_create(&node, dx_ddl_node_kind_string)) {
        return NULL;
      }
      Core_String* string_literal = NULL;
      if (Core_String_create(&string_literal, p->scanner->text.elements, p->scanner->text.size)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      if (dx_ddl_node_set_string(node, string_literal)) {
        DX_UNREFERENCE(string_literal);
        string_literal = NULL;
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      DX_UNREFERENCE(string_literal);
      string_literal = NULL;
      if (dx_ddl_parser_next(p)) {
        DX_UNREFERENCE(node);
        node = NULL;
        return NULL;
      }
      return node;
    } break;
    case dx_data_definition_language_word_kind_left_curly_bracket: {
      return dx_ddl_parser_on_map(p);
    } break;
    case dx_data_definition_language_word_kind_left_square_bracket: {
      return dx_ddl_parser_on_list(p);
    } break;
    default: {
      Core_setError(Core_Error_SyntacticalAnalysisFailed);
      return NULL;
    } break;
  }
}

static int dx_ddl_parser_on_map_0(dx_ddl_parser* p, dx_ddl_node* map_node) {
  if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_left_curly_bracket)) {
    if (!Core_getError()) {
      Core_setError(Core_Error_SyntacticalAnalysisFailed);
    }
    return Core_Failure;
  }
  if (dx_ddl_parser_next(p)) {
    return Core_Failure;
  }
  while (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_right_curly_bracket)) {
    if (Core_getError()) {
      return Core_Failure;
    }
    // name
    if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_name)) {
      return Core_Failure;
    }
    Core_String* name = NULL;
    if (Core_String_create(&name, p->scanner->text.elements, p->scanner->text.size)) {
      return Core_Failure;
    }
    if (dx_ddl_parser_next(p)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    // ':'
    if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_colon)) {
      if (!Core_getError()) {
        Core_setError(Core_Error_SyntacticalAnalysisFailed);
      }
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    if (dx_ddl_parser_next(p)) {
      DX_UNREFERENCE(name);
      name = NULL;
      return 1;
    }
    // value
    dx_ddl_node* value_node = dx_ddl_parser_on_value(p);
    if (!value_node) {
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    // put (name, value) into the map
    if (dx_ddl_node_map_set(map_node, name, value_node)) {
      DX_UNREFERENCE(value_node);
      value_node = NULL;
      DX_UNREFERENCE(name);
      name = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(value_node);
    value_node = NULL;
    DX_UNREFERENCE(name);
    name = NULL;
    // if no comma follows, break the loop
    if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_comma)) {
      if (Core_getError()) {
        return Core_Failure;
      }
      break;
    }
    if (dx_ddl_parser_next(p)) {
      return Core_Failure;
    }
  }
  if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_right_curly_bracket)) {
    dx_log("syntactical error: received ", sizeof("syntactical error: received ") - 1);
    dx_data_definition_language_word_kind word_kind;
    if (dx_ddl_parser_get_word_kind(&word_kind, p)) {
      return Core_Failure;
    }
    Core_String* s;
    if (dx_data_definition_language_word_kind_to_string(&s, word_kind)) {
      return Core_Failure;
    }
    Core_Size number_of_bytes;
    if (Core_String_getNumberOfBytes(&number_of_bytes, s)) {
      DX_UNREFERENCE(s);
      s = NULL;
      return Core_Failure;
    }
    char const* bytes = NULL;
    if (Core_String_getBytes(&bytes, s)) {
      DX_UNREFERENCE(s);
      s = NULL;
      return Core_Failure;
    }
    dx_log(bytes, number_of_bytes);
    DX_UNREFERENCE(s);
    s = NULL;

    dx_log(", expected ", sizeof(", expected ") - 1);
    if (dx_data_definition_language_word_kind_to_string(&s, dx_data_definition_language_word_kind_right_curly_bracket)) {
      return Core_Failure;
    }
    {
      Core_Size number_of_bytes = 0;
      if (Core_String_getNumberOfBytes(&number_of_bytes, s)) {
        return Core_Failure;
      }
      void const* bytes = NULL;
      if (Core_String_getBytes(&bytes, s)) {
        return Core_Failure;
      }
      dx_log(bytes, number_of_bytes);
    }
    DX_UNREFERENCE(s);
    s = NULL;

    return Core_Failure;
  }
  if (dx_ddl_parser_next(p)) {
    return Core_Failure;
  }
  return Core_Success;
}

static dx_ddl_node* dx_ddl_parser_on_map(dx_ddl_parser* p) {
  dx_ddl_node* node = NULL;
  if (dx_ddl_node_create(&node, dx_ddl_node_kind_map)) {
    return NULL;
  }
  if (dx_ddl_parser_on_map_0(p, node)) {
    DX_UNREFERENCE(node);
    node = NULL;
    return NULL;
  }
  return node;
}

static int dx_ddl_parser_on_list_0(dx_ddl_parser* p, dx_ddl_node* list_node) {
  if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_left_square_bracket)) {
    if (!Core_getError()) {
      Core_setError(Core_Error_SyntacticalAnalysisFailed);
    }
    return Core_Failure;
  }
  if (dx_ddl_parser_next(p)) {
    return Core_Failure;
  }
  while (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_right_square_bracket)) {
    // value
    dx_ddl_node* value_node = dx_ddl_parser_on_value(p);
    if (!value_node) {
      return Core_Failure;
    }
    if (dx_ddl_node_list_append(list_node, value_node)) {
      DX_UNREFERENCE(value_node);
      value_node = NULL;
      return Core_Failure;
    }
    DX_UNREFERENCE(value_node);
    value_node = NULL;
    // if no comma follows, break the loop
    if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_comma)) {
      if (Core_getError()) {
        return Core_Failure;
      }
      break;
    }
    if (dx_ddl_parser_next(p)) {
      return Core_Failure;
    }
  }
  if (!dx_ddl_parser_is_word_kind(p, dx_data_definition_language_word_kind_right_square_bracket)) {
    Core_String* s;

    dx_log("syntactical error: received ", sizeof("syntactical error: received ") - 1);

    dx_data_definition_language_word_kind word_kind;
    if (dx_ddl_parser_get_word_kind(&word_kind, p)) {
      return Core_Failure;
    }
    if (dx_data_definition_language_word_kind_to_string(&s, word_kind)) {
      return Core_Failure;
    }
    {
      Core_Size number_of_bytes = 0;
      if (Core_String_getNumberOfBytes(&number_of_bytes, s)) {
        return Core_Failure;
      }
      void const* bytes = NULL;
      if (Core_String_getBytes(&bytes, s)) {
        return Core_Failure;
      }
      dx_log(bytes, number_of_bytes);
    }
    DX_UNREFERENCE(s);
    s = NULL;

    dx_log(", expected ", sizeof(", expected ") - 1);
    if (dx_data_definition_language_word_kind_to_string(&s, dx_data_definition_language_word_kind_right_square_bracket)) {
      return Core_Failure;
    }
    {
      Core_Size number_of_bytes = 0;
      if (Core_String_getNumberOfBytes(&number_of_bytes, s)) {
        return Core_Failure;
      }
      void const* bytes = NULL;
      if (Core_String_getBytes(&bytes, s)) {
        return Core_Failure;
      }
      dx_log(bytes, number_of_bytes);
    }
    DX_UNREFERENCE(s);
    s = NULL;

    return Core_Failure;
  }
  if (dx_ddl_parser_next(p)) {
    return Core_Failure;
  }
  return Core_Success;
}

static dx_ddl_node* dx_ddl_parser_on_list(dx_ddl_parser* p) {
  dx_ddl_node* node = NULL;
  if (dx_ddl_node_create(&node, dx_ddl_node_kind_list)) {
    return NULL;
  }
  if (dx_ddl_parser_on_list_0(p, node)) {
    DX_UNREFERENCE(node);
    node = NULL;
    return NULL;
  }
  return node;
}

static Core_Result dx_ddl_parser_next(dx_ddl_parser* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_scanner_step(DX_SCANNER(SELF->scanner));
}

static void dx_ddl_parser_destruct(dx_ddl_parser* SELF) {
  DX_UNREFERENCE(SELF->diagnostics);
  SELF->diagnostics = NULL;
  DX_UNREFERENCE(SELF->scanner);
  SELF->scanner = NULL;
}

static void dx_ddl_parser_constructDispatch(dx_ddl_parser_dispatch* SELF)
{/*Intentionally empty.*/}

Core_Result dx_ddl_parser_construct(dx_ddl_parser* SELF, dx_data_definition_language_scanner* scanner, dx_data_definition_language_diagnostics* diagnostics) {
  DX_CONSTRUCT_PREFIX(dx_ddl_parser);
  if (!scanner || !diagnostics) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  SELF->scanner = scanner;
  DX_REFERENCE(SELF->scanner);
  SELF->diagnostics = diagnostics;
  DX_REFERENCE(SELF->diagnostics);
  CORE_OBJECT(SELF)->type = TYPE;
  return Core_Success;
}

Core_Result dx_ddl_parser_create(dx_ddl_parser** RETURN, dx_data_definition_language_scanner* scanner, dx_data_definition_language_diagnostics* diagnostics) {
  DX_CREATE_PREFIX(dx_ddl_parser);
  if (dx_ddl_parser_construct(SELF, scanner, diagnostics)) {
    DX_UNREFERENCE(SELF);
    SELF = NULL;
    return Core_Failure;
  }
  *RETURN = SELF;
  return Core_Success;
}

Core_Result dx_ddl_parser_set(dx_ddl_parser* SELF, char const* p, Core_Size l) {
  return dx_scanner_set(DX_SCANNER(SELF->scanner), p, l);
}

Core_Result dx_ddl_parser_get_word_kind(dx_data_definition_language_word_kind* RETURN, dx_ddl_parser const* SELF) {
  if (!SELF) {
    Core_setError(Core_Error_ArgumentInvalid);
    return Core_Failure;
  }
  return dx_data_definition_language_scanner_get_word_kind(RETURN, SELF->scanner);
}

Core_Result dx_ddl_parser_run(dx_ddl_node** RETURN, dx_ddl_parser* SELF) {
  // <start of input>
  if (!dx_ddl_parser_is_word_kind(SELF, dx_data_definition_language_word_kind_start_of_input)) {
    return Core_Failure;
  }
  if (dx_ddl_parser_next(SELF)) {
    return Core_Failure;
  }
  dx_ddl_node* root_node = dx_ddl_parser_on_value(SELF);
  if (!root_node) {
    return Core_Failure;
  }
  // <end of input>
  if (!dx_ddl_parser_is_word_kind(SELF, dx_data_definition_language_word_kind_end_of_input)) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
    return Core_Failure;
  }
  *RETURN = root_node;
  return Core_Success;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#if defined(DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS) && DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS

static dx_ddl_parser* dx_ddl_parser_test_create_parser() {
  dx_data_definition_language_diagnostics* diagnostics = NULL;
  if (dx_data_definition_language_diagnostics_create(&diagnostics)) {
    return NULL;
  }
  dx_data_definition_language_scanner* scanner = NULL;
  if (dx_data_definition_language_scanner_create(&scanner, diagnostics)) {
    DX_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  dx_ddl_parser* parser = NULL;
  if (dx_ddl_parser_create(&parser, scanner, diagnostics)) {
    DX_UNREFERENCE(scanner);
    scanner = NULL;
    DX_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  DX_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  return parser;
}

static Core_Result dx_ddl_parser_test1() {
  dx_ddl_parser* parser = dx_ddl_parser_test_create_parser();
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return Core_getError() ? Core_Failure : Core_Success;
}

static Core_Result dx_ddl_parser_test2() {
  static const char* input =
    "{\n"
    "  type : 'ColorRGBU8',\n"
    "  r : 255,\n"
    "  g : 0,\n"
    "  b : 0,\n"
    "}\n"
    ;
  dx_ddl_parser* parser = NULL;
  dx_ddl_node* root_node = NULL;
  
  parser = dx_ddl_parser_test_create_parser();
  if (!parser) {
    goto END;
  }
  if (dx_ddl_parser_set(parser, input, strlen(input))) {
    goto END;
  }
  if (dx_ddl_parser_run(&root_node, parser)) {
    goto END;
  }
END:
  if (root_node) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return Core_getError() ? Core_Failure : Core_Success;
}

static Core_Result dx_ddl_parser_test3() {
  static const char* input =
    "{\n"
    "  type : 'CheckerboardBrush',"
    "  numberOfCheckers : {\n"
    "    horizontal: 16,\n"
    "    vertical: 16,\n"
    "  },\n"
    "  checkerSize: {\n"
    "    horizontal: 128,"
    "    vertical: 128,"
    "  },\n"
    "  checkerColors: {\n"
    "    first: 'malachite',"
    "    second: 'amber',"
    "  },\n"
    "}\n"
    ;
  dx_ddl_parser* parser = NULL;
  dx_ddl_node* root_node = NULL;

  parser = dx_ddl_parser_test_create_parser();
  if (!parser) {
    goto END;
  }
  if (dx_ddl_parser_set(parser, input, strlen(input))) {
    goto END;
  }
  if (dx_ddl_parser_run(&root_node, parser)) {
    goto END;
  }
END:
  if (root_node) {
    DX_UNREFERENCE(root_node);
    root_node = NULL;
  }
  if (parser) {
    DX_UNREFERENCE(parser);
    parser = NULL;
  }
  return Core_getError() ? Core_Failure : Core_Success;
}

Core_Result dx_ddl_parser_tests() {
  if (dx_ddl_parser_test1()) {
    return Core_Failure;
  }
  if (dx_ddl_parser_test2()) {
    return Core_Failure;
  }
  if (dx_ddl_parser_test3()) {
    return Core_Failure;
  }
  return Core_Success;
}

#endif // DX_DATA_DEFINITION_LANGUAGE_PARSER_WITH_TESTS
