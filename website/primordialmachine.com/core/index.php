<?php
define('SITE_PRIVATE', true);
$ROOT_DIR = $_SERVER['DOCUMENT_ROOT'];
$DOCUMENT_DIR = $_SERVER['DOCUMENT_ROOT'] . '/core';
require_once($ROOT_DIR . '/site.php');
$options = [];
$options['site_title'] = $SITE_TITLE;
$options['site_url_prefix'] = $SITE_URL_PREFIX;
$options['additional_css_stylesheets']= array('core/index.css?v=1');
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
        <h1>Primordial Machine's Core Library</h1>
        <p>
        Primordial Machine's Core Library is a C library.
        It four primary purposes:
        </p>
        <ul class="p-list">
          <li>
          protect developers from compilation and execution environment idiosyncracies and enables cross-platform development
          </li>
          <li>
          provide developers with a framework (code and patterns) for object-oriented programming and concurrent programming
          as well as error handling and resolution in order to create maintainable, re-usable, efficient, and safe programs
          in the C language.
          </li>
          <li>
          provide developers implement functionality on top of the aforementioned framework that is dearly missed when writing
          programs in the C language. This functionality includes (but is not restricted to) UTF-8 string representations and
          operations, code annotations, memory management, data structures (array lists, hash maps, etc.), mathematics, safe
          arithmetic.
          </li>
        </ul>
        <p>
        The library emphasizes error-safety for embedding programs:
        that is, when used as intended, it will never never simply terminate upon an failure and always either resolve or propagate errors.
        </p>
        <?php
        $context = new CdocContext();
        $context->findContentsRecursive($DOCUMENT_DIR . '/includes/');
        $context->emit();
        ?>
      </div>
      <div class="right-column">
        <nav>
          <ul>
            <?php
              $index = CdocIndexManager::getInstance()->getByName('core');
              $index->sortEntries();
              for ($i = 0; $i < count($index->getEntryList()); ++$i) {
                $entry = $index->getEntryList()[$i];
                echo
                  '<li>' .
                  '<a href="' . $entry['href'] . '">' . $entry['text'] . '</a>' .
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
