

#ifndef _SUDOKU99_H_
#define _SUDOKU99_H_

#include <map>
#include <vector>
#
#include <utility>

#include "solver.hpp"

namespace sudoku
{
    class Sudoku
    {
    public:
        typedef std::pair<std::pair<int, int>, int> ROWCOLUMNVALUE;

        static const int UNDEFINED_VALUE;
        static const int MIN_VALUE;
        static const int MAX_VALUE;
        static const int NUM_ROWS;
        static const int NUM_COLUMNS;
        static const int SUBREGION_NUM_ROWS;
        static const int SUBREGION_NUM_COLUMNS;

        // Constructor
        Sudoku();

        // Destructor
        virtual ~Sudoku();

        /**
         * \brief Sets the value of the grid's cell (row, column).
         *
         * \throw std::out_of_range If any of the input parameters is out of
         *        the valid range. The different ranges are: row [0-9),
         *        column [0, 9), value [1, 9]
         */
        void setValue(int row, int column, int value);

        /**
         * \brief Gets the value of the grid's cell (row, column).
         *
         * \returns The cell value or UNDEFINED_VALUE if it is unspecified.
         *
         * \throw std::out_of_range If the (row, column) is out of the grid.
         */
        int getValue(int row, int column) const;

        /**
         * \brief Tries to solve the sudoku with the previous fixed values.
         *
         * \returns true if a solution is found, false otherwise
         */
        Solver::SOLVE_RESULT solve();


    private:
        void addOnlyOneValuePerCellConstraints(void);
        void addDontRepeatInColumnConstraints(void);
        void addDontRepeatInRowConstraints(void);
        void addDontRepeatInSubRegionConstraints(void);
        void addFixedValuesConstraints(void);
        void setGridFromSolverProof(void);

        int getLiteralForRowColumnValue(int row, int column, int value);
        void createLiteralForRowColumnValue(const ROWCOLUMNVALUE& rcv);
        const ROWCOLUMNVALUE& getRowColumnValueForLiteral(int literal);

        int **grid_;

        Solver solver_;

        int last_literal;
        std::map<ROWCOLUMNVALUE, int> rcv_lit_mapping_;
        std::map<int, ROWCOLUMNVALUE> lit_rcv_mapping_;
    };

}

#endif