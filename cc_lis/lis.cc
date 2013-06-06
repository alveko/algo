#include <vector>
#include <iterator> 
#include <algorithm> // std::max()
#include <iostream>  // std::cin, std::cout

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

    // print result (lis elements marked with "+")
    std::cout << "LIS size = " << lis.size() << std::endl;
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
