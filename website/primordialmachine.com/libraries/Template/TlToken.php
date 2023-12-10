<?php

require_once(__DIR__ . '/TlTokenType.php');

/* A token. */
class TlToken {
  
  /// The type of this token.
  private TlTokenType $type;

  /// The text of this token.
  private string $text;

  /// @param $type The type of this token.
  /// @param $text The text of this token.
  function __construct(TlTokenType $type, string $text) {
    $this->type = $type;
    $this->text = $text;
  }

  /// Get the type of this token.
  /// @return The type of this token.
  public function getTokenType() {
    return $this->type;
  }

  /// Get the text of this token.
  /// @return The text of this token.
  public function getTokenText() {
    return $this->text;
  }
  
}; // class TlToken

?>
