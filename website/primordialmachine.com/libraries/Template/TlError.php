<?php

class TlError extends Exception {
  public function __construct(string $message = '', int $code = 422) {
    parent::__construct($message, $code);
    $this->message = "$message";
    $this->code = $code;
  }
};

?>
