#include <iostream>
#include <cstdlib>

#include "sudoku99.hpp"

using namespace std;
using namespace sudoku;

// Simple menu to test the application

const int SET_VALUE = 1;
const int SOLVE = 2;
const int PRINT = 3;
const int EXIT = 4;

void printSudoku(const Sudoku99&);

int main(int argc, char *argv[])
{
	Sudoku99 sudoku;
    bool loop = true;
    int opt;

    int row, column, value;
    Solver::SOLVE_RESULT solve_res;

    cout << "1 - Set cell value" << endl;
    cout << "2 - Solve sudoku" << endl;
    cout << "3 - Print sudoku" << endl;
    cout << "4 - Exit" << endl;

    while (loop)
    {
        cout << "$ ";
        cin >> opt;

        switch(opt)
        {
            case SET_VALUE:
                cout << "Please specify the cell (row, column) and the value" << endl;
                cout << ": ";
                cin >> row >> column >> value;
                sudoku.setValue(--row, --column, value);
                break;

            case SOLVE:
                solve_res = sudoku.solve();
                switch(solve_res)
                {
                    case Solver::SATISFIABLE:
                        cout << "Solution found" << endl;
                        break;
                    case Solver::UNSATISFIABLE:
                        cout << "There isn't a solution with the given "
                            "configuration" << endl;
                        break;
                    default:
                        cout << "Unexpected solve result" << endl;
                        break;
                }
                break;

            case PRINT:
                printSudoku(sudoku);
                break;

            case EXIT:
                loop = false;
                break;

            default:
                cout << "Unknown option" << endl;
                break;
        }
    }


	return EXIT_SUCCESS;
}

void printSudoku(const Sudoku99& sudoku)
{
    for (int i = 0; i < Sudoku99::NUM_ROWS; ++i)
    {
        cout << "| ";
        for (int j = 0; j < Sudoku99::NUM_COLUMNS; ++j)
        {
            cout << sudoku.getValue(i, j) << " |";
            if (j != Sudoku99::NUM_COLUMNS - 1)
                cout << " ";
        }
        cout << endl;
    }
}
