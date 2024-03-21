# IdLib
*IdLib* provides matrix and vector math in C.
IdLib is licensed under the [Zlib license](LICENSE).

## Integration into your CMake project
Create a directory for external dependencies within your project directory.
For example `3rdParty`. Create a folder `IdLib` in that directory.
Download/checkout *IdLib* into that folder such that the contents of the directory
in which this `README.md` file is located in directly reside under `3rdParty/IdLib`.
Add to your project the `3rdParty/IdLib/CMakeLists.txt` using `add_subdirectory`.
Link your target `YourTarget` to *IdLib* using `target_link_libraries(YourTarget idlib)`.
In your C/C++ code, you can include now the functionality of idlib by adding the preprocessor include directive `#include "idlib.h"`.
