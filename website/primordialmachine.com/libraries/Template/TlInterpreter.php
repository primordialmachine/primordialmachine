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
  
  private array $functions;
  
  function __construct(TlParser $parser) {
    $this->isInParagraph = false;
    $this->parser = $parser;
    $this->stack = new TlStack();
    $this->functions = array();
    
    $this->functions['newline'] = function (TlInterpreter $interpreter, TlAst $ast) {
      if ($this->stack->size() < 1) {
        throw new Exception('stack corruption');
      }
      $nargs = $this->stack->pop();
      if (!is_int($nargs)) {
        throw new Exception('stack corruption');
      }
      if ($nargs != 0) {
        throw new Exception('invalid number of arguments');
      }
      echo '</br>';
    }; 
    
    $this->functions['code'] = function (TlInterpreter $interpreter, TlAst $ast) {
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

    $this->functions['meta'] = function (TlInterpreter $interpreter, TlAst $ast) {
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
    TlAstType::CodeCallExpression == $ast->getAstType() or die("invalid argument");
    $this->onCodeArgumentList($ast->getChildren()[1]);
    $nameAst = $ast->getChildren()[0];
    $nameAst->assertAstType(TlAstType::CodeName);
    //$this->onArgumentList($ast->getChildren()[1]);
    // [arg[1],...,arg[n],n]
    if (!isset($this->functions[$nameAst->getAstValue()])) {
      throw new Exception("unkown function: " . $nameAst->getAstValue());
    }
    $function = $this->functions[$nameAst->getAstValue()];
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

  public function registerFunction(string $name, $function) {
    if (isset($this->functions[$name])) {
      throw new Exception('a function of name `' . $name . '` is already registered');
    }
    $this->functions[$name] = $function;
  }

  public function execute(string $input, string $inputName) {
    $ast = $this->parser->execute($input, $inputName);
    $this->onInput($ast);
  }
}; // class TlInterpreter

?>
