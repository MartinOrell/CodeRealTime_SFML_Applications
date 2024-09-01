## Race
Before building this project, you need to first build SFML.
### Building SFML
Use a terminal inside the SFML folder to first generate the makefiles and then build the library
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build/
```
### building project
After the gui library is built, then you can compile the project as normal with the .tcjs file.