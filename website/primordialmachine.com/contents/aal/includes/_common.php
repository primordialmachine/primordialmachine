<?php
$ROOT_DIR = $_SERVER['DOCUMENT_ROOT'];
require_once($ROOT_DIR . '/libraries/Template/Include.php');
require_once($ROOT_DIR . '/libraries/Index/Include.php');

/**
 * create a 3rd level headline with
 * - the text $text
 * - the ID $id
 */
function section($id, $text) {
  $index = &IndexManager::getInstance()->getByName('core-ring-1');
  $indexEntry = array('text' => $text, 'id' => $id, 'href' => App::getInstance()->site_url_prefix . 'core' . '#' . $id);
  $index[] = $indexEntry;
  echo '<h3 id = "' . $indexEntry['id'] . '">';
  echo $indexEntry['text'];
  echo '</h3>';
}

/**
 * @brief Generate the HTML code from a documentation file.
 * @param string $pathname The pathname of the documentation file.
 * @remarks
 * A documentation file is a JSON file with particular contents.
 * The format is currently not specified.
 * For lots of references how to write them see the php and json file pairs in this directory (e.g., sin.php and sin.json).
 */
function TlRun(TlContentsFile $contentsFile) {
  $jsonData = $contentsFile->getJsonData();

  $interpreter = new TlInterpreter(new TlParser(new TlScanner()));
  
  section($jsonData['id'], $jsonData['name']);
  
  // signature
  echo '<h4>Signature</h4>'
     . '<p><code>'
     ;
  echo $jsonData['signature'];
  echo '</code></p>';
  
  // signature remarks (optional)
  if (isset($jsonData['signatureRemarks'])) {
    $signatureRemarks = $jsonData['signatureRemarks'];
    foreach ($signatureRemarks as $signatureRemark) {
      echo $signatureRemark;
    }
  }

  // description
  echo '<h4>Description</h4>'
     . '<p>'
     ;
  $interpreter->execute($jsonData['description']);
  echo '</p>';
  
  if (isset($jsonData['parameters'])) {
    $parametersJson = $jsonData['parameters'];
    echo '<h4>Parameters</h4>';
    if (!is_array($parametersJson)) {
      throw new Exception('`parameters` must be an array');
    }
    echo '<table style="width: initial !important">';
    foreach ($parametersJson as $parameterJson) {
      echo '<tr>';
      echo '<td><code>';
      $interpreter->execute($parameterJson['name']);
      echo '</code></td>';
      echo '<td>';
      $interpreter->execute($parameterJson['type']);
      echo '</td>';
      echo '<td>';
      $interpreter->execute($parameterJson['description']);
      echo '</td>';
      echo '</tr>';
    }
    echo '</table>';
  }
  
  // success
  if (isset($jsonData['success'])) {
    $successJson = $jsonData['success'];
    echo '<h4>Success</h4>';
    if (is_array($successJson)) {
      foreach ($successJson as $element) {
        echo '<p>';
        $interpreter->execute($element);
        echo '</p>';
      }
    } else {
      echo '<p>';
      $interpreter->execute($jsonData['success']);
      echo '</p>';
    }
  }

  // errors
  if (isset($jsonData['errors'])) {
    echo '<h4>Errors</h4>';
    echo '<table style="width: initial !important">';
    $errorsJson = $jsonData['errors'];
    foreach ($errorsJson as $errorJson) {
      echo '<tr>';
      echo '<td><code>';
      $interpreter->execute($errorJson['errorCode']);
      echo '</code></td>';
      echo '<td>';
      $interpreter->execute($errorJson['errorCondition']);
      echo '</td>';
      echo '</tr>';
    }
    echo '</table>';
  }

  // remarks
  if (isset($jsonData['remarks'])) {
    $remarksJson = $jsonData['remarks'];
    echo '<h4>Remarks</h4>';
    if (is_array($remarksJson)) {
      foreach ($remarksJson as $remarkJson) {
        echo '<p>';
        $interpreter->execute($remarkJson);
        echo '</p>';
      }
    } else {
      echo '<p>';
      $interpreter->execute($remarksJson);
      echo '</p>';
    }
  }
}

?>
