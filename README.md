# CarSim2000

## Description :triumph:

:100:

![](https://media.giphy.com/media/42C7eGLHbijtK/giphy.gif)

:100:

A simple ncurses based application made as a part of my university assignment.

It "simulates" cars moving in a system of roads.

Each car is represented by a red rectangle. A car will move randomly on a provided map until it reaches it's (randomly picked) destination.

The goal of the assignment was to create a multi-threaded application in C++ and so each car has it's own thread.

To make the application a bit more unique I've added the option for the user to provide a map in a text file, refer to Format section for more information.

## Compilation :joy:
### Requirements:
* cmake version 3.7 or higher
* a compiler that supports c++14
* ncurses library

### Steps:
1. Execute `cmake CMakeLists.txt`
2. Execute `make`
3. All done!

## Execution :heart_eyes:
The resulting binary will be located in the `bin` directory.

It can be executed without any arguments `./bin/CarSim`.

Or one can override the default map by specifying it in the first argument, for example: `./bin/CarSim bin/maps/map2.txt`

## Modifications :astonished:
As mentioned earlier the program can take maps as an optional argument:

The format of said maps is as follows:

### Format:

| Symbol | Meaning                    |
|--------|----------------------------|
| #      | Intersection               |
| >      | Right-directional road     |
| <      | Left-directinoal road      |
| v      | Downwards-directional road |
| ^      | Upwards-directional road   |
| !      | Spawn point                |
| @      | Destination point          |
| Other  | Wall                       |

The file must contain **only the map**, no empty lines, comments, etc. are allowed.

The map must be rectangular.

There's no validation implemented:
* The application might crash.
* Cars might clump together and block each other, halting the simulation.

### Example [:poop:](/maps/map2.txt):
```
................................................................
.#<<<<<<<<<<<#<<<<<<<<<<#<<<<<<<<<<<<#<#<<<<<<#<<<<<<<<#<<<<<<#.
.v...........^..........v............v.^......^........v......^.
.v...........#<!<<<<<<@<#............v.^......#<!<<<<@<#......^.
.v...................................v.^......................^.
.#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<#<#<#<#<<<<<<<<<<<<<<<<<<<<#.
.v.................................v.....^....................^.
.#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>#>#>#>#>>>>>>>>>>>>>>>>>>>>#.
.v...................................v.^......................^.
.v...................................v.^......................^.
.v...................................v.^......................^.
.v...........#>@>>>>>>!>#............v.^......#>@>>>>!>#......^.
.v...........^..........v............v.^......^........v......^.
.#>>>>>>>>>>>#>>>>>>>>>>#>>>>>>>>>>>>#>#>>>>>>#>>>>>>>>#>>>>>>#.
................................................................
```