#include <vector>
#include <iterator> 
#include <algorithm> // std::max()
#include <iostream>  // std::cin, std::cout
#include <stdlib.h>  // rand()
#include <time.h>    // time()

#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include "algo/longest_increasing_subsequence.hpp"

namespace po = boost::program_options;

void print_lis(const std::vector<int> &seq,
               const std::vector<size_t> &lis)
{
    // print result (lis elements marked with "+")
    size_t j = 0;
    for (size_t i = 0; i < seq.size(); i++) {

        if (i == lis[j]) {
            std::cout << boost::format("%2s%1s ") % seq[i] % "+";
            j++;
        } else {
            std::cout << boost::format("%2s%1s ") % seq[i] % " ";
        }
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    size_t size = 10;
    int minval = 0;
    int maxval = 100;
    int verbose = 0;
    
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help")
        ("verbose,v", po::value<int>(&verbose)->default_value(verbose),
         "Verbose output level: 0, 1 or 2")
        ("lis", po::value<std::string>()->default_value("all"),
         "LIS algorithm:\n<dp | nlogn | all>")
        ("size", po::value<size_t>(&size)->default_value(size),
         "Size of the input array for LIS")
        ("minval", po::value<int>(&minval)->default_value(minval),
         "Min random value of the array")
        ("maxval", po::value<int>(&maxval)->default_value(maxval),
         "Max random value of the array");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") ||
        (vm["lis"].as<std::string>() != "dp" &&
         vm["lis"].as<std::string>() != "nlogn" &&
         vm["lis"].as<std::string>() != "all")) {
        std::cout << desc << std::endl;
        return 1;
    }
    
    std::vector<int> seq;
    std::vector<size_t> lis;
    std::ostream_iterator<int> out_it (std::cout, " ");

    srand(time(NULL));
    seq.resize(size);
    std::generate(seq.begin(), seq.end(),
                  [ minval, maxval ]() -> int
                  { return minval + rand() % (maxval - minval + 1); });

    if (verbose > 1) {
        std::cout << boost::format("LIS type: %s\n") % vm["lis"].as<std::string>();
        std::cout << "seq  = ";
        std::copy(seq.begin(), seq.end(), out_it);
        std::cout << std::endl;
    }
    
    /*
    // read input sequence
    std::copy(std::istream_iterator<int>(std::cin), 
              std::istream_iterator<int>(),
              std::back_inserter(seq));
    */

    // compute longest increasing subsequence
    std::vector<size_t> lis_dp;
    std::vector<size_t> lis_nlogn;

    if (vm["lis"].as<std::string>() == "dp" ||
        vm["lis"].as<std::string>() == "all") {
        algo::longest_increasing_subsequence_dp(seq.begin(), seq.end(),
                                                std::back_inserter(lis_dp));
    }

    if (vm["lis"].as<std::string>() == "nlogn" ||
        vm["lis"].as<std::string>() == "all") {
        algo::longest_increasing_subsequence(seq.begin(), seq.end(),
                                             std::back_inserter(lis_nlogn));
    }
    
    if (verbose) {
        std::cout << "lis_dp.size()    = " << lis_dp.size() << std::endl;
        std::cout << "lis_nlogn.size() = " << lis_nlogn.size() << std::endl;
    }
    
    if (verbose > 1) {
        std::cout << "lis_dp     = ";
        std::copy (lis_dp.begin(), lis_dp.end(), out_it);
        std::cout << " => ";
        print_lis(seq, lis_dp);
        
        std::cout << "lis_nlogn  = ";
        std::copy (lis_nlogn.begin(), lis_nlogn.end(), out_it);
        std::cout << " => ";
        print_lis(seq, lis_nlogn);
    }

    return 0;
}
