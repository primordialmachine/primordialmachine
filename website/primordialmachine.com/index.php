<?php
$requestUri = $_SERVER['REQUEST_URI'];
$queryString = $_SERVER['QUERY_STRING'];
if (str_starts_with($queryString, 'core')) {
  require_once(__DIR__ . '/core/index.php');
  exit;
} else if (str_starts_with($queryString, 'data-definition-language/specification')) {
  require_once(__DIR__ . '/data-definition-language/specification/index.php');
  exit;
} else if (str_starts_with($queryString, 'asset-definition-language/specification')) {
  require_once(__DIR__ . '/asset-definition-language/specification/index.php');
  exit;
} else if (str_starts_with($queryString, 'val')) {
  require_once(__DIR__ . '/val/index.php');
  exit;
} else if (str_starts_with($queryString, 'aal')) {
  require_once(__DIR__ . '/aal/index.php');
  exit;
} else {
  require_once(__DIR__ . '/home/index.php');
  exit;
}
?>
