<?php
defined('SITE_PRIVATE') || die("no direct access");
?>
<h2>Introduction</h2>
<p>
This document is the manual for version 1 of the <name>Data Definition Language</name>.
Programs of this language are descriptions of data as text for the purpose of storing and exchanging that data between entities (humans and machines alike).
For the purpose of describing such data, the language offers built-in data types including scalar values (boolean, number, string, void) as well as aggregate values (maps and lists).
</p>

<p><b>Remark</b> The language is inspired by <name>JSON</name> (see
<a href="https://www.ecma-international.org/publications-and-standards/standards/ecma-404/">
  ECMA-404: The JSON data interchange syntax,2nd edition, December 2017
</a>
for more information), however, it is neither designed as a super- or subset of <name>JSON</name>.
However, a conversion between those two is possible without a loss of data.
</p>
