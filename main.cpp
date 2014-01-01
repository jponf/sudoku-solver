#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <limits>

#include "sudoku99.hpp"

using namespace std;
using namespace sudoku;

//
// Simple main to test the application (Must be changed in the future)
//

// Function prototypes

void printSudoku(const Sudoku99&);
void printHelp(const char* bin_path);
bool readParameters(int argc, char *argv[]);
void readSudoku99FromFile(ifstream&, Sudoku99&);

//

ifstream sudoku_file;
bool verbose = false;


int main(int argc, char *argv[])
{
    bool quit = readParameters(argc, argv);

    if (quit) {
        printHelp(argv[0]);
        return EXIT_FAILURE;
    }

    Sudoku99 sudoku;
    readSudoku99FromFile(sudoku_file, sudoku);

    if (verbose) {
        printSudoku(sudoku);
        cout << endl;
    }

    Solver::SOLVE_RESULT solve_res = sudoku.solve();

    switch(solve_res)
    {
        case Solver::SATISFIABLE:
            cout << "Solution found" << endl << endl;
            printSudoku(sudoku);
            return EXIT_SUCCESS;
        case Solver::UNSATISFIABLE:
            cout << "There isn't a solution with the given configuration"
                << endl;
            return EXIT_FAILURE;
        default:
            cout << "Unexpected solve result" << endl;
            return EXIT_FAILURE;
    }
}

// Reads user command line parameters
bool readParameters(int argc, char* argv[])
{
    if (argc < 2 || argc > 3)
    {
        printHelp(argv[0]);
        return true;
    }

    bool quit = true;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) {
            return true;
        } else if(strcmp("-v", argv[i]) == 0) {
            verbose = true;
        } else {
            sudoku_file.open(argv[i]);
            if (!sudoku_file.is_open() || !sudoku_file.good())
            {
                cerr << "Error opening the sudoku configuration file" << endl;
            } else {
                quit = false;
            }

        }
    }
    return quit;
}

void printHelp(const char* bin_path)
{
    cout << "Usage: " << bin_path << " [Options]" << endl;
    cout << endl;
    cout << "\tOptions:" << endl;
    cout << "\t\t-h/--help\tprint this message and exits." << endl;
    cout << "\t\t-v\t\tprints execution information." << endl;
    cout << "\t\tsudoku_file\tfile with the sudoku initial values." << endl;
    cout << endl;
    cout << "sudoku_file format:" << endl;
    cout << endl;
    cout << "\tThe sudoku grid starts at the top left cell (1, 1)" << endl;
    cout << "\tand finishes at the bottom right cell (9, 9)." << endl << endl;
    cout << "\tThe file must be composed by lines with the following format:" << endl;
    cout << "\t\t<row> <column> value" << endl;
}

//------------------------------------------------------------------------------

void readSudoku99FromFile(ifstream& ifs, Sudoku99& sudoku)
{
    int row, column, value;
    int line = 0;

    while (ifs.good())
    {
        ifs >> row >> column >> value;
        ++line;

        if ( (ifs.bad() || ifs.fail()) && !ifs.eof() )
        {
            cerr << "Error reading the file, line " << line << endl;
            ifs.clear();
            ifs.ignore(numeric_limits<streamsize>::max(), '\n');
            exit(-1);

        } else if ( !ifs.fail() ) {
            //cout << "(" << row << ", " << column << ") = " << value << endl;
            sudoku.setValue(--row, --column, value);
        }
    }
}

//------------------------------------------------------------------------------

void printColumnSeparator(const int column)
{
    if (column % Sudoku99::SUBREGION_NUM_COLUMNS) {
        cout << "|";
    } else {
        cout << "‖";
    }
}

void printRowSeparator(const int row)
{
    for (int k = 0; k < Sudoku99::NUM_COLUMNS; ++k)
    {
        printColumnSeparator(k);

        if (row % Sudoku99::SUBREGION_NUM_ROWS) {
            cout << "---";
        } else {
            cout << "===";
        }
    }
    printColumnSeparator(Sudoku99::NUM_COLUMNS);
    cout << endl;
}

void printSudoku(const Sudoku99& sudoku)
{
    for (int i = 0; i < Sudoku99::NUM_ROWS; ++i)
    {
        // Row region Separator
        printRowSeparator(i);

        // Row numbers
        for (int j = 0; j < Sudoku99::NUM_COLUMNS; ++j)
        {
            printColumnSeparator(j);
            cout << " " << sudoku.getValue(i, j) << " ";
        }
        printColumnSeparator(Sudoku99::NUM_COLUMNS);
        cout << endl;
    }

    // Last row Separator
    printRowSeparator(Sudoku99::NUM_ROWS);
}


