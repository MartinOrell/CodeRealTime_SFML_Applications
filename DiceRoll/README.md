## DiceRoll
This project features a die that can be rolled by either pressing space bar or clicking anywhere on screen. Before building the project the gui library needs to be built first.
### Building gui
Use a terminal inside the gui folder to first generate the makefiles and then build the library
```
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build/
```
### building project
After the gui library is built, then you can compile the project as normal with the .tcjs file.