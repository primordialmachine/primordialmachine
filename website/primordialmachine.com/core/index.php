<?php
require_once($_SERVER['DOCUMENT_ROOT'] . '/site.php');
$options = [];
$options['site_title'] = $SITE_TITLE;
$options['site_url_prefix'] = $SITE_URL_PREFIX;
$options['additional_css_stylesheets']= array('index.css?v=7ca97d4ac6f749b496c0cd49aca22b2c');
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
      <div class="news-item">
        <div class="news-item-header">
          <h1>2023-10-23</h1>
        </div>
        <div class="news-item-body">
          <p>
          The website was updated.
          </p>
          <p>
          In particular, typography was improved.
          </p>
          <p>
          In the course of the next few weeks, the existing documentation that was created so far, will be migrated to this site.
          </p>
        </div>
      </div>
    </div><!-- center-column -->
    <div class="right-column">
    </div>
  </div>
  
  </main>
  <footer class="box-shadow-top">
  </footer>
  </body>
<?php
on_leave_document();
?>
