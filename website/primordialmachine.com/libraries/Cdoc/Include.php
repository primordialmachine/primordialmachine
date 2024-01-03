<?php

/** 
 * Represents a "C documentation" file.
 * @author Michael Heilmann
 */
class CDocFile {
  
  /**
   * The pathname of the file.
   */
  private string $pathname;
  
  /**
   * The JSON data of the contents file.
   * @default null
   */
  private $jsonData;
  
  /**
   * Construct this TlContentsFile object.
   * @param $pathname The pathname of the contents file.
   */
  public function __construct(string $pathname) {
    $this->pathname = $pathname;
  }
  
  /**
   * Get the pathname of the contents file.
   * @return The pathname of the contents file.
   */
  public function getPathname() {
    return $this->pathname;
  }
   
  /**
   * Get the JSON data of the contents file.
   * @return The JSON data of the contents file.
   */
  public function getJsonData() {
    if (null == $this->jsonData) {
      $textData = file_get_contents($this->pathname);
      if (null == $textData) {
        throw new Exception("unable to load data (text) from file `" . $this->pathname . "`");
      }
      $jsonData = json_decode($textData, true);
      if (null == $jsonData) {
        throw new Exception("unable to load data (json) from file `" . $this->pathname . "`: " . json_last_error_msg());
      }
      $this->jsonData = $jsonData;
    }
    return $this->jsonData;
  }
  
  /**
   * Compare this TlContentsFile object with another TlContentsFile object.
   * The string comparisong of their pathnames are used for comparison.
   * @return
   * A value smaller (greater) than 0 if
   * the contents file represented to by this TlContentsFile object is smaller (greater) than
   * the contents file represented by the other TlContentsFile object.
   * 0 if the represented contents files are equivalent.
   */
  public function order(CdocFile $other) {
    return strcmp($this->getJsonData()['name'], $other->getJsonData()['name']);
  }
  
}; // class TlContentsFile


/**
 * The base of all "C Documentation" emitters.
 * An emitter emit code (e.g., HTML, LaTeX, Markdown, ...) from "C Documentation" files.
 * @author Michael Heilmann
 */
abstract class CdocEmitter {
  
  /**
   * @brief Emit the code for a CdocFile object.
   * @param $file The CdocFile object for which the code is emitted.
   */
  public abstract function emit(CdocFile $file);
  
}; // // class CdocEmitter

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
    echo '<h3 class="cdoc-entry" id = "' . $entry['id'] . '">';
    echo $entry['text'];
    echo '</h3>';
  }
 
  public function __construct() {
    $this->interpreter = new TlInterpreter(new TlParser(new TlScanner()));
  }
  
  public function emit(CdocFile $file) {
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
  /** The entries in this index. */
  private array $entryList;
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
  public function addEntry(string $text, string $id) {
    if (isset($this->entryMap[$text])) {
      throw new Exception('entry of text `' . $text . '` is already in index `' . $this->name . '`');
    } else {
      $entry =
        array
          (
            'text' => $text,
            'id' => $id,
            'href' => App::getInstance()->site_url_prefix . $this->name . '#' . $id
          );
      $this->entryList[] = $entry;
      $this->entryMap[$text] = $entry;
    }
  }
  
  public function sortEntries() {
    uasort($this->entryList, function($x, $y) { return strcmp($x['text'], $y['text']); });
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
  
  private $contents = array();
  
  // register a json file contents file
  // dies if this file is already registered
  public function registerContents($pathname) {
    if (isset($this->contents[$pathname])) {
      throw new Exception("contents file `" . $pathname . "` already registered");
    } else {
      $this->contents[$pathname] = new CdocFile($pathname);
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
  
  public function sortContents() {
    function cmp($x, $y) {
      return $x->order($y);
    }
    uasort($this->contents, 'cmp');
  }
  
  public function emit() {
    foreach ($this->getContents() as $x) {
      $jsonData = $x->getJsonData();

      if (!isset($jsonData['indices'])) {
        throw new Exception($file->getPathname() . ': error: `indices` is not not defined.');
      }
      
      if (is_array($jsonData['indices'])) {
        if (count($jsonData['indices']) > 1) {
          throw new Exception($file->getPathname() . ': error: multiple indices in `indices` not yet supported.');
        }
        $index = CdocIndexManager::getInstance()->getByName($jsonData['indices'][0]);
        $index->addEntry($jsonData['name'], $jsonData['id']);
      } else if (is_string($jsonData['indices'])) {
        $index = CdocIndexManager::getInstance()->getByName($jsonData['indices']);
        $index->addEntry($jsonData['name'], $jsonData['id']);
      } else {
        throw new Exception($file->getPathname() . ': error: `indices` must be an array of zero or more strings or a string or undefined.');
      }
    }
    $this->sortContents();
    $emitter = new CdocHtmlEmitter();
    foreach ($this->getContents() as $x) {
      $emitter->emit($x);
    }
  }
};

?>
