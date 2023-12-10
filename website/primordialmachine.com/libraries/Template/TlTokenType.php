<?php
/* Enumeration of token types. */
enum TlTokenType {

  case InputStart;

  case InputEnd;

  case Verbatim;

  /// `@{`
  case OpeningCodeBlockDelimiter;
 
  /// `}`
  case ClosingCodeBlockDelimiter;
  
  /// `@(`
  case OpeningCodeExpressionDelimiter;
  
  /// `)`
  case ClosingCodeExpressionDelimiter;

  case CodeName;

  case CodeString;

  case CodeLeftParenthesis;

  case CodeRightParenthesis;

  case CodeArgumentSeparator;

  public function toString() {
    return match($this) {
        TlTokenType::InputStart => '<start of input>',
        TlTokenType::InputEnd => '<end of input>',
        TlTokenType::Verbatim => '<verbatim>',
        TlTokenType::OpeningCodeBlockDelimiter => '<opening code block delimiter>',
        TlTokenType::ClosingCodeBlockDelimiter => '<closing code block delimiter>',
        TlTokenType::OpeningCodeExpressionDelimiter => '<opening code expression delimiter>',
        TlTokenType::ClosingCodeExpressionDelimiter => '<closing code expression delimiter>',
        TlTokenType::CodeName => '<code : name>',
        TlTokenType::CodeString => '<code : string>',
        TlTokenType::CodeLeftParenthesis => '<code : left parenthesis>',
        TlTokenType::CodeRightParenthesis => '<code : right parenthesis>',
        TlTokenType::CodeArgumentSeparator => '<code : argument separator>',
    };
  }
};

?>
