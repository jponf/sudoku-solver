#include "solver.hpp"

namespace sudoku
{
    Solver::Solver() : picosat_(::picosat_init())
    { }

}