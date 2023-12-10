<?php

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
    global $SITE_URL_PREFIX;
    $this->site_url_prefix = $SITE_URL_PREFIX;

    global $SITE_TITLE;
    $this->site_title = $SITE_TITLE;
    
    global $_SERVER;
    $this->site_document_root = $_SERVER['DOCUMENT_ROOT'];

    $this->css_files =
      array
        (
          ($SITE_URL_PREFIX) . 'reset.css',
          ($SITE_URL_PREFIX) . 'layout.css',
          ($SITE_URL_PREFIX) . 'colors-variant-1.css',
          ($SITE_URL_PREFIX) . 'site.css',
          ($SITE_URL_PREFIX) . 'icons.css',
          ($SITE_URL_PREFIX) . 'typography-variant-1.css',
          ($SITE_URL_PREFIX) . 'decorations.css',
        );

    $this->js_files =
      array
        (
        );

  } // __construct

}; // class

?>
