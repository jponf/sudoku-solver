
#ifndef _SUDOKU_FORMATTED_OUTPUTTER_HPP_
#define _SUDOKU_FORMATTED_OUTPUTTER_HPP_


#include "Sudoku.hpp"
#include "SudokuOutputter.hpp"


namespace sudoku 
{
    class SudokuFormattedOutputter : public SudokuOutputter
    {
    public:
        // construct/destroy
        SudokuFormattedOutputter(std::ostream& out_stream);
        virtual ~SudokuFormattedOutputter();

        void output(const Sudoku&);

    private:

        void outputRowSeparator(int row);
        void outputColumnSeparator(int column);
    };

}

#endif // _SUDOKU_FORMATTED_OUTPUTTER_HPP_
