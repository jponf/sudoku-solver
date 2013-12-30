#include <exception>

#include "sudoku99.h"
#include "lib/picosat.h"

namespace sudoku
{
    // Constants
    const int Sudoku99::UNDEFINED_VALUE = -1;
    const int Sudoku99::MIN_VALUE = 1;
    const int Sudoku99::MAX_VALUE = 9;
    const int Sudoku99::NUM_ROWS = 9;
    const int Sudoku99::NUM_COLUMNS = 9;

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
        // if (row < 0 || row >= NUM_COLUMNS)
        //     throw std::out_of_range("The row must be in the range[0, 9)");
        // if (column < 0 || column >= NUM_COLUMNS)
        //     throw std::out_of_range("The column must be in the range [0, 9)");
        // if (value < MIN_VALUE || value > MAX_VALUE)
        //     throw std::out_of_range(
        //         "The grid cell value must be in the range [1, 9]");

        grid_[row][column] = value;
    }

    // Tries to solve the grid, returns true if a solution is found
    bool Sudoku99::solve()
    {
        PicoSAT* picosat = ::picosat_init();

        // All possible values per cell
        for (int vn = 1; vn <= MAX_VALUE; ++vn) 
        {
            for(int j = 0; j <= NUM_COLUMNS; ++j)
            {
                for(int i = 0; i <= NUM_ROWS; ++i)
                {

                }
            }           
        }

        ::picosat_reset(picosat);

        return false;
    }

    //
    // Private
    //

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