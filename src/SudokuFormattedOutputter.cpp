//
// Author: Josep Pon Farreny
// File: SudokuFormattedOutputter.cpp
//

#include <iostream>

#include "Sudoku.hpp"
#include "SudokuFormattedOutputter.hpp"


namespace sudoku 
{
    SudokuFormattedOutputter::SudokuFormattedOutputter(
            std::ostream& out_stream)
        : SudokuOutputter(out_stream)
    { }


    SudokuFormattedOutputter::~SudokuFormattedOutputter()
    { }


    void SudokuFormattedOutputter::output(const Sudoku& sudoku)
    {
        std::ostream& os = getStream();

        for (int i = 0; i < Sudoku::NUM_ROWS; ++i) {
            outputRowSeparator(i);  // row region Separator

            for (int j = 0; j < Sudoku::NUM_COLUMNS; ++j) {
                outputColumnSeparator(j);
                int value = sudoku.getValue(i, j);
                if (value == Sudoku::UNDEFINED_VALUE) {
                    os << "   ";
                } else {
                    os << " " << value << " ";
                }
            }

            outputColumnSeparator(Sudoku::NUM_COLUMNS);
            os << std::endl;
        }

        outputRowSeparator(Sudoku::NUM_ROWS);  // last row Separator
    }


    // ------------------------------------------------------------------------
    // Private functions

    void SudokuFormattedOutputter::outputRowSeparator(int row) {
        std::ostream& os = getStream();
        std::string sep = (row % Sudoku::SUBREGION_NUM_ROWS) ? "---" : "===";

        for (int i = 0; i < Sudoku::NUM_COLUMNS; ++i) {
            outputColumnSeparator(i);
            os << sep;
        }
        outputColumnSeparator(Sudoku::NUM_COLUMNS);
        os << std::endl;
    }


    void SudokuFormattedOutputter::outputColumnSeparator(int column) {
        std::ostream& os = getStream();
        os << (column % Sudoku::SUBREGION_NUM_COLUMNS ? "|" : "‖");
    }
    
}
