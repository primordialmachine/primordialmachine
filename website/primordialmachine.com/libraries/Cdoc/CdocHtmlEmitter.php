<?php

$dir = __DIR__;
if (str_ends_with($dir, DIRECTORY_SEPARATOR)) {
  require_once($dir . './../App.php');
  require_once($dir . 'CdocFile.php');
  require_once($dir . 'CdocEmitter.php');
  require_once($dir . 'CdocIndexManager.php');
} else {
  require_once($dir . '/./../App.php');
  require_once($dir . '/CdocFile.php');
  require_once($dir . '/CdocEmitter.php');
  require_once($dir . '/CdocIndexManager.php');
}

/**
 * A "C Documentation" emitter emitting HTML code.
 */
class CdocHtmlEmitter extends CdocEmitter {

  /**
   * The "template language" interpreter.
   */
  private TlInterpreter $interpreter;

  /**
   * Emit a 3rd level headline ("<h3>") with
   * - the HTML text $text
   * - the HTML ID $id
   * $indexName is the name of the index to add the element to.
   */
  private function section($indexName, $id, $text) {
    $index = CdocIndexManager::getInstance()->getByName($indexName);
    $entry = $index->getEntryMap()[$text];
    echo '<h3 class="cdoc-entry" id = "' . $entry['file']->getJsonData()['id'] . '">';
    echo $entry['file']->getJsonData()['name'];
    echo '</h3>';
  }

  public function __construct() {
    $this->interpreter = new TlInterpreter(new TlParser(new TlScanner()));
  }
  
  public function getLinkInfo(CdocFile $file) {
    $url = App::getInstance()->site_url_prefix . $file->getJsonData()['indices'][0] . '#' . $file->getJsonData()['id'];
    $text = $file->getJsonData()['name'];
    return array( 'url' => $url, 'text' => $text );
  }
  
  public function emitTableOfContentsEntry($entry) {
    $linkInfo = $this->getLinkInfo($entry['file']);
    echo
      '<li>' .
      '<a href="' . $linkInfo['url'] . '">' . $linkInfo['text'] . '</a>' .
      '</li>'
      ;
  }

  public function emitContentsEntry(CdocFile $file) {
    $jsonData = $file->getJsonData();

    $interpreter = new TlInterpreter(new TlParser(new TlScanner()));

    echo '<div class="cdoc-entry">';

    if (!isset($jsonData['indices'])) {
      throw new Exception($file->getPathname() . ': error: `indices` is not not defined.');
    }

    if (is_array($jsonData['indices'])) {
      if (count($jsonData['indices']) > 1) {
        throw new Exception($file->getPathname() . ': error: multiple indices in `indices` not yet supported.');
      }
      $this->section($jsonData['indices'][0], $jsonData['id'], $jsonData['name']);
    } else if (is_string($jsonData['indices'])) {
      $this->section($jsonData['indices'], $jsonData['id'], $jsonData['name']);
    } else {
        throw new Exception($file->getPathname() . ': error: `indices` must be an array of zero or more strings or a string or undefined.');
    }

    // signature
    echo '<h4>Signature</h4>'
       . '<p><code style="overflow-wrap: anywhere">'
       ;
    echo $jsonData['signature'];
    echo '</code></p>';

    // signature remarks (optional)
    if (isset($jsonData['signatureRemarks'])) {
      $signatureRemarks = $jsonData['signatureRemarks'];
      if (is_array($signatureRemarks)) {
        foreach ($signatureRemarks as $signatureRemark) {
          echo $signatureRemark;
        }
      } else if (is_string($signatureRemarks)) {
        echo $signatureRemarks;
      } else {
        throw new Exception("signatureRemarks must be a) an array of zero or more strings, b) a string, or c) undefined");
      }
    }

    if ($jsonData['type'] == 'object') {
      if (isset($jsonData['extends'])) {
        echo '<h4>Extends</h4>';
        echo '<p>';
        $file = CdocIndexManager::getInstance()->getFileByName($jsonData['extends']);
        $linkInfo = $this->getLinkInfo($file);
        echo '<a href="' . $linkInfo['url'] . '">' . $linkInfo['text'] . '</a>';
        echo '</p>';
      }
    }

    // description
    echo '<h4>Description</h4>';
    if (is_array($jsonData['description'])) {
      echo '<p>';
      foreach ($jsonData['description'] as $x) {
        $interpreter->execute($x, $file->getPathname());
      }
      echo '</p>';
    } else {
      echo '<p>';
      $interpreter->execute($jsonData['description'], $file->getPathname());
      echo '</p>';
    }

    $clangTypes = array('function', 'function-prototype', 'structure', 'symbolic-constant', 'typedef');
    $clangExTypes = array('constructor', 'create-operator', 'enumeration', 'method', 'object');
    $types = array_merge($clangTypes, $clangExTypes);

    if (!in_array($jsonData['type'], $types)) {
      $msg = '';
      if (0 == count($types)) {
        throw new Exception("internal error");
      } else if (1 == count($types)) {
        $msg = '`' . $types[0] . '`';
      } else if (2 == count($types)) {
        $msg = '`' . $types[0] . '` or `' . $types[1] . '`';
      } else {
        $msg = '`' . $types[0] . '`';
        for ($i = 1; $i < count($types) - 1; ++$i) {
          $msg = $msg . ', `' . $types[$i] . '`';
        }
        $msg = $msg . ' or `' . $types[count($types) - 1] . '`';
      }
      throw new Exception($file->getPathname() . ": error: `type` is `" . $jsonData['type'] .
                          "`. `type` must be one of " . $msg);
    }

    // handle structures
    if ($jsonData['type'] == 'structure') {
      if (isset($jsonData['members'])) {
        $membersJson = $jsonData['members'];
        echo '<h4>Members</h4>';
        if (!is_array($membersJson)) {
          throw new Exception($file->getPathname() . ": error: `members` must be an array");
        }
        echo '<table class="structure-members-table" style="width: initial !important">';
        foreach ($membersJson as $memberJson) {
          echo '<tr>';
            echo '<td><code>';
            $interpreter->execute($memberJson['name'], $file->getPathname());
            echo '</code></td>';
            echo '<td>';
            $interpreter->execute($memberJson['type'], $file->getPathname());
            echo '</td>';
            echo '<td>';
            if (is_array($memberJson['description'])) {
              foreach ($memberJson['description'] as $x) {
                $interpreter->execute($x, $file->getPathname());
              }
            } else {
              $interpreter->execute($memberJson['description'], $file->getPathname());
            }
            echo '</td>';
          echo '</tr>';
        }
        echo '</table>';
      }
    }

    // handle enumerations
    if ($jsonData['type'] == 'enumeration') {
      if (isset($jsonData['elements'])) {
        $elementsJson = $jsonData['elements'];
        echo '<h4>Elements</h4>';
        if (!is_array($elementsJson)) {
          throw new Exception($file->getPathname() . ": error: `elements` must be an array");
        }
        echo '<table class="enumeration-elements-table">';
        foreach ($elementsJson as $elementJson) {
          echo '<tr>';
          echo '<td><code>';
          $interpreter->execute($elementJson['name'], $file->getPathname());
          echo '</code></td>';
          echo '<td>';
          if (is_array($elementJson['description'])) {
            foreach ($elementJson['description'] as $x) {
              $interpreter->execute($x, $file->getPathname());
            }
          } else {
            $interpreter->execute($elementJson['description'], $file->getPathname());
          }
          echo '</td>';
          echo '</tr>';
        }
        echo '</table>';
      }
    }

    // handle callables
    if ($jsonData['type'] == 'create-operator' || $jsonData['type'] == 'constructor' || $jsonData['type'] == 'function' || $jsonData['type'] == 'function-prototype'|| $jsonData['type'] == 'method') {
      if (isset($jsonData['parameters'])) {
        $parametersJson = $jsonData['parameters'];
        echo '<h4>Parameters</h4>';
        if (!is_array($parametersJson)) {
          throw new Exception($file->getPathname() . ": error: `parameters` must be an array");
        }
        echo '<table class="parameters-table">';
        foreach ($parametersJson as $parameterJson) {
          echo '<tr>';
          echo '<td><code>';
          $interpreter->execute($parameterJson['name'], $file->getPathname());
          echo '</code></td>';
          echo '<td>';
          $interpreter->execute($parameterJson['type'], $file->getPathname());
          echo '</td>';
          echo '<td>';
          $interpreter->execute($parameterJson['description'], $file->getPathname());
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
            $interpreter->execute($element, $file->getPathname());
            echo '</p>';
          }
        } else {
          echo '<p>';
          $interpreter->execute($jsonData['success'], $file->getPathname());
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
          $interpreter->execute($errorJson['errorCode'], $file->getPathname());
          echo '</code></td>';
          echo '<td style="width: 100%">';
          $interpreter->execute($errorJson['errorCondition'], $file->getPathname());
          echo '</td>';
          echo '</tr>';
        }
        echo '</table>';
      }

      // return
      if (isset($jsonData['return'])) {
        $values = $jsonData['return'];
        echo '<h4>Return</h4>';
        if (is_array($values)) {
          foreach ($values as $value) {
            echo '<p>';
            $interpreter->execute($value, $file->getPathname());
            echo '</p>';
          }
        } else {
          echo '<p>';
          $interpreter->execute($jsonData['return'], $file->getPathname());
          echo '</p>';
        }
      }
    }

    // remarks
    if (isset($jsonData['remarks'])) {
      $remarksJson = $jsonData['remarks'];
      echo '<h4>Remarks</h4>';
      if (is_array($remarksJson)) {
        echo '<p>';
        foreach ($remarksJson as $remarkJson) {
          $interpreter->execute($remarkJson, $file->getPathname());
        }
        echo '</p>';
      } else {
        echo '<p>';
        $interpreter->execute($remarksJson, $file->getPathname());
        echo '</p>';
      }
    }

    echo '</div>';
  }
}; // class CdocHtmlEmitter

?>
