# The Program
The folder, in which this `README.md` is located in, contains the source code and the build and configuration files for a C program.

The program opens a window.

It presents seventeen scenes:

- `day1.empty.adl` An empty scene with a "Capri"-colored background.

- `day2.quadriliteral.adl` A quadriliteral.
  The background is "Capri"-colored.
  The geometric is "Malachite"-colored.

- `day2.triangle.adl` A triangle.
  The background is "Capri"-colored.
  The geometric is "Malachite"-colored.

- `day3.quadriliteral.adl` A quadriliteral.
  The background is "Capri"-colored.
  The geometric is vertex-colored.
  The vertex color is oscillating between colors.
 
- `day3.triangle.adl` A triangle.
  The background is "Capri"-colored.
  The geometric is vertex-colored.
  The vertex color is oscillating between colors.

- `day4.quadriliteral.adl` A quadriliteral.
  The background is "Capri"-colored.
  The geometric is vertex-colored.

- `day4.triangle.adl` A triangle.
  The background is "Capri"-colored.
  The geometric is vertex-colored.
 
- `day5.quadriliteral.adl` A quadriliteral.
  The background is "Capri"-colored.
  The geometric is textured with a checkerboard texture.

- `day5.triangle.adl` A triangle.
  The background is "Capri"-colored.
  The geometric is textured with a checkerboard texture.

- `day6.quadriliteral.adl` A quadriliteral with a checkerboard texture.
  The background is "Capri"-colored.
  The camera rotates around the geometrics.

- `day7.triangle.adl` A triangle with a checkerboard texture.
  The background is "Capri"-colored.
  The camera rotates around the geometrics.

- `day8.cube.adl` A cube with a checkerboard texture.
  The background is "Capri"-colored.
  The camera rotates around the geometrics.

- `day8.octahedron.adl` An octahderon with a checkerboard texture.
  The background is "Capri"-colored.
  The camera rotates around the geometrics.

- `day9.cube.loaded-texture.adl` Two cubes both with a white texture with black lines at the seams of the sides.
  The background is "Capri"-colored.
  The cubes are distributed around the origin.
  The camera rotates around the geometrics.

- `day9.octahedron.loaded-texture.adl` Two octahedra both with a white texture with black lines at the seams of the sides.
  The background is "Capri"-colored.
  The octahedra are distributed around the origin.
  The camera rotates around the geometrics.

To exit the program either close the window or press `Escape` when the window has keyboard focus.
To switch between the two scenes, press `Enter`.

# Building the Program
To compile this program, you generate your environment-specific build files using the KitWare's CMake.

- Ensure KitWare's CMake is installed on your system and is in your path.
  To assert CMake is properly installed, open a console and enter `cmake --help`. The programs should be found and print its help text.
- Check out this repository to some folder. Henceforth we will denote that folder by the placeholder name `<source>`.
- Create a folder outsode of `<source>`. Henceforth, we will denote that folder by the placeholder name `<build>`.
- To generate the Visual C++ project files in the folder `<build>`, open a console, enter the folder `<build>` and enter `cmake <source>`.

The above will use the x64 of x86 generator depending on your system.
To specifically choose one generator, enter `cmake -A x64 <source>` for x64 and `cmake -A Win32 <source>` for x86.
