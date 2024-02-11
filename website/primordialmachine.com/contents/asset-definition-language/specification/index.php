<?php
define('SITE_PRIVATE', true);
$ROOT_DIR = $_SERVER['DOCUMENT_ROOT'];
$DOCUMENT_DIR = $_SERVER['DOCUMENT_ROOT'] . '/asset-definition-language/';
require_once($ROOT_DIR . '/site.php');
$options = [];
$options['site_title'] = App::getInstance()->site_title;
$options['site_url_prefix'] = App::getInstance()->site_url_prefix;
$options['additional_css_stylesheets']= array('contents/asset-definition-language/index.css?v=1');
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
        <h1>Asset Definition Language</h1>
        <p>
        Nothing to see here yet. Come back later, traveler.
        </p>
        <!--
        <h2>Tutorial</h2>
        <p>
        An asset definitions file contains a number of asset definitions.
        An asset definition is henceforth simply called a "def" (plural "defs").
        The outermost asset definition in a file must be a an asset of type <code>Package</code>.
        </p>

        <p>
        Create a file <code>MyPalette1.adl</code> and paste the following contents into that file:
        </p>
        <p><code>
        {
          type : "Package",
          name : "MyPalette1",
          definitions : [
            {
              "type" : "Color",
              "name" : "MyRed",
              "red" : 255,
              "green" : 0,
              "blue" : 0,
            }
          ]
        }
        </code></p>
        
        <p>
        The outermost element is an asset definition of type <code>Package</code>. That package has a name
        <code>MyPalette</code>. The <code>definitions</code> is a list the asset defs inside that package.
        This list contains a single element, an asset definition for the color red.
        That color has a name <code>MyRed</code>.
        </p>

        <p>
        Loading this file using the C api (granted there are no conflicts) will enable you to acquire the
        def of the def o the color:
        </p>

        <p><code>
        Core_String* path = NULL;
        Core_String_createFromC(&path, "./MyPalette1.adl");
        Core_Asset_Context_load(
        Core_String* fqn = NULL;
        Core_String_createFromC(&fqn, "MyPalette.MyRed");
        Core_Asset_Def* def = NULL;
        Core_Assets_getDef(context, &def, fqn);
        </code></p>

        <p><code>MyPalette1.MyRed</code> is called the fully qualified name of the asset definition for our color.
        In all files loaded, no two defs of the same fully qualified name may exist.
        </p>

        <h2>Refs</h2>
        <p>
        An def may contain asset references to other defs.
        An asset reference is henceforth simply called a "ref" (plural "refs").
        </p>

        <p>
        Create a file <code>Scene1.adl</code> and paste the following contents into that file:
        </p>
        <p><code>
        {
          type : "Package",
          name : "MyScene1",
          definitions : [
            {
              "type" : "Image",
              "name" : "MyImage",
              "color" : {
                "type": "Ref",
                "name" : "MyPalette.MyRed"
              }
            }
          ]
        }
        </code></p>
        -->
      </div>
    </div> <!-- container -->
  </main>
  <footer class="box-shadow-top">
  </footer>
  </body>
<?php
on_leave_document();
?>
