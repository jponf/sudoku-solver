
#ifndef _SUDOKU_OUTPUTTER_HPP_
#define _SUDOKU_OUTPUTTER_HPP_

#include <iosfwd>

#include "sudoku.hpp"

namespace sudoku
{
    class SudokuOutputter
    {
    public:
        // construct/destroy
        SudokuOutputter(std::ostream& output_stream);
        virtual ~SudokuOutputter();

        virtual void output(const Sudoku& sudoku) = 0;

    protected:
        std::ostream& getStream();

    private:

        // disabled methods, declared private and not implemented
        SudokuOutputter();
        SudokuOutputter(const SudokuOutputter&);
        SudokuOutputter& operator=(const SudokuOutputter&);

        // attributes
        std::ostream& out_stream_;
    };
}

#endif // _SUDOKU99_OUTPUTTER_HPP_
