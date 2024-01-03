"Cdoc" is a documentation format used by the documentation website of this project.

Cdoc parses JSON files. Each parsed JSON file must contain one "Cdoc entry".
A "Cdoc entry" provides the documentation of a library element (e.g., a struct, a preprocessor define, an enumeration, ...).
A "Cdoc entry" is a JSON object. 

The fields of a "Cdoc entry" are as follows:

- `indices` must be either undefined or a `string` or a list of strings.
   Each string denotes an index to which the entry is added to.

- `name` must be a `string`.
   For entries of type `symbolic-constant`, `typedef`, and `function` this denotes the name of the definition.
   *We should rename scalar-typedef.`

- `id` must be a `string`.
   This denotes the unique ID of the entry.
   The ID must be unique wrt to all entries in all CDoc entry files.
