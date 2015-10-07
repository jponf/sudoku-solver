//
// Author: Josep Pon Farreny
// File: SudokuFormattedOutputter.cpp
//

#include <iostream>

#include "Sudoku.hpp"
#include "SudokuSimpleOutputter.hpp"


namespace sudoku 
{
    SudokuSimpleOutputter::SudokuSimpleOutputter(std::ostream& out_stream)
        : SudokuOutputter(out_stream)
    { }


    SudokuSimpleOutputter::~SudokuSimpleOutputter()
    { }


    void SudokuSimpleOutputter::output(const Sudoku& sudoku)
    {
        std::ostream& os = getStream();

        for (int i = 0; i < Sudoku::NUM_ROWS; ++i) {
            for (int j = 0; j < Sudoku::NUM_COLUMNS; ++j) {
                os << sudoku.getValue(i, j)
                   << (j + 1 < Sudoku::NUM_COLUMNS ? " " : "");
            }
            os << std::endl;
        }
    }
    
}
