# forestFire :evergreen_tree: :fire: : a cellular automaton 
## Introduction
A C++ implementation of [the cellular automata](http://cormas.cirad.fr/fr/applica/fireautomata.htm) developed by the CORMAS. (Only the first model is curently available.)
## Installation
This program have only be tested on Ubuntu 20.10. 
### Linux
#### Installing dependencies

This program depends on:
 * a C++ compiler (g++, clang, ...) 
 * CMake 3.10
 * SDL2 

```
apt update
apt upgrade
apt install g++
apt install libsdl2-2.0-0 libsdl2-dev
snap install cmake
```

#### Building the program

```
mkdir build
cd build/
cmake ..
```

### Windows

:construction: Work in progress.

## Commands

<kbd> left mouse click </kbd> : Set a tile on fire
<kbd> p </kbd> or <kbd> enter </kbd> : Play / Pause 
<kbd> esc </kbd> : Stop

## Sources
Christophe Le Page, François Bousquet, Pierre Bommel, *Cellular automata of a forest fire*, http://cormas.cirad.fr/en/applica/fireautomata.htm
