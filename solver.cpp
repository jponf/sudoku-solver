#include "solver.hpp"

namespace sudoku
{
    const int Solver::DEF_DECISION_LIMIT = 1000;

    Solver::Solver() 
        : picosat_(::picosat_init()),
          decision_limit(DEF_DECISION_LIMIT)
    { }

    Solver::~Solver()
    {
        picosat_reset(picosat_);
    }

    //--------

    Solver::SOLVE_RESULT Solver::solve()
    {
        int res = picosat_sat(picosat_, decision_limit);
        // 'PICOSAT_UNSATISFIABLE', 'PICOSAT_SATISFIABLE', or 'PICOSAT_UNKNOWN'.
        if (res == PICOSAT_SATISFIABLE)
            return SATISFIABLE;
        else if (res == PICOSAT_UNSATISFIABLE)
            return UNSATISFIABLE;
        else
            return UNKNOWN;
    }

    void Solver::addClause(const std::vector<int>& literals)
    {
        if (!literals.empty())
        {
            std::vector<int> clause(literals);
            clause.push_back(0);
            // Get vector as array ()
            int *clause_arr = &clause[0];

            ::picosat_add_lits(picosat_, clause_arr);
        }
    }

    void Solver::addAtLeastOneConstraint(const std::vector<int>& literals)
    {
        addClause(literals);
    }

    void Solver::addAtMostOneConstraint(const std::vector<int>& literals)
    {
        const size_t ilim = literals.size() - 1;
        const size_t jlim = literals.size();

        for (size_t i = 0; i < literals.size() - 1; ++i)
        {
            for (size_t j = i + 1; j < literals.size(); ++j)
            {
                int clause[3] = { -literals[i], -literals[j], 0 };
                ::picosat_add_lits(picosat_, clause);
            }
        }
    }

    void Solver::addExactlyOneConstraint(const std::vector<int>& literals)
    {
        addAtLeastOneConstraint(literals);
        addAtMostOneConstraint(literals);
    }
}