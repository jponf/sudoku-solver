
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

        enum SOLVE_RESULT { UNSATISFIABLE, SATISFIABLE, UNKNOWN };
        enum LITERAL_VALUE { FALSE, TRUE, UNDEFINED };

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
         * \brief Adds a restriction to force the solver assume an specified
         *        value for the given literal.
         *
         * F.E: 1, assumes 1 must be true. -1, assumes 1 must be false;
         */
        void assumeLiteral(int literal);

        /**
         * \brief Returns the value of the specified literal after a calling 
         *        solve()
         *
         * This function only makes sence if the last call to solve has returned
         * SATISFIABLE.
         *
         * \throw logic_error If the last call to solve hasn't been SATISFIABLE.
         *
         * \return The value of the literal { TRUE, FALSE, UNKNOWN }
         */
        LITERAL_VALUE getLiteralValue(int literal) const;

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