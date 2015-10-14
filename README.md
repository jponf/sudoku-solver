sudoku-solver
=============

Simple 9x9 sudoku solver

This program uses a SAT solver to solve classical 9x9 sudokus, the
selected SAT solver is PicoSAT, which can be downloaded from the
authors' web page - http://fmv.jku.at/picosat/.

As it is, Picosat source code, written in C, does not integrate well
with C++ code (it is necessary to add extern "C" in picosat.h), for
this reason a modified version of PicoSAT 960 is incuded inside the
"lib" directory.

# Build

To make it easy on Unix-like operating systems, the project comes
with a simple Makefile that handles the compilation and linking of
the code. By default this Makefile looks for the PicoSAT library
included into "lib", but does not build it automatically.

Step by step (in a unix-like O.S), you must first build the PicoSAT
library, to do so, open a terminal and move to the PicoSAT source 
directory and type:

> ./configure && make

After PicoSAT has been compiled, just move to the projects main directory and
type:

> make [debug | release]

If no error is reported a list of directories, build/debug/ or build/release/,
should have been created and inside one of these there should be a bin directory
that contains the sudoku-solver binary.
