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
  
  /// @brief Get the stack of this interpreter.
  /// @return The stack of this interpreter.
  public function getStack() : TlStack {
    return $this->stack;
  }
  
  function __construct(TlParser $parser) {
    $this->isInParagraph = false;
    $this->parser = $parser;
    $this->stack = new TlStack();
    $this->functions = array();
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

  /// @brief Register a function.
  /// @param $name [string] The name of the function.
  /// @param $function A function of the signature (TlInterpreter, TlAst)
  /// @throw Exception a function of the name is already registered.
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
