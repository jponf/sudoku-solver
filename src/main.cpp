#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>

#include <stdexcept>

#include "sudoku99.hpp"

using namespace sudoku;

//
// Simple main to test the application (Must be changed in the future)
//

// Local types
// --------------------------------------------------------
struct Options 
{
    bool help;
    bool verbose;
    bool simple_output;
    std::string file_path;
};


class IOError : public std::runtime_error
{
public:
    IOError(const std::string& what) : std::runtime_error(what) { }
};

// Function prototypes
// --------------------------------------------------------
Options readParameters(int argc, char *argv[]);

void printHelp(const char* bin_path);
void printSudoku(std::ostream& ,const Sudoku99&);

void loadSudoku99(const Options&, Sudoku99&);
void loadSudoku99(std::istream&, Sudoku99&);

//


int main(int argc, char *argv[])
{
    Options opts = readParameters(argc, argv);

    if (opts.help) {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    try {
        Sudoku99 sudoku;
        loadSudoku99(opts, sudoku);

        if (opts.verbose) {
            printSudoku(std::cerr, sudoku);
        }
    } catch (const IOError& e) {
        std::cerr << "IO error captured. Error message:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    /*if (verbose) {
        printSudoku(sudoku);
        std::cerr << endl;
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
    }*/

    return EXIT_SUCCESS;
}


// Reads user command line parameters
Options readParameters(int argc, char* argv[])
{
    Options opts;

    // default values
    opts.help = false;
    opts.verbose = false;
    opts.simple_output = false;
    opts.file_path = "";

    // argument parsing
    for (int i = 1; i < argc; ++i) {
        if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) {
            opts.help = true;
            break;
        } else if (strcmp("-v", argv[i]) == 0) {
            opts.verbose = true;
        } else if (strcmp("-s", argv[i]) == 0) {
            opts.simple_output = true;
        } else {
            if (!opts.file_path.empty()) {
                std::cerr << "Warning: More than one file specified ... using the first one."
                          << std::endl;
            } else {
                opts.file_path = std::string(argv[i]);
            }
        }
    }

    return opts;
}


//
void printHelp(const char* bin_path)
{
    std::cout << "Usage: " << bin_path << " [Options]" << std::endl;
    std::cout << std::endl;
    std::cout << "\tOptions:" << std::endl;
    std::cout << "\t\t-h/--help\tprint this message and exits." << std::endl;
    std::cout << "\t\t-v\t\tprints execution information." << std::endl;
    std::cout << "\t\tsudoku_file\tfile with the sudoku initial values." << std::endl;
    std::cout << std::endl;
    std::cout << "sudoku_file format:" << std::endl;
    std::cout << std::endl;
    std::cout << "\tThe sudoku grid starts at the top left cell (1, 1)" << std::endl;
    std::cout << "\tand finishes at the bottom right cell (9, 9)." << std::endl << std::endl;
    std::cout << "\tThe file must be composed by lines with the following format:" << std::endl;
    std::cout << "\t\t<row> <column> value" << std::endl;
}


//------------------------------------------------------------------------------
//

void loadSudoku99(const Options& opts, Sudoku99& sudoku)
{
    if (opts.file_path.empty()) {
        loadSudoku99(std::cin, sudoku);
    } else {
        std::ifstream file(opts.file_path);
        if (!file.is_open()) {
            throw IOError("Unable to open file: " + opts.file_path);
        }

        loadSudoku99(file, sudoku);
        file.close();
    }
}

void loadSudoku99(std::istream& is, Sudoku99& sudoku)
{
   int row, column, value;
   int line = 0;

    while (is.good()) {
        is >> row >> column >> value;
        
        if ((is.bad() || is.fail()) && !is.eof()) {  // Add option to ignore errors
            std::ostringstream oss;
            oss << "Error loading sudoku. Line: " << line;
            is.clear();
            throw IOError(oss.str());
        } else if (!is.fail()) {
            line += 1;
            sudoku.setValue(--row, --column, value);
        }
    }
}

//------------------------------------------------------------------------------

void printColumnSeparator(std::ostream& os, const int column)
{
    os << (column % Sudoku99::SUBREGION_NUM_COLUMNS ? "|" : "‖");
}

void printRowSeparator(std::ostream& os, const int row)
{
    for (int k = 0; k < Sudoku99::NUM_COLUMNS; ++k)
    {
        printColumnSeparator(os, k);

        if (row % Sudoku99::SUBREGION_NUM_ROWS) {
            os << "---";
        } else {
            os << "===";
        }
    }
    printColumnSeparator(os, Sudoku99::NUM_COLUMNS);
    std::cout << std::endl;
}

void printSudoku(std::ostream& os, const Sudoku99& sudoku)
{
    for (int i = 0; i < Sudoku99::NUM_ROWS; ++i)
    {
        // Row region Separator
        printRowSeparator(os, i);

        // Row numbers
        for (int j = 0; j < Sudoku99::NUM_COLUMNS; ++j)
        {
            printColumnSeparator(os, j);
            int value = sudoku.getValue(i, j);
            if (value == Sudoku99::UNDEFINED_VALUE) {
                os << "   ";
            } else {
                os << " " << value << " ";
            }
        }
        printColumnSeparator(os, Sudoku99::NUM_COLUMNS);
        os << std::endl;
    }

    // Last row Separator
    printRowSeparator(os, Sudoku99::NUM_ROWS);
}
