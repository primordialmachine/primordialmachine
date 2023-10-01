<?php
defined('SITE_PRIVATE') || die("no direct access");
?>
<h2>Types and Values</h2>
<p>The <em>Data Definition Language</em> knows six basic types <em>List</em> and <em>Map</em>,
which are the so called aggregate types, and <em>Boolean</em>, <em>Number</em>, <em>String</em>
and <em>Void</em>, which are the so called scalar types.

<h3>Scalar Types</h3>

<h4><em>Boolean</em> Type</h4>
<p>The type <em>Boolean</em> has two values <em>true</em> and <em>false</em> which are expressed in the language by the words
<code>true</code> and <code>false</code>, respectively (as defined in the lexical grammar).

<h4><em>Number</em> Type</h4>
<p>The type <em>Number</em> represents both 2-complement integer numbers as well as IEE754 floating-point numbers.
A value of type <em>Number</em> is expressed in the language by the word `number` (as defined in the lexical grammar).</p>

<p>Note that the <em>Data Definition Language</em> does not impose restrictions on the range and precision of values
of type <em>Number</em>. Implementations, however, may impose restrictions.</p>

<h4><em>String</em> Type</h4>
<p>The type <em>String</em> represents UTF-8 strings. <em>String</em> values are expressed in the language by the word <code>string</code> (as defined in the lexical grammar).
At the end of the lexical translation of a <em>String</em> word, its escape sequences are replaced by the Unicode characters they are representing.
Furthermore, the opening and closing quotes are removed.</p>

<p>
Note that the <em>Data Definition Language</em> does not impose restrictions on the length of values of type <em>String</em>.
Implementations, however, may impose restrictions.
<p>

<h4><em>Void</em> Type</h4>
<p>
The type <em>Void</em> has a single value <em>void</em> which is represented in the language by the word
<code>void</code> (as defined in the lexical grammar).
</p>

<h3>Aggregate Types</h3>

<h4><em>List</em> Type</h4>
<p>The type <em>List</em> represents lists of values.
A value of type <em>List</em> is expressed in the language by the sentence <code>list</code> (as defined in the syntactical grammar).</p>

<div class="example">
<p>Example:</p>
<p><code>
// A list with three numbers 1, 2, and 3.</br>
[ 1, 2, 3 ]
</code></p>
</div>

<h4><em>Map</em> Type</h4>
<p>The type <em>Map</em> represents maps from names to values.
A value of type <em>Map</em> is expressed in the language by the sentence <code>map</code> (as defined in the syntactical grammar).</p>

<div class="example">
<p>Example:</p>
<p><code>
// A map of</br>
// text to 'Hello, World!'</br>
// action to 'Print', and</br>
// fontSize to 12.</br>
{ text : 'Hello World!', action : 'Print', fontSize: 12 }
</code></p>
</div>

<p>
If two name/value pairs from the same name in a map are
specified, then last specified    name/value pair takes
precedence.
</p>

<div class="example">
<p>Example:</p>
<p>
The following <em>Data Definition Language</em> program defines a <em>Map</em> value that contains two name/value pairs with the same name <code>x</code>.
The first name/value pair maps to the value <code>0</code> and second name/value pair to the number <code>1</code>.
</p>
<p><code>
{ x : 0, x : 1 }
</code></p>
<p>The effective <em>Map</em> value defined by the program is hence</p>
<p><code>
{ x : 1 }
</code></p>
<p>as name/value pair mapping to the value <code>0</code> is specified before the name/value pair mapping to the value <code>1</code>.</p>
