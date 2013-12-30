

#ifndef _SUDOKU99_H_
#define _SUDOKU99_H_

#include <vector>
#include <utility>
#include <map>

namespace sudoku
{
    class Sudoku99
    {
    public:
        typedef std::pair<std::pair<int, int>, int> ROWCOLUMNVALUE;

        static const int UNDEFINED_VALUE;
        static const int MIN_VALUE;
        static const int MAX_VALUE;
        static const int NUM_ROWS;
        static const int NUM_COLUMNS;

        // Constructor
        Sudoku99();

        /**
         * \brief Sets the value of the grid cell (row, column).
         *
         * \throw std::out_of_range If any of the input parameters is out of
         *        the valid range. The different ranges are: row [0-9),
         *        column [0, 9), value [1, 9]
         */
        void setValue(int row, int column, int value);

        /**
         * \brief Tries to solve the sudoku with the previous fixed values.
         *
         * \returns true if the a solution is found, false otherwise
         */
        bool solve();


    private:
        void addDontRepeatInColumnConstraints(void);
        //void addNoSameNumberInRowConstraint(PicoSAT*);
        //void addNoSameNumberPerSquareConstraint(PicoSAT*);

        int getLiteralForRowColumnValue(int row, int column, int value);
        void createLiteralForRowColumnValue(const ROWCOLUMNVALUE& rcv);
        const ROWCOLUMNVALUE& getRowColumnValueForLiteral(int literal);

        std::vector< std::vector<int> > grid_;

        int last_literal;
        std::map<ROWCOLUMNVALUE, int> rcv_lit_mapping_;
        std::map<int, ROWCOLUMNVALUE> lit_rcv_mapping_;
    };

}

#endif