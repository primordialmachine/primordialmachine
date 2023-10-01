<?php
/// The name of this website.
$site_name = 'Primordial Machine';

/// The title of this website.
$SITE_TITLE = 'Primordial Machine';

/// The URL prefix of this site of the form `[protocol]://([subdomain].?)[domain]/`.
$SITE_URL_PREFIX = 'https://primordialmachine.com/';
//$SITE_URL_PREFIX = "http://localhost/";

class App {

  // list of CSS files.
  public $css_files;

  // list of JavaScript files.
  public $js_files;

  // site URL prefix as specified by $SITE_URL_PREFIX.
  public $site_url_prefix;

  // site title as specified by $SITE_TITLE.
  public $site_title;

  function __construct() {
    //
    global $SITE_URL_PREFIX;
    $this->site_url_prefix = $SITE_URL_PREFIX;

    global $SITE_TITLE;
    $this->site_title = $SITE_TITLE;

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

$g_app = new App();

/// Callback: Invoked after entering the document.
function on_enter_document($properties) {
  echo '<!DOCTYPE html>';
  echo '<html>';
}

function on_logo($properties) {
  global $g_app;

  /*
   * get the adjacent "ul" element of the "a" element.
   * toggle its visibility.
   */
  echo '<script>';
    echo 'function on_dropdown_toggle(element) {';
      echo 'console.log("on_dropdown_toggle");';
      echo 'sibling = element.nextSibling;';
      echo 'sibling.classList.toggle("closed");';
      echo 'sibling.classList.toggle("opened");';
    echo '}';
  echo '</script>';

  echo '<div>';
  echo '<div class="logo">';
  echo '<a href="' . ($properties['site_url_prefix']) . '">Primordial Machine</a>';
  echo '</div>';
  echo '</div>';

  echo '<div>';
  echo '</div>';

  echo '<div>';
  echo '<nav>';

  function lnk($text, $href, $onclick) {
        echo '<a';
        if ($onclick != null) {
          echo ' ' . 'onclick="' . $onclick . '"';
        }
        echo ' ' . 'href="';
          echo $href;
        echo '">';
          echo $text;
        echo '</a>';
  }

  echo '<ul class="nav-menu-level-1">';

    echo '<li class="nav-menu-element-level-1">';
      lnk('Specifications', '#', 'on_dropdown_toggle(this)');
      echo '<ul class="nav-menu-level-2 open-left closed">';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Data Definition Language', $g_app->site_url_prefix . 'data-definition-language', null);
        echo '</li>';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Asset Definition Language', $g_app->site_url_prefix . 'asset-definition-language', null);
        echo '</li>';
      echo '</ul>';
    echo '</li>';

    echo '<li class="nav-menu-element-level-1">';
      lnk('Contact', '#', 'on_dropdown_toggle(this)');
    echo '</li>';

 echo '</ul>';

  echo '</nav>';
  echo '</div>';
}

/// Callback: Invoked before leaving the document.
function on_leave_document() {
  echo '</html>';
}

function on_head($properties) {
 echo '<head>';

 echo '  <meta charset="utf-8"/>';
 echo '  <meta name="viewport" content="width=device-width">';

 echo '  <link rel="icon" type="image/x-icon" href="' . ($properties['site_url_prefix']) . 'favicon.ico">';

 echo '  <title>' . ($properties['site_title']) . '</title>';

 echo '  <script src="https://polyfill.io/v3/polyfill.min.js?features=es6"></script>';
 echo '  <script id="MathJax-script" async src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js"></script>';

 global $g_app;
 $id = '6';
 foreach ($g_app->css_files as $css_file) {
  echo '  ';
  echo '<link rel="stylesheet" href="' . $css_file . '?v=' . $id . '">';
 }

 if (isset($properties['additional_css_stylesheets'])) {
  foreach ($properties['additional_css_stylesheets'] as $stylesheet) {
    echo '  <link rel="stylesheet" href="' . ($properties['site_url_prefix']) . $stylesheet . '">';
  }
 }

 echo '</head>';
}
?>
