#include <vector>
#include <iterator> 
#include <algorithm> // std::max()
#include <iostream>  // std::cin, std::cout

#include "algo/longest_increasing_subsequence.hpp"

std::vector<size_t> compute_lis(const std::vector<int> &seq)
{
    int  maxln = 0;
    std::vector<int> tails(seq.size()+1, -1);
    std::vector<int> prevs(seq.size(), -1);
    
    for (size_t i = 0; i < seq.size(); i++) {

        // binary search for a tail less than current element
        int j  = 0;
        int lo = 1;
        int hi = maxln;
        while (lo <= hi) {
            int k = lo + (hi - lo) / 2;
            if (seq[tails[k]] < seq[i]) {
                j = k;
                lo = k + 1;
            } else {
                hi = k - 1;
            }
        }
        
        prevs[i] = tails[j];
        if ((j == maxln) || (seq[i] < seq[tails[j+1]])) {
            tails[j+1] = i;
            maxln = std::max(maxln, j + 1);
        }
    }

    // backtrack and store the result
    std::vector<size_t> lis(maxln, 0);
    int n = maxln;
    int i = tails[maxln];
    while (i != -1 && n > 0) {
        lis[--n] = i;
        i = prevs[i];
    }

    return lis;
}

int main(int argc, char *argv[])
{
    std::vector<int> seq;
    std::vector<size_t> lis;

    // read input sequence
    std::copy(std::istream_iterator<int>(std::cin), 
              std::istream_iterator<int>(),
              std::back_inserter(seq));

    // compute longest increasing subsequence
    lis = compute_lis(seq);

    /*
    seq[0] = 3;
    std::cout << "seq.size()  = " << seq.size() << std::endl;
    std::cout << "end - begin = " << seq.end() - seq.begin() << std::endl;

    std::cout << "*begin      = " << *(seq.begin()) << std::endl;
    std::cout << "*end        = " << *(seq.end()) << std::endl;
    std::cout << "*(end-1)    = " << *(seq.end()-1) << std::endl;
    */

    size_t lislen1;
    size_t lislen2;
    size_t lislen3;
    std::vector<size_t> lis1;
    std::vector<size_t> lis2;
    std::vector<size_t> lis3;

    lislen1 = algo::longest_increasing_subsequence(seq.begin(), seq.end());
    
    lislen2 = algo::longest_increasing_subsequence(seq.begin(), seq.end(),
                                                   std::back_inserter(lis2));
    
    lislen3 = algo::longest_increasing_subsequence_dp(seq.begin(), seq.end(),
                                                      std::back_inserter(lis3));

    // print result (lis elements marked with "+")
    std::cout << "LIS size    = " << lis.size() << std::endl;
    std::cout << "lislen1     = " << lislen1 << std::endl;
    std::cout << "lislen2     = " << lislen2 << std::endl;
    std::cout << "lislen3     = " << lislen3 << std::endl;
    std::cout << "lis1.size() = " << lis1.size() << std::endl;
    std::cout << "lis2.size() = " << lis2.size() << std::endl;
    std::cout << "lis3.size() = " << lis3.size() << std::endl;

    std::ostream_iterator<int> out_it (std::cout, " ");
    std::cout << "lis  = ";
    std::copy (lis.begin(), lis.end(), out_it);
    std::cout << std::endl;
  
    std::cout << "lis2 = ";
    std::copy (lis2.begin(), lis2.end(), out_it);
    std::cout << std::endl;
    
    std::cout << "lis3 = ";
    std::copy (lis3.begin(), lis3.end(), out_it);
    std::cout << std::endl;
    
    size_t j = 0;
    for (size_t i = 0; i < seq.size(); i++) {
        if (i == lis[j]) {
            std::cout << seq[lis[j++]] << "+ ";
        } else {
            std::cout << seq[i] << " ";
        }
        
    }
    std::cout << std::endl;
    return 0;
}
