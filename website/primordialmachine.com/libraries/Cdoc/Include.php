<?php

$dir = __DIR__;
if (str_ends_with($dir, DIRECTORY_SEPARATOR)) {
  require_once($dir . './../App.php');
  require_once($dir . 'CdocFile.php');
  require_once($dir . 'CdocEmitter.php');
} else {
  require_once($dir . '/./../App.php');
  require_once($dir . '/CdocFile.php');
  require_once($dir . '/CdocEmitter.php');
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
  
  public function emitTableOfContentsEntry($entry) {
    $linkHref = App::getInstance()->site_url_prefix . $entry['index']->getName() . '#' . $entry['file']->getJsonData()['id'];
    $linkText = $entry['file']->getJsonData()['name'];
    echo
      '<li>' .
      '<a href="' . $linkHref . '">' . $linkText . '</a>' .
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

    if ($jsonData['type'] == 'object') {
      echo '<h4>Extends</h4>';
      if (!isset($jsonData['extends'])) {
        throw new Exception('`extends` is not set');
      }
      echo '<p>';
      echo $jsonData['extends'];
      echo '</p>';
    }

    // description
    echo '<h4>Description</h4>';
    if (is_array($jsonData['description'])) {
      echo '<p>';
      foreach ($jsonData['description'] as $x) {
        $interpreter->execute($x);
      }
      echo '</p>';
    } else {
      echo '<p>';
      $interpreter->execute($jsonData['description']);
      echo '</p>';
    }


    if ($jsonData['type'] !== 'structure' &&
        $jsonData['type'] !== 'enumeration' &&
        $jsonData['type'] !== 'function' &&
        $jsonData['type'] !== 'typedef' &&
        $jsonData['type'] !== 'object' &&
        $jsonData['type'] !== 'symbolic-constant') {
      throw new Exception("`type` is `" . $jsonData['type'] . "`. `type` must be one of `enumeration`, `function`, `structure`, `typedef`, `object`, `symbolic-constant`");
    }

    // handle enumerations
    if ($jsonData['type'] == 'enumeration') {
      if (isset($jsonData['elements'])) {
        $elementsJson = $jsonData['elements'];
        echo '<h4>Elements</h4>';
        if (!is_array($elementsJson)) {
          throw new Exception('`elements` must be an array');
        }
        echo '<table style="width: initial !important">';
        foreach ($elementsJson as $elementJson) {
          echo '<tr>';
          echo '<td><code>';
          $interpreter->execute($elementJson['name']);
          echo '</code></td>';
          echo '<td style="width: 100%">';
          if (is_array($elementJson['description'])) {
            foreach ($elementJson['description'] as $x) {
              $interpreter->execute($x);
            }
          } else {
            $interpreter->execute($elementJson['description']);
          }
          echo '</td>';
          echo '</tr>';
        }
        echo '</table>';
      }
    }

    // handle functions
    if ($jsonData['type'] == 'function') {
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
          echo '<td style="width: 100%">';
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
          echo '<td style="width: 100%">';
          $interpreter->execute($errorJson['errorCondition']);
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
            $interpreter->execute($value);
            echo '</p>';
          }
        } else {
          echo '<p>';
          $interpreter->execute($jsonData['return']);
          echo '</p>';
        }
      }
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

    echo '</div>';
  }
}; // class CdocHtmlEmitter

class CdocIndex {

  /** The name of this index. */
  private string $name;

  /** List of entries in this index. Sorted lexicographically by the index entry names of the entries. */
  private array $entryList;

  /** Map from index entry names to entries. */
  private array $entryMap;

  /**
   * Construct this CdocIndex object.
   * @param $name The name of this index.
   */
  public function __construct(string $name) {
    $this->name = $name;
    $this->entryList = array();
    $this->entryMap = array();
  }

  /**
   * Get the name of this index.
   * @return The name of this index.
   */
  public function getName() : string {
    return $this->name;
  }

  /**
   * Add an entry.
   * @param $text The text of the entry.
   * @param $id The HTML id of the entry.
   */
  public function addEntry(CdocFile $file) {
    $jsonData = $file->getJsonData();
    $name = $jsonData['name'];
    $id = $jsonData['id'];
    if (isset($this->entryMap[$name])) {
      throw new Exception('entry of text `' . $name . '` is already in index `' . $this->name . '`');
    } else {
      $entry =
        array
          (
            'index' => $this,
            'file' => $file,
          );
      $this->entryList[] = $entry;
      $this->entryMap[$name] = $entry;
    }
  }

  public function sortEntries() {
    setlocale (LC_COLLATE, 'en_US');
    uasort($this->entryList, function($x, $y) { return strcoll(strtolower($x['file']->getJsonData()['name']), strtolower($y['file']->getJsonData()['name'])); });
  }

  public function getEntryList() {
    return $this->entryList;
  }

  public function getEntryMap() {
    return $this->entryMap;
  }

}; // class CdocIndex

/**
 * A map from names to data.
 * @author Michael Heilmann
 * @singleton
 * @todo Proper documentation.
 */
class CdocIndexManager {

  private static $instance = null;

  public static function getInstance() {
    if (self::$instance == null) {
      self::$instance = new CdocIndexManager();
    }

    return self::$instance;
  }

  private array $indices = array();

  private function __construct()
  {/*Intentionally empty.*/}

  // get an index by its name.
  public function &getByName(string $name) : CdocIndex {
    if (!isset($this->indices[$name])) {
      $this->indices[$name] = new CdocIndex($name);
    }
    return $this->indices[$name];
  }
}; // class CdocIndexManager

/// @brief The "C documentation" translater.
/// It organizes all the tasks from finding the "C documentation" files to the emission of HTML, LaTeX, Markdown, ... code.
/// @author Michael Heilmann
class CdocContext {
  
  private static $instance = null;

  public static function getInstance() {
    if (self::$instance == null) {
      self::$instance = new CdocContext();
    }

    return self::$instance;
  }

  protected function __construct() {
  }

  /// This is a map. An entry consists of the path name of a Cdoc file to a CdocFile object of that Cdoc file.
  /// The values are sorted as specified by CDocFile::order.
  private $contents = array();
  
  // register a contents file by its path
  // registration means
  // - add the file to the list of content files
  // - add an entry for the content of the file to the indices specified by the content
  // raises an exception if that contents file is already registered
  public function registerContents($pathname) {
    if (isset($this->contents[$pathname])) {
      throw new Exception("contents file `" . $pathname . "` already registered");
    }
    $file = new CdocFile($pathname);
    $this->contents[$pathname] = $file;

    // get the JSON data and add the contents of the file to its corresponding indices.
    $jsonData = $file->getJsonData();
    $indexNames = $jsonData['indices'];
    foreach ($indexNames as $indexName) {
      $index = CdocIndexManager::getInstance()->getByName($indexName);
      $index->addEntry($file);
    }
  }

  public function findContentsRecursive(string $dir) {
    $stack = array();

    $dir = realpath($dir);
    $stack[] = $dir;
    while (count($stack) > 0) {
      $dir = array_pop($stack);
      $dirHandle = opendir($dir);
      if (!$dirHandle) {
        throw new Exception("unable to open fs object `" . $dir . "` as directory");
      }
      try {
        while (false !== ($relativeEntry = readdir($dirHandle))) {
          // Skip '.' and '..' fs objects.
          if ($relativeEntry == "." || $relativeEntry == "..") {
            continue;
          }
          $absoluteEntry  = $dir. DIRECTORY_SEPARATOR  .$relativeEntry;
          // Skip non-file, non-directory fs objects.
          if (!is_dir($absoluteEntry) && !is_file($absoluteEntry)) {
            continue;
          }
          if (is_dir($absoluteEntry)) {
            $stack[] = $absoluteEntry;
          } else {
            // Skip files without the "json" extension.
            $extension = pathinfo($absoluteEntry, PATHINFO_EXTENSION);
            if ($extension != 'json') {
              continue;
            }

            // Finally, register the file.
            $this->registerContents($absoluteEntry);
          }
        }
      } finally {
        closedir($dirHandle);
      }
    }
  }

  /**
   * Find all "C documentation" files in a directory and add them to the list of contents.
   * If a "C documentation" file was already added, an exception is raised.
   * @param $directory The directory.
   */
  public function findContents(string $dir) {
    $dir = realpath($dir);
    $dirHandle = opendir($dir);
    if (!$dirHandle) {
      throw new Exception("unable to open fs object `" . $dir . "` as directory");
    }
    try {
      while (false !== ($relativeEntry = readdir($dirHandle))) {
        // Skip '.' and '..' fs objects.
        if ($relativeEntry == "." || $relativeEntry == "..") {
          continue;
        }
        $absoluteEntry  = $dir. DIRECTORY_SEPARATOR .$relativeEntry;
        // Skip non-file fs objects.
        if (!is_file($absoluteEntry)) {
          continue;
        }
        // Skip files without the "json" extension.
        $extension = pathinfo($absoluteEntry, PATHINFO_EXTENSION);
        if ($extension != 'json') {
          continue;
        }
        // Finally, register the file.
        $this->registerContents($absoluteEntry);
      }
    } finally {
      closedir($dirHandle);
    }
  }

  // get the registered json content files
  public function getContents() {
    return $this->contents;
  }

  protected function sortContents() {
    uasort($this->contents, function($x, $y) { return $x->order($y); });
  }

  // if $includedIndices is null, then it is ignored. Otherwise only contents in the listed indices is included.
  public function emitContents($includedIndices = null) {
    if (!is_null($includedIndices)) {
      if (is_array($includedIndices)) {
        foreach ($includedIndices as $includedIndex) {
          if (!is_string($includedIndex)) {
            throw new Error('invalid argument');
          }
        }
      } else if (is_string($includedIndices)) {
        $temporary = array();
        array_push($temporary, $includedIndices);
        $includedIndices = $temporary;
      } else {
        throw new Error('invalid argument');
      }
    }
    $this->sortContents();
    $emitter = new CdocHtmlEmitter();
    foreach ($this->getContents() as $contents) {
      if (!is_null($includedIndices)) {
        $jsonData = $contents->getJsonData();
        $indices = $jsonData['indices'];
        if (!count(array_intersect($indices, $includedIndices))) {
          continue;
        }
      }
      $emitter->emitContentsEntry($contents);
    }
  }
};

?>
