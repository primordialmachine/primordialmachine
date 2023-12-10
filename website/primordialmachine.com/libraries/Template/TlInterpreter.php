<?php

require_once(__DIR__ . '/TlParser.php');

class TlStack {
  private $stack = array();

  // push
  public function push($value) {
    $this->stack[] = $value;
  }

  // pop & return last element
  // exception if stack is empty
  public function pop() {
    if (!count($this->stack)) {
      throw new Exception('stack underflow');
    }
    return array_pop($this->stack);
  }

  // get the size
  public function size() {
    return count($this->stack);
  }
};

class TlInterpreter {

  private TlStack $stack;

  private TlParser $parser;

  function __construct(TlParser $parser) {
    $this->parser = $parser;
    $this->stack = new TlStack();
  }

  public function onCodeArgumentList(TlAst $ast) {
    $ast->assertAstType(TlAstType::CodeArgumentList);
    foreach ($ast->getChildren() as $child) {
      $child->assertAstType(TlAstType::CodeArgument);
      $child->getChildren()[0]->assertAstType(TlAstType::CodeString);
      $stringValue = $child->getChildren()[0]->getAstValue();
      $this->stack->push($stringValue);
    }
    $this->stack->push(count($ast->getChildren()));
  }

  protected function onCodeCallExpression(TlAst $ast) {
    $functions = array();

    $functions['code'] = function (TlInterpreter $interpreter, TlAst $ast) {
      if ($this->stack->size() < 1) {
        throw new Exception('stack corruption');
      }
      $nargs = $this->stack->pop();
      if (!is_int($nargs)) {
        throw new Exception('stack corruption');
      }
      if ($nargs != 1) {
        throw new Exception('invalid number of arguments');
      }
      $arg1 = $this->stack->pop();
      echo '<code>' . $arg1 . '</code>';
    };

    $functions['meta'] = function (TlInterpreter $interpreter, TlAst $ast) {
      if ($this->stack->size() < 1) {
        throw new Exception('stack corruption');
      }
      $nargs = $this->stack->pop();
      if (!is_int($nargs)) {
        throw new Exception('stack corruption');
      }
      if ($nargs != 1) {
        throw new Exception('invalid number of arguments');
      }
      $arg1 = $this->stack->pop();
      echo '<code>' . $arg1 . '</code>';
    };

    TlAstType::CodeCallExpression == $ast->getAstType() or die("invalid argument");
    $this->onCodeArgumentList($ast->getChildren()[1]);
    $nameAst = $ast->getChildren()[0];
    $nameAst->assertAstType(TlAstType::CodeName);
    //$this->onArgumentList($ast->getChildren()[1]);
    // [arg[1],...,arg[n],n]
    if (!isset($functions[$nameAst->getAstValue()])) {
      throw new Exception("unkown function: " . $nameAst->getAstValue());
    }
    $function = $functions[$nameAst->getAstValue()];
    $function($this, $ast);
  }

  protected function onInput(TlAst $ast) {
    TlAstType::Input == $ast->getAstType() or die("invalid argument");
    foreach ($ast->getChildren() as $childAst) {
      switch ($childAst->getAstType()) {
        case TlAstType::Verbatim:
          echo $childAst->getAstValue();
          break;
        case TlAstType::CodeCallExpression:
          $this->onCodeCallExpression($childAst);
          break;
        default:
          //die("unknown/unsupported AST type");
      };
    }
  }

  public function execute(string $input) {
    $ast = $this->parser->execute($input);
    $this->onInput($ast);
  }
}; // class TlInterpreter

?>
