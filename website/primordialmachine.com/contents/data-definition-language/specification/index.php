<?php
define('SITE_PRIVATE', true);
require_once($_SERVER['DOCUMENT_ROOT'] . '/site.php');
$options = [];
$options['site_title'] = App::getInstance()->site_title;
$options['site_url_prefix'] = App::getInstance()->site_url_prefix;
$options['additional_css_stylesheets']= array('contents/data-definition-language/specification/index.css?v=2');
on_enter_document($options);
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
        <h1>Data Definition Language</h1>
        <?php
        include($_SERVER['DOCUMENT_ROOT'] . '/contents/data-definition-language/specification/includes/1-introduction.php');
        include($_SERVER['DOCUMENT_ROOT'] . '/contents/data-definition-language/specification/includes/2-grammars.php');
        include($_SERVER['DOCUMENT_ROOT'] . '/contents/data-definition-language/specification/includes/3-lexical-structure.php');
        include($_SERVER['DOCUMENT_ROOT'] . '/contents/data-definition-language/specification/includes/4-syntactical-structure.php');
        include($_SERVER['DOCUMENT_ROOT'] . '/contents/data-definition-language/specification/includes/5-types-and-values.php');
        ?>
      </div>
      <div class="right-column">
      </div>
    </div> <!-- container -->
  </main>
  <footer class="box-shadow-top">
  </footer>
  </body>
<?php
on_leave_document();
?>
