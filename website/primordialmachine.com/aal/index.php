<?php
define('SITE_PRIVATE', true);
$ROOT_DIR = $_SERVER['DOCUMENT_ROOT'];
$DOCUMENT_DIR = $_SERVER['DOCUMENT_ROOT'] . '/aal/';
require_once($ROOT_DIR . '/site.php');
$options = [];
$options['site_title'] = $SITE_TITLE;
$options['site_url_prefix'] = $SITE_URL_PREFIX;
$options['additional_css_stylesheets']= array('aal/index.css?v=1');
on_enter_document($options);

require_once($ROOT_DIR . '/libraries/Template/Include.php');
require_once($ROOT_DIR . '/libraries/Cdoc/Include.php');
?>
  <?php on_head($options); ?>
  <body>
  <header class="box-shadow-bottom">
    <?php on_logo($options); ?>
  </header>
  <main>
    <div class="container">
      <div class="left-column box-shadow-right">
        <ul class="social-icons">
          <li>
            <a class="icon-github" href="https://github.com/primordialmachine/primordialmachine"></a>
          </li>
          <li>
            <a class="icon-gitlab" href="https://gitlab.com/primordialmachine/primordialmachine"></a>
          </li>
          <li>
            <a class="icon-bitbucket" href="https://bitbucket.com/primordialmachine/primordialmachine"></a>
          </li>
        </ul>
      </div>
      <div class="center-column">
        <h1>Primordial Machine's Audials Library</h1>
        <p>
        Nothing to see here yet. Come back later, traveler.
        </p>
      </div>
      <div class="right-column">
        <nav>
          <ul>
            <?php
              $index = CdocIndexManager::getInstance()->getByName('aal');            
              for ($i = 0; $i < count($index); ++$i) {
                $indexEntry = $index[$i];
                echo
                  '<li>' .
                  '<a href="' . $indexEntry['href'] . '">' . $indexEntry['text'] . '</a>' .
                  '</li>'
                  ;
              }
            ?>
          </ul>
        </nav>
      </div>
    </div> <!-- container -->
  </main>
  <footer class="box-shadow-top">
  </footer>
  </body>
<?php
on_leave_document();
?>
