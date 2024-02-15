<?php
require_once(__DIR__ . '/config.php');
require_once(__DIR__ . '/libraries/App.php');


/// Callback: Invoked after entering the document.
function on_enter_document($properties) {
  echo '<!DOCTYPE html>';
  echo '<html>';
}

function on_logo($properties) {
  $path = App::getInstance()->site_document_root . '/js/nav.js';
  $contents =  file_get_contents($path);

  /*
   * close all currently open menus in the navigation.
   * open the menu the user clicked on.
   */
  echo '<script>';
    echo $contents;
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
          lnk('Data Definition Language', App::getInstance()->site_url_prefix . 'data-definition-language/specification', null);
        echo '</li>';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Asset Definition Language', App::getInstance()->site_url_prefix . 'asset-definition-language/specification', null);
        echo '</li>';
      echo '</ul>';
    echo '</li>';
    echo '<li class="nav-menu-element-level-1">';
      lnk('Projects', '#', 'on_dropdown_toggle(this)');
      echo '<ul class="nav-menu-level-2 open-left closed">';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Core', App::getInstance()->site_url_prefix . 'core', null);
        echo '</li>';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Assets', App::getInstance()->site_url_prefix . 'assets', null);
        echo '</li>';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Visuals Abstraction Layer (VAL)', App::getInstance()->site_url_prefix . 'val', null);
        echo '</li>';
        echo '<li class="nav-menu-element-level-2">';
          lnk('Audials Abstraction Layer (AAL)', App::getInstance()->site_url_prefix . 'aal', null);
        echo '</li>';
      echo '</ul>';
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

  echo
    "
    <meta charset='utf-8'/>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    "
    ;
 
  // use javascript to hide partially loaded content if javascript is available
  // graceful degradation if javascript is not available
  echo
    "
    <style>
      html.hidden { visibility: hidden; }
    </style>
    <script>
    document.documentElement.classList.add('hidden');
    document.addEventListener('DOMContentLoaded', () => {
      document.documentElement.classList.remove('hidden');
    });
    </script>
    "
    ;

  echo '  <link rel="icon" type="image/x-icon" href="' . ($properties['site_url_prefix']) . 'favicon.ico">';

  echo '  <title>' . ($properties['site_title']) . '</title>';

 
  /*echo '  <script src="https://polyfill.io/v3/polyfill.min.js"></script>';*/
  echo '  <script id="MathJax-script" async src="/static/external/mathjax/tex-mml-chtml.js"></script>';

  $id = '6';
  foreach (App::getInstance()->css_files as $css_file) {
    echo '  ';
    echo '<link rel="stylesheet" type="text/css" href="' . $css_file . '?v=' . $id . '">';
  }

  if (isset($properties['additional_css_stylesheets'])) {
    foreach ($properties['additional_css_stylesheets'] as $stylesheet) {
      echo '  <link rel="stylesheet" type="text/css" href="' . ($properties['site_url_prefix']) . $stylesheet . '">';
    }
  }
  
  echo
  "
  <script>
  /*
   * location.hash returns the anchor part of an URL as a string,  with hash (#) symbol included. */
   * wait for page elements to load
   * execute function only if the anchor exists in the URL address
   */
  window.onload = function() {
    if(location.hash) {
      // remove # from the string
      var elId = location.hash.replace('#','');
      // locate the anchored element on the page by its ID property 
      var scrollToEl = document.getElementById(elId);
      // scroll to the anchored element
      if (scrollToEl) {
        scrollToEl.scrollIntoView(true);
      }
    }
  }
  </script> 
  "
  ;

  echo '</head>';
}

?>
