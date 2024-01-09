<?php

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

?>
