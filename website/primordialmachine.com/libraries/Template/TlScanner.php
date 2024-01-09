<?php
class TlScanner {
  private $s;
  private $n;
  private $i;
  private $tokens = array();
  private $buffer;
  private $inputName = '<empty>';

  function __construct()
  {}

  /// Get (a copy) of the list of tokens.
  public function getTokens() {
    return $this->tokens;
  }

  private function isAlphabetic() {
    if ($this->i == $this->n) {
      return false;
    }
    $x = $this->s[$this->i];
    return ('A' <= $x && $x <= 'Z')
        || ('a' <= $x && $x <= 'z');
  }

  private function isUnderscore() {
    if ($this->i == $this->n) {
      return false;
    }
    $x = $this->s[$this->i];
    return '_' == $x;
  }

  private function isNumeric() {
    if ($this->i == $this->n) {
      return false;
    }
    $x = $this->s[$this->i];
    return ('0' <= $x && $x <= '9');
  }

  private function onString($quote) {
    $this->buffer = '';
    if (!$this->IS($quote)) {
      $this->error("<string opening delimiter>");
    }
    $this->i++;
    while ($this->i < $this->n) {
      if ($this->IS($quote)) {
        break;
      } else if ($this->IS('\\')) {
        $this->i++;
        if ($this->IS($quote)) {
          $this->buffer .= $quote;
          $this->i++;
        } else if ($this->IS('\\')) {
          $this->buffer .= '\\';
          $this->i++;
        } else {
          $this->error("\\");
        }
      } else {
        $this->buffer .= $this->s[$this->i];
        $this->i++;
      }
    }
    if (!$this->IS($quote)) {
      $this->error("<string closing delimiter>");
    }
    $this->i++;
    $this->tokens[] = new TlToken(TlTokenType::CodeString, $this->buffer);
    $this->buffer = '';
  }

  // MACRO function.
  private function IS($symbol) {
    return $this->i < $this->n && $symbol == $this->s[$this->i];
  }

  private function error(string|array $expected) {
    $received = $this->i == $this->n ? "<end of input>" : $this->s[$this->i];
    if (is_array($expected)) {
      if (count($expected) == 0) {
        throw new Exception("expected `" . $expected . "`");
      } else if (count($expected) == 1) {
        throw new Exception("expected `" . $expected[0] . "`, received `" . $received . "`");
      } else if (count($expected) == 2) {
        throw new Exception("expected `" . $expected[0] . "` or `" . $expected[1] . "`, received `" . $received . "`");
      } else {
        $i = 0;
        $msg = "expected `" . $expected[$i] . "`"; 
        $i++;
        while($i < count($expected) - 1) {
          $msg .= ", `" . $expected[$i] . "`";
          $i++;
        }
        $msg .= ", or `" . $expected[$i] . "`, received `" . $received . "`";
        throw new Exception($msg);
      }
    } else {
      throw new Exception($this->inputName . ": expected `" . $expected . "`, received `" . $received . "`");
    }
  }

  private function onName() {
    $this->buffer = '';
    if (!$this->isAlphabetic()) {
      $this->error("<alphabetic>");
    }
    do {
      $this->buffer .= $this->s[$this->i];
      $this->i++;
    } while ($this->isAlphabetic() || $this->isNumeric() || $this->isUnderscore());
    $this->tokens[] = new TlToken(TlTokenType::CodeName, $this->buffer);
    $this->buffer = '';
  }

  // a block of the form
  // @{ <block> }
  private function onCodeBlock() {
    $balance = 0;
    $balance++;

    while ($this->i < $this->n) {
      if ($this->IS('}')) {
        $balance--;
        if ($balance == 0) {
          $this->tokens[] = new TlToken(TlTokenType::ClosingCodeBlockDelimiter, '}');
          $this->i++;
          break;
        } else {
          $this->tokens[] = new TlToken(TlTokenType::RightCurlyBracket, '}');
          $this->i++;
          continue; // start from beginning
        }
      }
      if ($this->IS(' ') || $this->IS('\t')) {
        do {
          // skip ws
          $this->i++;
        } while ($this->IS(' ') || $this->IS('\t'));
        continue; // start from beginning
      }
      switch ($this->s[$this->i]) {
        case '(': {
          $this->tokens[] = new TlToken(TlTokenType::CodeLeftParenthesis, '(');
          $this->i++;
        } break;
        case ')': {
          $this->tokens[] = new TlToken(TlTokenType::CodeRightParenthesis, ')');
          $this->i++;
        } break;
        case '\'': {
          $this->onString('\'');
        } break;
        case '"': {
          $this->onString('"');
        } break;
        default: {
          $this->onName();
        } break;
      }
    } // while
  }

  // an expression of the form
  // @(<expression>)
  private function onCodeExpression() {
    $balance = 0;
    $balance++;

    while ($this->i < $this->n) {
      if ($this->IS(' ') || $this->IS('\t')) {
        do {
          // skip ws
          $this->i++;
        } while ($this->IS(' ') || $this->IS('\t'));
        continue; // start from beginning
      }
      switch ($this->s[$this->i]) {
        case '(': {
          $this->tokens[] = new TlToken(TlTokenType::CodeLeftParenthesis, '(');
          $balance++;
          $this->i++;
        } break;
        case ')': {
          $balance--;
          if ($balance == 0) {
            $this->tokens[] = new TlToken(TlTokenType::ClosingCodeExpressionDelimiter, ')');
            $this->i++;
            return;
          } else {
            $this->tokens[] = new TlToken(TlTokenType::CodeRightParenthesis, ')');
            $this->i++;
          }
        } break;
        case '\'': {
          $this->onString('\'');
        } break;
        case '"': {
          $this->onString('"');
        } break;
        default: {
          $this->onName();
        } break;
      }
    } // while
  }

  private function execute1() {
    $this->buffer = '';
    while ($this->i < $this->n) {
      if ($this->IS('@')) {
        // flush the output
        if (strlen($this->buffer) > 0) {
          $this->tokens[] = new TlToken(TlTokenType::Verbatim, $this->buffer);
          $this->buffer = '';
        }
        $this->i++;
        if ($this->i == $this->n) {
          $this->error(array('@', '{', '('));
        }
        if ($this->IS('@')) {
          // @@
          $this->tokens[] = new TlToken(TlTokenType::Verbatim, '@');
          $this->i++;
        } else if ($this->IS('{')) {
          // @{<block>}
          $this->tokens[] = new TlToken(TlTokenType::OpeningCodeBlockDelimiter, '@{');
          $this->i++;
          $this->onCodeBlock();
        } else if ($this->IS('(')) {
          // @(<expression>)
          $this->tokens[] = new TlToken(TlTokenType::OpeningCodeExpressionDelimiter, '@(');
          $this->i++;
          $this->onCodeExpression();
        } else {
          $this->error(array('@', '{', '('));
        }
      } else {
        $this->buffer .= $this->s[$this->i];
        $this->i++;
      }
    }
    if (strlen($this->buffer) > 0) {
      $this->tokens[] = new TlToken(TlTokenType::Verbatim, $this->buffer);
      $this->buffer = '';
    }
  }

  function execute($input, $inputName) {
    $this->inputName = $inputName;
    $this->s = $input;
    $this->i = 0;
    $this->n = strlen($this->s);
    $this->tokens = [];
    $this->tokens[] = new TlToken(TlTokenType::InputStart, '<start of input>');
    $this->execute1();
    $this->tokens[] = new TlToken(TlTokenType::InputEnd, '<end of input>');
  }
  
  function getInputName() : string {
    return $this->inputName;
  }
};

?>
