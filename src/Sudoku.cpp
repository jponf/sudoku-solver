#include <cstring>
#include <ctime>

#include <stdexcept>

#include "Sudoku.hpp"

namespace sudoku
{
    // Constants
    const int Sudoku::UNDEFINED_VALUE = 0;
    const int Sudoku::MIN_VALUE = 1;
    const int Sudoku::MAX_VALUE = 9;
    const int Sudoku::NUM_ROWS = 9;
    const int Sudoku::NUM_COLUMNS = 9;
    const int Sudoku::SUBREGION_NUM_ROWS = 3;
    const int Sudoku::SUBREGION_NUM_COLUMNS = 3;

    // Constructor
    Sudoku::Sudoku()
        : grid_(NULL),
          solver_(::time(NULL)),  // Randomly initialize the solver
          last_literal(0),
          rcv_lit_mapping_(),
          lit_rcv_mapping_()
    {
        grid_ = new int*[NUM_ROWS]();
        for (int i = 0; i < NUM_ROWS; ++i)
        {
            grid_[i] = new int[NUM_COLUMNS];
            ::memset(grid_[i], UNDEFINED_VALUE, NUM_COLUMNS * sizeof(int));
        }
    }

    // Destructor
    Sudoku::~Sudoku()
    {
        for (int i = 0; i < NUM_ROWS; ++i)
        {
            delete[] grid_[i];
        }
        delete[] grid_;
    }

    // Set cell value
    void Sudoku::setValue(int row, int column, int value)
    {
        if (row < 0 || row >= NUM_COLUMNS)
            throw std::out_of_range("The row must be in the range[0, 9)");
        if (column < 0 || column >= NUM_COLUMNS)
            throw std::out_of_range("The column must be in the range [0, 9)");
        if (value < MIN_VALUE || value > MAX_VALUE)
            throw std::out_of_range(
                "The grid cell value must be in the range [1, 9]");

        grid_[row][column] = value;
    }

    // Get cell value
    int Sudoku::getValue(int row, int column) const
    {
        if (row < 0 || row >= NUM_COLUMNS)
            throw std::out_of_range("The row must be in the range[0, 9)");
        if (column < 0 || column >= NUM_COLUMNS)
            throw std::out_of_range("The column must be in the range [0, 9)");

        return grid_[row][column];
    }

    // Tries to solve the grid, returns true if a solution is found
    Solver::SOLVE_RESULT Sudoku::solve()
    {
        addOnlyOneValuePerCellConstraints();
        addDontRepeatInColumnConstraints();
        addDontRepeatInRowConstraints();
        addDontRepeatInSubRegionConstraints();
        addFixedValuesConstraints();

        Solver::SOLVE_RESULT res = solver_.solve();

        if (res == Solver::SATISFIABLE)
            setGridFromSolverProof();

        return res;
    }


    //
    // Private
    //
    void Sudoku::addOnlyOneValuePerCellConstraints(void)
    {
        std::vector<int> literals(MAX_VALUE, 0);

        for (int i = 0; i < NUM_ROWS; ++i)
        {
            for (int j = 0; j < NUM_COLUMNS; ++j)
            {
                for (int vn = MIN_VALUE; vn <= MAX_VALUE; ++vn)
                {
                    literals[vn-MIN_VALUE] =
                        getLiteralForRowColumnValue(i, j, vn);
                }
                solver_.addExactlyOneConstraint(literals);
            }
        }
    }

    void Sudoku::addDontRepeatInColumnConstraints(void)
    {
        std::vector<int> literals(NUM_ROWS, 0);
        // All possible values per cell
        for (int vn = MIN_VALUE; vn <= MAX_VALUE; ++vn)
        {
            for (int j = 0; j < NUM_COLUMNS; ++j)
            {
                for (int i = 0; i < NUM_ROWS; ++i)
                {
                    literals[i] = getLiteralForRowColumnValue(i, j, vn);
                }
                solver_.addExactlyOneConstraint(literals);
            }
        }
    }

    void Sudoku::addDontRepeatInRowConstraints(void)
    {
        std::vector<int> literals(NUM_COLUMNS, 0);
        // All possible values per cell
        for (int vn = MIN_VALUE; vn <= MAX_VALUE; ++vn)
        {
            for (int i = 0; i < NUM_ROWS; ++i)
            {
                for (int j = 0; j < NUM_COLUMNS; ++j)
                {
                    literals[j] = getLiteralForRowColumnValue(i, j, vn);
                }
                solver_.addExactlyOneConstraint(literals);
            }
        }
    }

    void Sudoku::addDontRepeatInSubRegionConstraints(void)
    {
        std::vector<int> literals(
            SUBREGION_NUM_ROWS * SUBREGION_NUM_COLUMNS, 0);

        for (int nv = MIN_VALUE; nv <= MAX_VALUE; ++nv)
        {
            for (int si = 0; si < NUM_ROWS; si += SUBREGION_NUM_ROWS)
            {
                for (int sj = 0; sj < NUM_COLUMNS;
                     sj += SUBREGION_NUM_COLUMNS)
                {
                    int lit_index = -1;
                    for (int i = si; i < si + SUBREGION_NUM_ROWS; ++i)
                    {
                        for (int j = sj; j < sj + SUBREGION_NUM_COLUMNS; ++j)
                        {
                            literals[++lit_index] =
                                getLiteralForRowColumnValue(i, j, nv);
                        }
                    }
                    solver_.addExactlyOneConstraint(literals);
                }
            }
        }
    }

    void Sudoku::addFixedValuesConstraints(void)
    {
        for (int i = 0; i < NUM_ROWS; ++i)
        {
            for (int j = 0; j < NUM_COLUMNS; ++j)
            {
                if (grid_[i][j] != UNDEFINED_VALUE)
                {
                    int literal = getLiteralForRowColumnValue(
                        i, j, grid_[i][j]);
                    solver_.assumeLiteral(literal);
                }
            }
        }
    }

    void Sudoku::setGridFromSolverProof(void)
    {
        std::map<int, ROWCOLUMNVALUE>::iterator map_it, end_it;
        end_it = lit_rcv_mapping_.end();

        for(map_it = lit_rcv_mapping_.begin(); map_it != end_it; ++map_it)
        {
            int literal = map_it->first;
            const ROWCOLUMNVALUE& rcv = map_it->second;
            Solver::LITERAL_VALUE lit_val = solver_.getLiteralValue(literal);
            if (lit_val == Solver::TRUE)
            {
                int row = rcv.first.first;
                int column = rcv.first.second;
                int value = rcv.second;
                grid_[row][column] = value;

                // std::cout << "(" << row << ", " << column << "): " << value << std::endl;
                // std::cout << grid_[row][column] << std::endl;
            }
            // std::cout << literal << " = " << lit_val << std::endl;

        }
    }

    int Sudoku::getLiteralForRowColumnValue(int row, int column, int value)
    {
        ROWCOLUMNVALUE rcv = std::make_pair(std::make_pair(row, column), value);

        if (rcv_lit_mapping_.count(rcv) < 1)
            createLiteralForRowColumnValue(rcv);

        return rcv_lit_mapping_[rcv];
    }

    void Sudoku::createLiteralForRowColumnValue(const ROWCOLUMNVALUE& rcv)
    {
        int new_lit = last_literal + 1;
        rcv_lit_mapping_[rcv] = new_lit;
        lit_rcv_mapping_[new_lit] = rcv;
        last_literal = new_lit;
    }

    const Sudoku::ROWCOLUMNVALUE& Sudoku::getRowColumnValueForLiteral(
        int literal)
    {
        return lit_rcv_mapping_[literal];
    }




}
