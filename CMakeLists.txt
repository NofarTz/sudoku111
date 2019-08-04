cmake_minimum_required(VERSION 3.14)
project(sudoku2_0 C)

set(CMAKE_C_STANDARD 90)
set(GCC_COVERAGE_COMPILE_FLAGS "-fmessage-length=0 -ansi -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )


add_executable(sudoku2_0 main.c Game.c Game.h MainAux.c MainAux.h gameTester.c gameTester.h Solver.c Solver.h)