<?php
require_once(__DIR__ . '/TlAstType.php');

class TlAst {

  private ?string $value;

  private TlAstType $type;
  
  private array $children = array();

  function __construct(TlAstType $type, ?string $value) {
    $this->type = $type;
    $this->value = $value;
  }
  
  public function append(TlAst $child) {
    $this->children[] = $child;
  }
  
  public function getChildren() {
    return $this->children;
  }

  public function getAstType() {
    return $this->type;
  }

  public function getAstValue() {
    return $this->value;
  }
  
  public function assertAstType(TlAstType $type) {
    $type == $this->getAstType() or throw new Exception("unexpected AST type `" . $this->getAstType()->toString() . "` received, expected AST type`" . $type->toString() . "`"); 
  }
  
}; // class TlAst

?>
