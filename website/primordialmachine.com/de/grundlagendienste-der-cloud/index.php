<?php
define('SITE_PRIVATE', true);
require_once($_SERVER['DOCUMENT_ROOT'] . '/site.php');
$options = [];
$options['site_title'] = $SITE_TITLE;
$options['site_url_prefix'] = $SITE_URL_PREFIX;
$options['additional_css_stylesheets']= array('de/grundlagendienste-der-cloud/index.css?v=1');
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
        <?php
        include($_SERVER['DOCUMENT_ROOT'] . '/de/grundlagendienste-der-cloud/body.php');
        ?>
      </div>
      <div class="right-column">
      </div>
    </div><!-- container -->
  </main>
  <footer class="box-shadow-top">
  </footer>
  </body>
<?php
on_leave_document();
?>
