<?php
defined('SITE_PRIVATE') || die("no direct access");
?>
<h2>Syntactical Structure</h2>
<p>
The syntactical grammar describes the translation of the sequence of words that make up a program into sentences.
The single goal symbol of the syntactical grammar is the sentence <code>sentence</code> symbol.
</p>

<p>The words <code>whitespace</code>, <code>line_terminator</code>, and <code>comment</code> are removed from the
sequence of words before the translation to sentences is performed.</p>

<p>The goal sentence <code>sentence</code> is defined by</p>
<p><code>
sentence : value
</code></p>

<p>
The sentence <code>value</code> is defined by
</p>
<p><code>
value : map</br>
value : list</br>
value : string</br>
value : number</br>
value : boolean</br>
value : void
</code></p>

<p>
The sentence <code>map</code> is defined by
</p>
<p><code>
map : left_curly_bracket</br>
&nbsp;&nbsp;map_body</br>
&nbsp;&nbsp;right_curly_bracket</br>
</br>
map_body : map_body_element map_body_rest</br>
map_body : &epsilon;</br>
</br>
map_body_rest : comma map_body_element map_body_rest</br>
map_body_rest : comma</br>
map_body_rest : &epsilon;</br>
</br>
map_body_element : name colon value</br>
</code></p>

<p>
The sentence <code>list</code> is defined by
</p>
<p><code>
list : left_square_bracket</br>
&nbsp;&nbsp;list_body</br>
&nbsp;&nbsp;right_square_bracket</br>
</br>
list_body : list_body_element list_body_rest</br>
list_body : &epsilon;</br>
</br>
list_body_rest : comma list_body_element list_body_rest</br>
list_body_rest : comma</br>
list_body_rest : &epsilon;</br>
</br>
list_body_element : value
</code></p>
