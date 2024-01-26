<?php

$dir = __DIR__;
if (str_ends_with($dir, DIRECTORY_SEPARATOR)) {
  require_once($dir . './../App.php');
  require_once($dir . 'CdocFile.php');
  require_once($dir . 'CdocEmitter.php');
  require_once($dir . 'CdocHtmlEmitter.php');
  require_once($dir . 'CdocIndexManager.php');
} else {
  require_once($dir . '/./../App.php');
  require_once($dir . '/CdocFile.php');
  require_once($dir . '/CdocEmitter.php');
  require_once($dir . '/CdocHtmlEmitter.php');
  require_once($dir . '/CdocIndexManager.php');
}

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
