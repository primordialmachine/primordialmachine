<?php

/**
 * The base of all "C documentation" emitters.
 * An emitter emit code (e.g., HTML, LaTeX, Markdown, ...) from "C documentation" files.
 * @author Michael Heilmann
 */
abstract class CdocEmitter {

  /**
   * @brief Emit the code for a CDocFile.
   * @param $file The CdocFile object for which the code is emitted.
   */
  public abstract function emitContentsEntry(CdocFile $file);
  
  /**
   * @brief Emit the code for "C documentation" table of contents entry.
   */
  public abstract function emitTableOfContentsEntry($entry);

}; // // class CdocEmitter

?>
