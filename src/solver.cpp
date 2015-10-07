#include <stdexcept>

#include "solver.hpp"

namespace sudoku
{
    const int Solver::DEF_DECISION_LIMIT = 1000;

    Solver::Solver()
        : picosat_(::picosat_init())
    { }

    Solver::~Solver()
    {
        if (picosat_ != NULL)
            ::picosat_reset(picosat_);
    }

    //--------

    void Solver::clear()
    {
        if (picosat_ != NULL)
            ::picosat_reset(picosat_);
        picosat_ = ::picosat_init();
    }

    Solver::SOLVE_RESULT Solver::solve(int decision_limit)
    {
        int res = picosat_sat(picosat_, decision_limit);
        // 'PICOSAT_UNSATISFIABLE', 'PICOSAT_SATISFIABLE', or 'PICOSAT_UNKNOWN'.
        switch(res)
        {
            case PICOSAT_SATISFIABLE:
                return SATISFIABLE;
            case PICOSAT_UNSATISFIABLE:
                return UNSATISFIABLE;
            default:
                return UNKNOWN;
        }
    }

    // Adds the given literals as a clause
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

    // Adds a clause to assume a literal value
    void Solver::assumeLiteral(int literal)
    {
        ::picosat_add_arg(picosat_, literal, 0);
    }

    Solver::LITERAL_VALUE Solver::getLiteralValue(int literal) const
    {
        if (::picosat_res(picosat_) != PICOSAT_SATISFIABLE)
            throw std::logic_error(
                "Solve hasn't been called, or the previous call result has "
                "been UNSATISFIABLE");

        switch (::picosat_deref(picosat_, literal))
        {
            case 1:
                return TRUE;
            case -1:
                return FALSE;
            default:
                return UNDEFINED;
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

        for (size_t i = 0; i < ilim; ++i)
        {
            for (size_t j = i + 1; j < jlim; ++j)
            {
                int lits[2] = { -literals[i], -literals[j] };
                std::vector<int> clause;
                clause.assign(lits, lits + 2);

                addClause(clause);
            }
        }
    }

    void Solver::addExactlyOneConstraint(const std::vector<int>& literals)
    {
        addAtLeastOneConstraint(literals);
        addAtMostOneConstraint(literals);
    }
}