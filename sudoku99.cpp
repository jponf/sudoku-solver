#include <string>
#include <stdexcept>

#include "sudoku99.hpp"
#include "lib/picosat.h"

namespace sudoku
{
    // Constants
    const int Sudoku99::UNDEFINED_VALUE = -1;
    const int Sudoku99::MIN_VALUE = 1;
    const int Sudoku99::MAX_VALUE = 9;
    const int Sudoku99::NUM_ROWS = 9;
    const int Sudoku99::NUM_COLUMNS = 9;
    const int Sudoku99::SUBREGION_NUM_ROWS = 3;
    const int Sudoku99::SUBREGION_NUM_COLUMNS = 3;

    // Constructor
    Sudoku99::Sudoku99()
        : grid_(NUM_ROWS, std::vector<int>(NUM_COLUMNS, UNDEFINED_VALUE)),
          rcv_lit_mapping_(),
          lit_rcv_mapping_(),
          last_literal(0)
    { }

    // Set cell value
    void Sudoku99::setValue(int row, int column, int value)
    {
        if (row < 0 || row >= NUM_COLUMNS)
            throw std::out_of_range(
                std::string("The row must be in the range[0, 9)"));
        if (column < 0 || column >= NUM_COLUMNS)
            throw std::out_of_range(
                std::string("The column must be in the range [0, 9)"));
        if (value < MIN_VALUE || value > MAX_VALUE)
            throw std::out_of_range(
                "The grid cell value must be in the range [1, 9]");

        grid_[row][column] = value;
    }

    // Tries to solve the grid, returns true if a solution is found
    Solver::SOLVE_RESULT Sudoku99::solve()
    {
        addDontRepeatInColumnConstraints();
        addDontRepeatInRowConstraints();
        addDontRepeatInSubRegionConstraints();

        Solver::SOLVE_RESULT res = solver_.solve();

        return res;
    }


    //
    // Private
    //

    void Sudoku99::addDontRepeatInColumnConstraints(void)
    {
        std::vector<int> literals(NUM_ROWS, 0);
        // All possible values per cell
        for (int vn = 1; vn <= MAX_VALUE; ++vn)
        {
            for (int j = 0; j <= NUM_COLUMNS; ++j)
            {
                for (int i = 0; i <= NUM_ROWS; ++i)
                {
                    literals[i] = getLiteralForRowColumnValue(i, j, vn);
                }
                solver_.addExactlyOneConstraint(literals);
            }
        }
    }

    void Sudoku99::addDontRepeatInRowConstraints(void)
    {
        std::vector<int> literals(NUM_ROWS, 0);
        // All possible values per cell
        for (int vn = 1; vn <= MAX_VALUE; ++vn)
        {
            for (int i = 0; i <= NUM_ROWS; ++i)
            {
                for (int j = 0; j <= NUM_COLUMNS; ++j)
                {
                    literals[i] = getLiteralForRowColumnValue(i, j, vn);
                }
                solver_.addExactlyOneConstraint(literals);
            }
        }
    }

    void Sudoku99::addDontRepeatInSubRegionConstraints(void)
    {
        std::vector<int> literals(
            SUBREGION_NUM_ROWS * SUBREGION_NUM_COLUMNS, 0);

        for (int nv = 1; nv <= MAX_VALUE; ++nv)
        {
            for (int si = 0; si <= NUM_ROWS; si += SUBREGION_NUM_ROWS)
            {
                for (int sj = 0; sj <= NUM_COLUMNS;
                     sj += SUBREGION_NUM_COLUMNS)
                {
                    for (int i = si; i < SUBREGION_NUM_ROWS; ++i)
                    {
                        for (int j = sj; j < SUBREGION_NUM_COLUMNS; ++j)
                        {
                            literals[i * SUBREGION_NUM_ROWS + j] = 
                                getLiteralForRowColumnValue(i, j, nv);
                        }
                    }
                    solver_.addExactlyOneConstraint(literals);
                }
            }
        }
    }

    int Sudoku99::getLiteralForRowColumnValue(int row, int column, int value)
    {
        ROWCOLUMNVALUE rcv = std::make_pair(std::make_pair(row, column), value);

        if (rcv_lit_mapping_.count(rcv) < 1)
            createLiteralForRowColumnValue(rcv);

        return rcv_lit_mapping_[rcv];
    }

    void Sudoku99::createLiteralForRowColumnValue(const ROWCOLUMNVALUE& rcv)
    {
        int new_lit = last_literal + 1;
        rcv_lit_mapping_[rcv] = new_lit;
        lit_rcv_mapping_[new_lit] = rcv;
        last_literal = new_lit;
    }

    const Sudoku99::ROWCOLUMNVALUE& Sudoku99::getRowColumnValueForLiteral(
        int literal)
    {
        return lit_rcv_mapping_[literal];
    }




}