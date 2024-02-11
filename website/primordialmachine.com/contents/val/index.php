<?php
define('SITE_PRIVATE', true);
$ROOT_DIR = $_SERVER['DOCUMENT_ROOT'];
if ($ROOT_DIR != '/') {
  $ROOT_DIR .= DIRECTORY_SEPARATOR;
}
$DOCUMENT_DIR = $ROOT_DIR . 'val' . DIRECTORY_SEPARATOR;
require_once($ROOT_DIR . 'site.php');
$options = [];
$options['site_title'] = App::getInstance()->site_title;
$options['site_url_prefix'] = App::getInstance()->site_url_prefix;
$options['additional_css_stylesheets']= array('contents/val/index.css?v=2');
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
        <h1>Primordial Machine's Visuals Library</h1>
        <p>
        Nothing to see here yet. Come back later, traveler.
        </p>
        <?php
        if (PRINT_DIRECTORY_INFORMATION) {
          echo '<p>root directory:    ' . $ROOT_DIR . '</p>';
          echo '<p>document directory:' . $DOCUMENT_DIR . '</p>';
          echo '<p>include directory: ' . $DOCUMENT_DIR . 'includes' . DIRECTORY_SEPARATOR . '</p>';
        }
        $context = CDocContext::getInstance();
        $context->emitContents('val');
        ?>
      </div>
      <div class="right-column">
        <nav>
          <ul>
            <?php
              $emitter = new CdocHtmlEmitter();
              $index = CdocIndexManager::getInstance()->getByName('val');
              $index->sortEntries();
              foreach ($index->getEntryList() as $entry) {
                $emitter->emitTableOfContentsEntry($entry);
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
