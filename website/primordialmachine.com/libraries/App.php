<?php

require_once(__DIR__ . '/../config.php');

/// @singleton
class App {
  
  private static $instance = null;

  public static function getInstance() {
    if (self::$instance == null) {
      self::$instance = new App();
    }
 
    return self::$instance;
  }

  // list of CSS files.
  public $css_files;

  // list of JavaScript files.
  public $js_files;

  // site URL prefix as specified by $SITE_URL_PREFIX.
  public $site_url_prefix;

  // site title as specified by $SITE_TITLE.
  public $site_title;
  
  /// site document root.
  public $site_document_root;

  private function __construct() {
    //
    //global $SITE_URL_PREFIX;
    $this->site_url_prefix = SITE_URL_PREFIX;

    //global $SITE_TITLE;
    $this->site_title = SITE_TITLE;
    
    global $_SERVER;
    $this->site_document_root = $_SERVER['DOCUMENT_ROOT'];

    $this->css_files =
      array
        (
          ($this->site_url_prefix) . 'reset.css',
          ($this->site_url_prefix) . 'layout.css',
          ($this->site_url_prefix) . 'colors-variant-1.css',
          ($this->site_url_prefix) . 'site.css',
          ($this->site_url_prefix) . 'icons.css',
          ($this->site_url_prefix) . 'typography-variant-1.css',
          ($this->site_url_prefix) . 'decorations.css',
        );

    $this->js_files =
      array
        (
        );

  } // __construct

}; // class

?>
