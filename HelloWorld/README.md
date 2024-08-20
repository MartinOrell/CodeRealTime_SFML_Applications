## helloSFML
This is a project to showcase using SFML for graphics in a code realTime application. SFML is used by a user library called *gui*. gui is then used by a capsule.

### Building gui
Before using gui which contains SFML, the library needs to be built. Use a terminal inside the gui folder to first generate the makefiles and then build the library
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build/
```
### building project
After the gui library is built, then you can compile the project as normal with the .tcjs file.
### add dll to executable

Before you can run, the dll files from SFML needs to be added to the same folder as the executable. The files are found in helloSFML/gui/build/bin. The files should go to helloSFML/HelloSFML_target/default

Then the program should be able to be run.

### misc
This is the content inside the .tcjs file specifically used to be able to run SFML.

```
tc.userLibraries = [
    "../../gui/build/libgui.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-audio.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-graphics.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-main.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-network.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-system.a",
    "../../gui/build/_deps/sfml-build/lib/libsfml-window.a"
];
tc.inclusionPaths = [
    "../../gui/include",
    "../../gui/build/_deps/sfml-src/include"
]
tc.compileArguments = "-DSFML_STATIC"
```