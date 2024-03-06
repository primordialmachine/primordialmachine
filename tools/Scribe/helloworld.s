# Copyright (c) 2024 Michael Heilmann. All rights reserved.

# there is only the metadata primitive 'string'.
# A metadata string starts with an exclamation mark
# followed by a sequence of characters surrounded
# by double quotes.

# Scribe supports metadata.
# Metadata values are introduced by an ampersand followed by the name of the metadata.
# If the same name is used twice in a file, then this is an error.
# The name is followed by a colon.
# After the color, a metadata value must follow.
# the value can be either
# - a string,
# - a boolean value,
# - a void value,
# - a map value,
# - or a list value.

# The following metadata is used to describe this very file.
# The metadata is OPTIONAL.
@file : {
  author : {
    name : "Michael Heilmann",
    email : "michaelheilmann@primordialmachine.com",
  }
}

# The following metadata is used to determine the compiler.
# The metada is MANDATORY.
# The fields "name" and "version" are required.
@language : {
  name : "Low Level Language",
  version : "1.0",
  authors : [
    { name : "Michael Heilmann", email: "michaelheilmann@primordialmachine.com" },
    { name : "John Doe", email : "johndoe@primordialmachine.com" },
    { name : "Jane Doe", email : "janedoe@primordialmachine.com" },
  ],
}
