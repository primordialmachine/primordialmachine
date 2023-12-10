<?php

/* Enumeration of AST types. */
enum TlAstType {
  // An input.
  case Input;
  
  // call -> name, argumentList
  case CodeCallExpression;
  
  case CodeEmptyExpression;
  
  // A name literal.
  case CodeName;
  
  // (Code) An argument list.
  // Has zero or more TlAstType::Argument nodes as children.
  case CodeArgumentList;
  
  // (Code) An argument.
  // Has an expression node as child.
  case CodeArgument;
  
  // (Code) A string literal.
  // Is an expression node.
  case CodeString;
  
  // Text that is emitted verbatim.
  case Verbatim;
  
  public function toString(): string
  {
    return match($this) {
      TlAstType::Input => 'TlAstType::Name',
      TlAstType::CodeCallExpression => 'TlAstType::CodeCallExpression',
      TlAstType::CodeEmptyExpression => 'TlAstType::CodeEmptyExpression',
      TlAstType::CodeName => 'TlAstType::CodeName',
      TlAstType::CodeArgumentList => 'TlAstType::CodeArgumentList',
      TlAstType::CodeArgument => 'TlAstType::CodeArgument',
      TlAstType::CodeString => 'TlAstType::CodeString',
      TlAstType::Verbatim => 'TlAstType::Verbatim',
    };
  }

};

?>
