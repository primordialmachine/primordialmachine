<?php

$dir = __DIR__;
if (str_ends_with($dir, DIRECTORY_SEPARATOR)) {
  require_once($dir . './../App.php');
} else {
  require_once($dir . '/./../App.php');
}

/**
 * Represents a "C documentation" file.
 * Provides access to the file's path name and to its JSON contents.
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
      
      // "indices" must be specified.
      if (!isset($jsonData['indices'])) {
        throw new Exception('`indices` not specified');
      }
      if (is_string($jsonData['indices'])) {
        // if "indices" is a single string then make "indices" an array of that string.
        $temporary = array();
        array_push($temporary, $jsonData['indices']);
        $jsonData['indices'] = $temporary;
      } else if (is_array($jsonData['indices'])) {
        // if "indices" is an array, ensure "indices" is an array of strings.
        $indices = $jsonData['indices'];
        foreach ($indices as $index) {
          if (!is_string($index)) {
            throw new Exception('if `indices` must be a string or an array of strings');
          }
        }
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
    setlocale (LC_COLLATE, 'en_US');
    return strcoll(strtolower($this->getJsonData()['name']), strtolower($other->getJsonData()['name']));
  }

}; // class CdocFile

?>
