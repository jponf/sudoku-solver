
#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <vector>

#include "lib/picosat.h"

namespace sudoku
{
    class Solver
    {
    public:
        // Maximum number of decisions before aborting the sat routine.
        // A negative value means no limit.
        static const int DEF_DECISION_LIMIT; 

        enum SOLVE_RESULT { SATISFIABLE, UNSATISFIABLE, UNKNOWN };

        /**
         *
         */
        Solver();

        /**
         *
         */
        virtual ~Solver();

        /**
         * \brief Tries to Solve the defined formula.
         *
         * \return True if the formula is satisfiable, flase otherwise.
         */
        SOLVE_RESULT solve();

        /**
         * \brief Adds the given literals as a clause.
         */
        void addClause(const std::vector<int>& literals);

        /**
         * \brief Adds the necessary constraints to force that at least one of
         *        the literals evaluates to true.
         */
        void addAtLeastOneConstraint(const std::vector<int>& literals);

        /**
         * \brief Adds the necessary constraints to force that only at most one
         *        of the literals evaluates to true.
         */
        void addAtMostOneConstraint(const std::vector<int>& literals);

        /**
         * \brief Adds the necessary constraints to force that exactly one of
         *        the literals evaluates to true.
         */
        void addExactlyOneConstraint(const std::vector<int>& literals);

    private:
        int decision_limit;
        PicoSAT* picosat_;
    };

}

#endif