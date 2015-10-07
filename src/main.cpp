#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>

#include <stdexcept>

#include "Sudoku.hpp"
#include "SudokuOutputter.hpp"
#include "SudokuFormattedOutputter.hpp"
#include "SudokuSimpleOutputter.hpp"


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
void runSudokuSolver(const Options& opts);
Options readParameters(int argc, char *argv[]);
SudokuOutputter* createSudokuOutputter(const Options& opts, std::ostream& os);

void printHelp(const char* bin_path);
void loadSudoku(const Options&, Sudoku&);
void loadSudoku(std::istream&, Sudoku&);


// Functions
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    Options opts = readParameters(argc, argv);

    if (opts.help) {
        printHelp(argv[0]);
        return EXIT_SUCCESS;
    }

    runSudokuSolver(opts);
    return EXIT_SUCCESS;
}


void runSudokuSolver(const Options& opts)
{
    SudokuOutputter* outputter = createSudokuOutputter(opts, std::cout);

    try {
        Sudoku sudoku;
        loadSudoku(opts, sudoku);

        if (opts.verbose) {
            outputter->output(sudoku);
            std::cout << "/**" << std::endl << " * Solving ..."
                      << std::endl << " */" << std::endl;
        }

        Solver::SOLVE_RESULT solve_res = sudoku.solve();

        switch(solve_res) {
            case Solver::SATISFIABLE:
                outputter->output(sudoku);
                break;
            case Solver::UNSATISFIABLE:
                std::cout << "Error: There is no solution for the given sudoku"
                          << std::endl;
                break;
            default:
                std::cout << "Error: Unexpectd solver result" << std::endl;
                break;
        }

    } catch (const IOError& e) {
        std::cerr << "IO error captured. Error message:" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    delete outputter;
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


SudokuOutputter* createSudokuOutputter(const Options& opts, std::ostream& stream)
{
    if (opts.simple_output)
        return new SudokuSimpleOutputter(stream);
    return new SudokuFormattedOutputter(stream);
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

void loadSudoku(const Options& opts, Sudoku& sudoku)
{
    if (opts.file_path.empty()) {
        if (opts.verbose)
            std::cout << "/**" << std::endl
                      << " * Loading from standard output ..." << std::endl
                      << " */" << std::endl;
        loadSudoku(std::cin, sudoku);
    } else {
        std::ifstream file(opts.file_path.c_str());
        if (!file.is_open()) {
            throw IOError("Unable to open file: " + opts.file_path);
        }
        if (opts.verbose)
            std::cout << "/**" << std::endl 
                      << " * Loading from '" << opts.file_path << "'" 
                      << std::endl << " */" << std::endl;

        loadSudoku(file, sudoku);
        file.close();
    }
}

void loadSudoku(std::istream& is, Sudoku& sudoku)
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
