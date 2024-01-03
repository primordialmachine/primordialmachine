<?php
$ROOT_DIR = $_SERVER['DOCUMENT_ROOT'];
if ($ROOT_DIR != '/') {
  $ROOT_DIR .= DIRECTORY_SEPARATOR;
}
require_once($ROOT_DIR . 'site.php');
$options = [];
$options['site_title'] = App::getInstance()->site_title;
$options['site_url_prefix'] = App::getInstance()->site_url_prefix;
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
            <h1>2023-12-23</h1>
          </div>
          <div class="news-item-body">
            <p>
            The website was updated.
            Find below a list of the most important changes.
            </p>
            <ul style="list-style: square inside;">
              <li>Fix navigational elements</li>
              <li>Improve site responsiveness in particular on mobile devices with small screens</li>
              <li>Add placeholders for the <em>Asset Definition Language Specification</em> and the <em>AAL Library</em> and the <em>VAL Library</em></li>
              <li>Improve layout of <em>Data Definition Language Specification</em></li>
              <li>Add to the documentation of the <em>Core Library</em> many previously undocumented elements</li>
            </ul>
          </div>
        </div>
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
        <div class="news-item">
          <div class="news-item-header">
            <h1>2023-10-13</h1>
          </div>
          <div class="news-item-body">
            <p>
            Hi! I am  Michael Heilmann. This is my website.
            </p>
            <p>
            I would like to introduce you to an open source project of mine: <em>Primordial Machine</em> is a game in which you embark on a journey through the void of
            space as an artificial intelligence. The project was effectively started in 2023 and is in an early stage. The technological as well as artistical  aspects
            of this creation are inclined and are a journey of their own. Feel free to contact me under michaelheilmann et primordialmachine dot com if you intend   to
            take part in that journey.
            </p>
            <p>
            In the course of the next few weeks, the existing documentation that was created so far, will be migrated to this site.
            </p>
          </div>
        </div>
      </div><!-- center-column -->
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
