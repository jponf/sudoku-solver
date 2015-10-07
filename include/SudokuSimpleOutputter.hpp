
#ifndef _SUDOKU_SIMPLE_OUTPUTTER_HPP_
#define _SUDOKU_SIMPLE_OUTPUTTER_HPP_


#include "Sudoku.hpp"
#include "SudokuOutputter.hpp"


namespace sudoku 
{
    class SudokuSimpleOutputter : public SudokuOutputter
    {
    public:
        // construct/destroy
        SudokuSimpleOutputter(std::ostream& out_stream);
        virtual ~SudokuSimpleOutputter();

        void output(const Sudoku&);

    private:
    };

}

#endif // _SUDOKU_SIMPLE_OUTPUTTER_HPP_
