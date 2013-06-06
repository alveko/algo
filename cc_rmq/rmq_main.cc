#include <vector>
#include <iterator> 
#include <algorithm> // std::min()
#include <iostream>  // std::cin, std::cout
#include <stdlib.h>  // rand()
#include <time.h>    // time()

#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include "rmq.hh"

namespace po = boost::program_options;

class RmqProblemHelper
{
    std::vector<int> v;
    std::vector<size_t> segmenttree;
    std::vector< std::vector<size_t> > sparsetable;
    
public:

    struct RmqParams
    {
        enum {
            rmqt_naive = (1<<0),
            rmqt_sparsetable = (1<<1),
            rmqt_segmenttree = (1<<2),
            rmqt_all = 0x0F,
            rmqt_test = 0x1F,
        } rmqt;
        
        size_t size;
        size_t q_num;
        int minval;
        int maxval;
    } params;
    
    RmqProblemHelper()
    {
        params.rmqt = RmqParams::rmqt_all;
        params.size = 100;
        params.q_num = 10;
        params.minval = 10;
        params.minval = 99;
    }
    
    void randomize_vector()
    {
        v.clear();
        v.resize(params.size);
        srand (time(NULL));
        for (size_t i = 0; i < params.size; i++) {
            v[i] = params.minval + rand() % (params.maxval - params.minval + 1);
        }
    }

    void rmq_init()
    {
        std::cout <<
            boost::format("Params:\n"
                          "size      = %d\n"
                          "minval    = %d\n"
                          "maxval    = %d\n"
                          "q_num     = %d\n")
            % params.size % params.minval
            % params.maxval % params.q_num;

        randomize_vector();
    
        if (params.rmqt & RmqParams::rmqt_sparsetable) {
            rmq_sparse_table_build(v, sparsetable);
        }
        
        if (params.rmqt & RmqParams::rmqt_segmenttree) {
            rmq_segment_tree_build(v, segmenttree);
        }
    }

    void rmq_run()
    {
        if (params.rmqt == RmqParams::rmqt_test) {
            // test all possible queries (quadratic time)
            for (size_t i = 0; i < params.size; i++) {
                for (size_t j = i; j < params.size; j++) {
                    rmq_query(i, j);
                }
            }
        } else {
            // run a number of radom queries
            for (size_t q = 0; q < params.q_num; q++) {
                
                size_t q_size = rand() % (params.size + 1);
                size_t i = rand() % (params.size - q_size + 1);
                size_t j = i + q_size - 1;
                
                rmq_query(i, j);
            }
        }
    }
    
    void rmq_query(size_t i, size_t j)
    {
        size_t rmq1, rmq2, rmq3;
        
        if (params.rmqt & RmqParams::rmqt_naive)
            rmq1 = rmq_naive(v, i, j);
        
        if (params.rmqt & RmqParams::rmqt_sparsetable)
            rmq2 = rmq_sparse_table_query(v, sparsetable, i, j);
        
        if (params.rmqt & RmqParams::rmqt_segmenttree)
            rmq3 = rmq_segment_tree_query(v, segmenttree, i, j);
        
        if (params.rmqt == RmqParams::rmqt_all &&
            ! (v[rmq1] == v[rmq2] &&
               v[rmq2] == v[rmq3])) {
            std::cout <<
                boost::format("\nerror: i = %d, j = %d\n"
                              "rmq1 = %d, v[rmq1] = %d\n"
                              "rmq2 = %d, v[rmq2] = %d\n"
                              "rmq3 = %d, v[rmq3] = %d\n")
                % i % j % rmq1 % v[rmq1] % rmq2 % v[rmq2] % rmq3 % v[rmq3];
        }
    }
        
};

int main(int argc, char *argv[])
{
    RmqProblemHelper rmq;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Show help")
        
        ("rmq", po::value<std::string>()->default_value("all"),
         "RMQ algorithm:\n<naive | sparsetable | segmenttree | all | test>")
        
        ("size", po::value<size_t>(&rmq.params.size)->default_value(rmq.params.size),
         "Size of the array for RMQ")
        
        ("minval", po::value<int>(&rmq.params.minval)->default_value(rmq.params.minval),
         "Min random value of the array")
        
        ("maxval", po::value<int>(&rmq.params.maxval)->default_value(rmq.params.maxval),
         "Max random value of the array")
        
        ("q-num", po::value<size_t>(&rmq.params.q_num)->default_value(rmq.params.q_num),
         "Number of range min queries");
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") ||
        (vm["rmq"].as<std::string>() != "naive" &&
         vm["rmq"].as<std::string>() != "sparsetable" &&
         vm["rmq"].as<std::string>() != "segmenttree" &&
         vm["rmq"].as<std::string>() != "all" &&
         vm["rmq"].as<std::string>() != "test")) {
        std::cout << desc << std::endl;
        return 1;
    }

    std::cout << boost::format("RMQ: %s") % vm["rmq"].as<std::string>();
    
    if (vm["rmq"].as<std::string>() == "naive")
        rmq.params.rmqt = RmqProblemHelper::RmqParams::rmqt_naive;
    if (vm["rmq"].as<std::string>() == "sparsetable")
        rmq.params.rmqt = RmqProblemHelper::RmqParams::rmqt_sparsetable;
    if (vm["rmq"].as<std::string>() == "segmenttree")
        rmq.params.rmqt = RmqProblemHelper::RmqParams::rmqt_segmenttree;
    if (vm["rmq"].as<std::string>() == "all")
        rmq.params.rmqt = RmqProblemHelper::RmqParams::rmqt_all;
    if (vm["rmq"].as<std::string>() == "test")
        rmq.params.rmqt = RmqProblemHelper::RmqParams::rmqt_test;
    
    rmq.rmq_init();
    
    rmq.rmq_run();
    
    return 0;
}
