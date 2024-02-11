<?php
require_once($_SERVER['DOCUMENT_ROOT'] . '/site.php');
$options = [];
$options['site_title'] = $SITE_TITLE . ' :: ' . 'Trigonometric Functions';
$options['site_url_prefix'] = $SITE_URL_PREFIX;
on_enter_document($options);
?>
  <?php on_head($options); ?>
  <body>
  <header class="box-shadow-bottom">
    <?php on_logo($options); ?>
  </header>
  <main>
    <p>The definition of the trigonometric functions explained via the unit circle.</p>
    <img src=<?php echo '"' . $options['site_url_prefix'] . "mathematical-foundations/trigonometric-functions/unit-circle-2023-10-24.svg" . '"'; ?> alt="My Happy SVG"/>
    <p>
    $$
    \begin{gathered}
    \overline{CP} = \sin b\\
    \overline{SP} = \cos b\\
    \overline{DT} = \tan b\\
    \overline{EK} = \cot b\\
    \overline{OT} = \sec b\\
    \overline{OK} = \csc b\\
    \end{gathered}
    $$
  </p>
  </main>
  <footer class="box-shadow-top">
  </footer>
  <body>
<?php
on_leave_document();
?>
