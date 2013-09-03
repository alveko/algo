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

void print_array_1d(const array &arr, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        std::cout << boost::format("%02d ") % arr[i];
    }
    std::cout << std::endl;
}

void print_array_2d(const array &arr, size_t n, size_t m)
{
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            std::cout << boost::format("%02d ") % arr[i*m + j];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    size_t rows = 5;
    size_t cols = 9;
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

    for (size_t n = 1; n <= rows; n++) {
        for (size_t m = 1; m <= cols; m++) {

            array arr1(n * m);
            for (size_t i = 0; i < arr1.size(); i++) {
                arr1[i] = i;
            }

            array arr2 = arr1;
            algo::array_2d_transpose(arr2.begin(), n, m);

            bool bOk = true;
            for (size_t i = 0; (i < n) && bOk; i++) {
                for (size_t j = 0; (j < m) && bOk; j++) {
                    if (arr1[m*i+j] != arr2[n*j+i]) {
                        bOk = false;
                        std::cout <<
                            boost::format("n = %d, m = %d, i = %d, j = %d\n"
                                          "arr1[m*i+j = %d] = %d\n"
                                          "arr2[n*j+i = %d] = %d\n")
                            % n % m % i % j
                            % (m*i+j) % (arr1[m*i+j])
                            % (n*j+i) % (arr2[n*j+i]);
                    }
                }
            }

            if (!bOk) {
                std::cout << "\narr1 (original)  : ";
                print_array_1d(arr1, n*m);
                print_array_2d(arr1, n, m);
                std::cout << "\narr2 (transposed): ";
                print_array_1d(arr2, n*m);
                print_array_2d(arr2, m, n);
            }
        }
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
