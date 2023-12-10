<?php
require_once(__DIR__ . '/TlAst.php');

class TlParser {

  private TlScanner $scanner;
  private ?array $tokens;
  private int $i;
  private int $n;

  function __construct(TlScanner $scanner) {
    $this->scanner = $scanner;
    $this->i = 0;
    $this->n = 0;
    $this->tokens = null;
  }
  
  private function IS(TlTokenType $tokenType) {
    return $tokenType == $this->tokens[$this->i]->getTokenType();
  }
  
  private function code_onArgument() {
    if ($this->IS(TlTokenType::CodeString)) {
      $argumentAst = new TlAst(TlAstType::CodeArgument, null);
      $stringAst = new TlAst(TlAstType::CodeString, $this->tokens[$this->i]->getTokenText());
      $this->i++;
      $argumentAst->append($stringAst);
      return $argumentAst;
    } else {
      throw new Exception("expected <code.argument>");
    }
  }
  
  // code.argumentList := e
  //                    | code.argument code.argumentListRest
  // code.argumentListRest := ',' code.argument code.argumentListRest
  //                        | e
  private function code_onArgumentList() {
    $argumentListAst = new TlAst(TlAstType::CodeArgumentList, null);
    if (!$this->IS(TlTokenType::CodeRightParenthesis)) {
      //while (!$this->IS(TlTokenType::CodeRightParenthesis)) {
        $argumentAst = $this->code_onArgument();
        $argumentListAst->append($argumentAst);
      //}
      while ($this->IS(TlTokenType::CodeArgumentSeparator)) {
        $this->i++;
        $argumentAst = $this->code_onArgument();
        $argumentListAst->append($argumentAst);
      }
      if (!$this->IS(TlTokenType::CodeRightParenthesis)) {
        throw new Exception("unclosed <code.argumentList>");
      }
      $this->i++;
    }
    return $argumentListAst;
  }
  
  // code.expression := code.name code.argumentList
  private function code_onExpression() {
    if (!$this->IS(TlTokenType::CodeName)) {
      throw new Exception("expected <name>");
    }
    $nameAst = new TlAst(TlAstType::CodeName, $this->tokens[$this->i]->getTokenText());
    $this->i++;
    if (!$this->IS(TlTokenType::CodeLeftParenthesis)) {
      throw new Exception("expected <left parenthesis>");
    }
    $this->i++;
    $argumentListAst = $this->code_onArgumentList();
    $callAst = new TlAst(TlAstType::CodeCallExpression, null);
    $callAst->append($nameAst);
    $callAst->append($argumentListAst);
    return $callAst;
  }

  // codeBlock := '@{' code.expression? '}'
  private function onCodeBlock() {
    $expressionAst = null;
    if (!$this->IS(TlTokenType::InputEnd)) {
      $expressionAst = $this->code_onExpression();
    }
    if (!$this->IS(TlTokenType::ClosingCodeBlockDelimiter)) {
      throw new Exception("expected " . TlTokenType::ClosingCodeBlockDelimiter->toString() . ", received " . $this->tokens[$this->i]->getTokenType()->toString());
    }
    $this->i++;
    if (!$expressionAst) {
      $expressionAst = new TlAst(TlAstType::CodeEmptyExpression, null);
    }
    return $expressionAst;
  }

  // codeExpression := '@(' code.expression? ')'
  private function onCodeExpression() {
    $expressionAst = null;
    if ($this->i < $this->n) {
      $expressionAst = $this->code_onExpression();
    }
    if ($this->i == $this->n || $this->tokens[$this->i]->getTokenType() != TlTokenType::ClosingCodeExpressionDelimiter) {
      throw new Exception("expected " . TlTokenType::ClosingCodeExpressionDelimiter->toString() . ", received " . $this->tokens[$this->i]->getTokenType()->toString());
    }
    $this->i++;
    if (!$expressionAst) {
      $expressionAst = new TlAst(TlAstType::CodeEmptyExpression, null);
    }
    return $expressionAst;
  }

  public function execute(string $input) {
    $inputAst = new TlAst(TlAstType::Input, null);

    $this->scanner->execute($input);
    $this->tokens = $this->scanner->getTokens();
    $this->i = 0;
    $this->n = count($this->tokens);
    while ($this->i < $this->n) {
      $token = $this->tokens[$this->i];     
      switch ($token->getTokenType()) {
        case TlTokenType::Verbatim:
          $ast = new TlAst(TlAstType::Verbatim, $token->getTokenText());
          $inputAst->append($ast);
          $this->i++;
          break;
        case TlTokenType::OpeningCodeBlockDelimiter:
          $this->i++;
          $ast = $this->onCodeBlock();
          $inputAst->append($ast);
          break;
        case TlTokenType::OpeningCodeExpressionDelimiter:
          $this->i++;
          $ast = $this->onCodeExpression();
          $inputAst->append($ast);
          break;
        default:
          $this->i++;
          break;
      }; // switch
    } // while
    
    return $inputAst;
  }
  
}; // class TlParser

?>
