//
// Author: Josep Pon Farreny
// File: SudokuOutputter.cpp
//

#include "SudokuOutputter.hpp"


namespace sudoku 
{
    SudokuOutputter::SudokuOutputter(std::ostream& out_stream)
        : out_stream_(out_stream)
    { }


    SudokuOutputter::~SudokuOutputter()
    { }


    std::ostream& SudokuOutputter::getStream()
    {
        return out_stream_;
    }

}
