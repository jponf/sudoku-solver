
#ifndef _SOLVER_H_
#define _SOLVER_H_

#include "lib/picosat.h"

namespace sudoku
{
    class Solver
    {
    public:
        Solver();
    private:
        PicoSAT* picosat_;
    };

}

#endif