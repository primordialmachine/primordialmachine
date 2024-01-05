# Primordial Machine
This repository contains the source code of a demo program providing various examples from the field of computer audials and visuals.
This software is licensed under the Mozilla Public License 2.0. 
You can find the license text in the file `LICENSE.md` in this directory.

## Folder Overview
It follows an overview over the folders of this directory
- `player`: The source code of the actual demo program.
- `library`: The source code of various libraries the demo program depends on.
- `website`: The source code of the projects website.
- `plugins`: The source code of dynamic libraries for font loading and image saving and loading.

For more information, consult the `README.md` file of the respective folder.

## Building the Program under Windows 11/Visual Studio Community 2023

### Prerequisites

#### KitWare's CMake
Environment-specific build files for C libraries and programs are generated by KitWare's CMake.

Ensure KitWare's CMake is installed on your system and is in your path.
To assert CMake is properly installed, open a console and enter `cmake --help`. The program should be found and print its help text.

#### Web Server
For self-hosting the documentation of Primordial Machine a web server (e.g., Apache) with PHP 8.2 or newer is required.

### Building
- Check out this repository to some folder. Henceforth we will denote that folder by the placeholder name `<source>`.
- Create a folder outside of `<source>`. Henceforth, we will denote that folder by the placeholder name `<build>`.
- To generate the Visual C++ project files in the folder `<build>`, open a console, enter the folder `<build>` and enter `cmake <source>`.

The above will use the x64 of x86 generator depending on your system.
To specifically choose one generator, enter `cmake -A x64 <source>` for x64 and `cmake -A Win32 <source>` for x86.

### Running Tests
The project's libraries ship with an increasing amount of automated tests.
The tests of a particular library have a name of the form `<library-name>.tests`
where `<library-name>` is the name of the library and `<test-name>` is the name of the respective test.

### Viewing the Documentation
The documentation of Primordial Machine is provided as a website which resides under `website/primordialmachine`.
Visit https://primordialmachine.com/ to read the documentation online.

## CI/CD
[![master branch build status](https://ci.appveyor.com/api/projects/status/x7jiybp3h9v65vkf/branch/master?svg=true)](https://ci.appveyor.com/project/primordialmachine/primordialmachine-develop/branch/master)
[![develop branch build status](https://ci.appveyor.com/api/projects/status/x7jiybp3h9v65vkf/branch/develop?svg=true)](https://ci.appveyor.com/project/primordialmachine/primordialmachine-develop/branch/develop)
