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
// Simple main to test the sudoku solver
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


// Local utility inline functions
// -----------------------------------------------------------------------------

#define coutln(X) (std::cout << X << std::endl)

inline bool streq(const char* str1, const char* str2)
{
    return strcmp(str1, str2) == 0;
}


// Functions
// -----------------------------------------------------------------------------

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
        std::cout << "Error: IO error '" << e.what() << "'" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << std::endl;
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
        if (streq("-h", argv[i]) || streq("--help", argv[i])) {
            opts.help = true;
        } else if (streq("-v", argv[i]) || streq("--verbose", argv[i])) {
            opts.verbose = true;
        } else if (streq("-s", argv[i]) || streq("--simple", argv[i])) {
            opts.simple_output = true;
        } else {
            if (!opts.file_path.empty()) {
                std::cerr << "Warning: More than one file specified ..."
                             " using the first one." << std::endl;
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
    coutln("Usage: " << bin_path << " [Options] [sudoku_file]");

    std::cout << std::endl;
    coutln("\tOptions:");
    coutln("\t\t-h/--help     print this message and exit.");
    coutln("\t\t-v/--verbose  print additional execution information.");

    std::cout << std::endl;
    //coutln("\t\t-a/--all      computes all the possible solutions [TODO].");
    coutln("\t\t-s/--simple   print sudoku without formatting.");
    coutln("\t\tsudoku_file   file with the sudoku initial values.");
    coutln("\t\t              If not specified reads from the standard input.");

    std::cout << std::endl;
    coutln("sudoku_file format:");
    std::cout << std::endl;
    coutln("\tThe sudoku grid starts at the top left cell (1, 1)");
    coutln("\tand finishes at the bottom right cell (9, 9).");
    std::cout << std::endl;
    coutln("\tThe file must be composed by lines with the following format:");
    coutln("\t\t<row> <column> value");
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
