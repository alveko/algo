#include <vector>
#include <iterator>
#include <algorithm> // std::max()
#include <iostream>  // std::cin, std::cout
#include <stdlib.h>  // rand()
#include <time.h>    // time()

#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include "algo/array_2d_transpose.hpp"

namespace po = boost::program_options;

using array = std::vector<int>;

void print_array_2d(const array &arr, size_t n, size_t m)
{

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            std::cout << boost::format("%2d ") % arr[i*m + j];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    size_t rows = 3;
    size_t cols = 5;
    int verbose = 0;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help")
        ("verbose,v", po::value<int>(&verbose)->default_value(verbose),
         "Verbose output level: 0, 1 or 2")
        ("rows", po::value<size_t>(&rows)->default_value(rows),
         "Number of rows (n)")
        ("rows", po::value<size_t>(&rows)->default_value(rows),
         "Number of columns (m)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }


    array arr(rows * cols);
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = i;
    }

    std::cout << "\nOriginal array:" << std::endl;
    print_array_2d(arr, rows, cols);

    algo::array_2d_transpose(arr.begin(), rows, cols);

    std::cout << "\nTransposed array:" << std::endl;
    print_array_2d(arr, cols, rows);

    return 0;
}
