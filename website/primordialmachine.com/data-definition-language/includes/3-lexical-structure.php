<?php
defined('SITE_PRIVATE') || die("no direct access");
?>
<h2>Lexical Structure</h2>
<p>
The lexical grammar describes the translation of Unicode characters into words.
The single goal symbol of the lexical grammar is the <code>word</code> symbol.
</p>

<h3>goal symbol</h3>
<p>
The goal symbol <code>word</code> is defined by
</p>
<p><code>
word : delimiters</br>
word : boolean</br>
word : number</br>
word : string</br>
word : void</br>
word : name</br>
word : left_curly_bracket</br>
word : right_curly_bracket</br>
word : left_square_bracket</br>
word : right_square_bracket</br>
word : comma</br>
word : colon</br>
/*whitespace, newline, and comment are not considered the syntactical grammar*/</br>
word : whitespace</br>
word : newline</br>
word : comment</br>
</code></p>

<h3>whitespaces</h3>
<p>The word <code>whitespace</code> is defined by</p>
<p><code>
/* #9 is also known as "CHARACTER TABULATION" */</br>
whitespace : #9</br>
/* #20 is also known as "SPACE" */</br>
whitespace : #20</br>
</code></p>

<h3>line terminators</h3>
<p>The word <code>line_terminator</code> is defined by</p>
<p><code>
/* #a is also known as "LINEFEED (LF)" */</br>
/* #d is also known as "CARRIAGE RETURN (CR)" */</br>
line_terminator : #a {#d}</br>
line_terminator : #d {#a}
</code></p>

<h3>comments</h3>
<p>The language supports both single-line comments and multi-line comments. A <code>comment_block</code> is either a <code>single_line_comment</code> or a <code>multi_line_comment</code>
and hence is defined by</p>
<p><code>
comment : single_line_comment</br>
        | multi_line_comment
</code></p>

<p>A <code>single_line_comment</code> starts with two solidus. It extends to the end of the line. Hence it is defined by</p>
<p><code>
/* #2f is also known as SOLIDUS */</br>
single_line_comment :</br>
  #2f #2f</br>
  /* any sequence of characters except for line_terminator */
</code></p>
<p>The <code>line_terminator</code> is not considered as part of the comment text.</p>

<p>A <code>multi_line_comment</code> is opened by a solidus and an asterisk and closed by an asterisk and a solidus. Hence it is defined by</p>
<p><code>
/* #2f is also known as SOLIDUS */</br>
/* #2a is also known as ASTERISK */</br>
multi_line_comment :</br>
  #2f #2a</br>
  /* any sequence of characters except except for #2a #2f */</br>
  #2a #2f
</code></p>
<p>The <code>#2f 2a</code> and <code>#2a #2f</code> sequences are not considered as part of the comment text.</p>

<p>This implies</p>
<ul class="p-list">
  <li><code>//</code> has no special meaning either comment.</li>
  <li><code>/*</code> and <code>*/</code> have no special meaning in single-line comments.
  <li>Multi-line comments do not test.</li>
</ul>

<h3>parentheses</h3>
<p>The words <code>left_parenthesis</code> and <code>right_parenthesis</code>, respectively, are defined by</p>
<p><code>
/* #28 is also known as "LEFT PARENTHESIS" */</br>
left_parenthesis : #28</br>
/* #29 is also known as "RIGHT PARENTHESIS" */</br>
right_parenthesis : #29
</code></p>

<h3>curly brackets</h3>
<p>The words <code>left_curly_bracket</code> and <code>right_curly_bracket</code>, respectively, are defined by</p>
<p><code>
/* #7b is also known as "LEFT CURLY BRACKET" */</br>
left_curly_bracket : #7b</br>
/* #7d is also known as "RIGHT CURLY BRACKET" */</br>
right_curly_bracket : #7d
</code></p>
 
<h3>colon</h3>
<p>The word <code>colon</code> is</p>
<p><code>
/* #3a is also known as "COLON" */</br>
colon : #3a
</code></p>
 
<h3>square brackets</h3>
<p>The words <code>left_square_bracket</code> and <code>right_square_bracket</code>, respectively, are defined by</p>
<p><code>
/* #5b is also known as "LEFT SQUARE BRACKET" */</br>
left_square_bracket : #5b</br>
/* #5d is also known as "RIGHT SQUARE BRACKET" */</br>
right_square_bracket : #5d
</code></p>

<h3>alphanumeric</h3>
<p>The word <code>alphanumeric</code> is reserved for future use.</p>

<h3>comma</h3>
<p>The word <code>comma</code> is</p>
<p><code>
/* #2c is also known as "COMMA" */
comma : #2c
</code></p>

<h3>name</h3>
<p>The word <code>name</code> is defined by</p>
<p><code>
name : {underscore} alphabetic {name_suffix_character}</br>
</br>
/* #41 is also known as "LATIN CAPITAL LETTER A" */</br>
/* #5a is also known as "LATIN CAPITAL LETTER Z" */</br>
/* #61 is also known as "LATIN SMALL LETTER A" */</br>
/* #7a is also known as "LATIN SMALLER LETTER Z" */</br>
name_suffix_character : /* The unicode characters from #41 to #5a and from #61 to #7a. */</br>
</br>
/* #30 is also known as "DIGIT ZERO" */</br>
/* #39 is also known as "DIGIT NINE" */</br>
name_suffix_character : /* The unicode characters from #30 to #39. */</br>
</br>
/* #5f is also known as "LOW LINE" */</br>
name_suffix_character : #5f
</code></p>

<h3>number</h3>
<p>The word <code>number</code> is defined by</p>
<p><code>
number : integer_number</br>
number : real_number</br>
integer_number : [sign] digit {digit}</br>
real_number : [sign] period digit {digit} [exponent]</br>
real_number : [sign] digit {digit} [period {digit}] [exponent]</br>
exponent : exponent_prefix [sign] digit {digit}</br>
</br>
/* #2b is also known as "PLUS SIGN" */</br>
sign : #2b</br>
/* #2d is also known as "MINUS SIGN" */</br>
sign : #2d</br>
/* #2e is also known as "FULL STOP" */</br>
period : 2e</br>
/* #65 is also known as "LATIN SMALL LETTER E" */</br>
exponent_prefix : #65</br>
/* #45 is also known as "LATIN CAPITAL LETTER E" */</br>
exponent_prefix : #45
</code></p>

<h3>string</h3>
<p>The word <code>string</code> is defined by</p>
<p><code>
string : single_quoted_string</br>
stirng : double_quoted_string</br>
</br>
double_quoted_string : double_quote {double_quoted_string_character} double_quote</br>
double_quoted_string_character : /* any character except for newline and double_quote */</br>
double_quoted_string_character : escape_sequence</br>
double_quoted_string_character : #5c double_quote</br>
/* #22 is also known as "QUOTATION MARK" */</br>
double_quote : #22</br>
</br>
single_quoted_string : single_quote {single_quoted_string_character} single_quote</br>
single_quoted_string_character : /*any character except for newline and single quote*/</br>
single_quoted_string_character : escape_sequence</br>
single_quoted_string_character : #5c single_quote</br>
/* #27 is also known as "APOSTROPHE" */</br>
single_quote : #27</br>
</br>
/* #5c is also known as "REVERSE SOLIDUS" */</br>
escape_sequence : #5c #5c</br>
/* #6e is also known as "LATIN SMALL LETTER N" */</br>
escape_sequence : #5c #6e</br>
/* #72 is also known as "LATIN SMALL LETTER R" */</br>
escape_sequence : #5c #72
</code></p>

<h3>boolean, void</h3>
<p>The words <code>boolean</code> and <code>void</code>, respectively, are defined by
<p><code>
boolean : true</br>
boolean : false</br>
true : #74 #72 #75 #65</br>
false : #66 #61 #6c #73 #65</br>
void : #76 #6f # #69 #64
</code></p>

<h3>digit</h3>
<p>The word <code>digit is defined by
<p><code>
digit : /* A single Unicode character from the code point range +U0030 to +U0039. */
</code></p>
