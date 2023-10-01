<?php
defined('SITE_PRIVATE') || die("no direct access");
?>
<h2>Grammars</h2>
<p>This section describes context-free grammars used in this specification to define the lexical and syntactical structure of a program.</p>

<h3>Context-free Grammars</h3>
<p>A context-free grammar consists of a number of productions. Each production has an abstract symbol called a nonterminal as its left-hand side, and a sequence of one or more nonterminal and terminal symbols as its right-hand side.
For each grammar, the terminal symbols are drawn from a specified alphabet.</p>

<p>Starting from a sentence consisting of a single distinguished nonterminal, called the goal symbol, a given context-free grammar specifies a language, namely,
the set of possible sequences of terminal symbols that can result from repeatedly replacing any nonterminal in the sequence with a right-hand side of a production
for which the nonterminal is the left-hand side.</p>

<h3>Lexical Grammar</h3>
<p>A lexical grammar for the DDL language has its terminal symbols the characters of the Unicode character set.
It defines a set of productions, starting from the goal symbol `word`, that describe how sequences of Unicode characters are
translated into a sequence of words. Only UTF-8 sequences of length 1 are support in version 1 of this language.</p>

<h3>Syntactical Grammar</h3>
<p>A syntactical grammar for the <em>Data Definition Language</em> has its terminal symbols the words defined by the lexical grammar.
It defines a set of productions, starting from the goal symbol `sentence`, that describes how sequences of words
are translated into a sentence.</p>

<h3>Grammar Notation</h3>
<p>Productions are written in <code>fixed width</code> fonts.</p>

<p>
A production is defined by its left-hand side, followed by a colon <code>:</code>, followed by its right-hand side definition.
The left hand side is the name of the non-terminal defined by the production.
</p>

<p>
Multiple alternating definitions of a production may be defined.
</p>

<p>
The right hand side of a production consists of any sequence of terminals and non-terminals.
</p>

<p>
In certain cases the right-hand side is replaced by a comment describing the right-hand.
This comment is opened by <code>/*</code> and closed by <code>*/</code>.
</p>

<div class="example">
<p>Example:</p>
<p><code>
digit : /* A single Unicode character from the code point range +U0030 to +U0039. */
</code></p>
</div>

<!------------------------------------------------------------------------------------------------------------------------------------------------------------>
<p>
A terminal is a sequence of Unicode symbols.
A Unicode symbol is denoted a shebang (<code>#</code> followed by a hexadecimal number denoting its code point.
</p>

<div class="example">
<p>Example:</p>

<p>
The following productions denote the non-terminal for a sign as used in the definitions of numerals:
</p>

<p><code>
/* #2b is also known as "PLUS SIGN" */</br>
plus_sign : #2b</br>
/* #2d is also known as "MINUS SIGN" */</br>
minus_sign : #2d
sign : plus_sign</br>
sign : minus_sign
</code></p>

</div>

<!------------------------------------------------------------------------------------------------------------------------------------------------------------>
<p>
The syntax <code>{x}</code> on the right-hand side of a production denotes zero or more occurrences of <code>x</code>.
</p>

<div class="example">
<p>Example:</p>
<p>
The following production defines a possibly empty sequence of digits
</p>

<p><code>
zero-or-more-digits : {digit}
</code></p>

</div>

<!------------------------------------------------------------------------------------------------------------------------------------------------------------>
<p>
The syntax <code>[x]</code> on the right-hand side of a production denotes zero or one occurrences of <code>x</code>.
</p>

<div class="example">
<p>Example:</p>
<p>
The following productions denotes a possible definition of an integer numeral.
It consists of an optional <code>sign</code> followed by a (with <code>sign</code> and <code>zero-or-more-digits</code> as defined in the preceeding examples):
</p>

<p><code>
integer : [sign] digit zero-or-more-digits
</code></p>

</div>

<!------------------------------------------------------------------------------------------------------------------------------------------------------------>
<p>
The empty string is denoted by <code>&epsilon;</code>.
</p>

<div class="example">
<p>Example:</p>
<p>
The following productions denotes a possibly empty list of integers
(with <code>integer</code> as defined in the preceeding example).
Note that this list may include a trailing comma.
</p>

<p><code>
integer-list : integer integer-list-rest</br>
integer-list : &epsilon;</br>
</br>
integer-list-rest : comma integer integer-list-rest</br>
integer-list-rest : comma</br>
integer-list-rest : &epsilon;</br>
</br>
/* #2c is also known as "COMMA" */</br>
comma : #2c
</code></p>

</div>
