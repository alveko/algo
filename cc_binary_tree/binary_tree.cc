#include <vector>
#include <iterator>
#include <algorithm>   // std::max()
#include <iostream>    // std::cin, std::cout
#include <functional>  // std::bind

#include <stdlib.h>  // rand()
#include <time.h>    // time()

//#include <boost/program_options.hpp>
//#include <boost/format.hpp>

#include "algo/binary_tree.hpp"

using namespace std::placeholders;

using BinaryTreeNode = algo::binary_tree_node<int>;

template <typename V>
void print_vector(const char *msg, const V &v) {
    std::cout << msg << " ";
    for (auto it : v)
        std::cout << it->data << " ";
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int n = 25;
    int cnt = 1;

    BinaryTreeNode *root;
    root = algo::binary_tree_new_node<BinaryTreeNode>(cnt++);

    for (int i = cnt; i <= n; i++) {
        algo::binary_tree_insert_randomly(root,
                                          algo::binary_tree_new_node
                                          <BinaryTreeNode>(cnt++));
    }

    auto v2v = [ ] (BinaryTreeNode *n, std::vector<BinaryTreeNode *> *v) {
        if (n)
            v->push_back(n);
    };

    std::vector<BinaryTreeNode*> vinr, vin;
    algo::binary_tree_traverse_inorder_r(root, std::bind(v2v, _1, &vinr));
    algo::binary_tree_traverse_inorder(root, std::bind(v2v, _1, &vin));

    std::vector<BinaryTreeNode*> vprer, vpre;
    algo::binary_tree_traverse_preorder_r(root, std::bind(v2v, _1, &vprer));
    algo::binary_tree_traverse_preorder(root, std::bind(v2v, _1, &vpre));

    std::vector<BinaryTreeNode*> vpostr, vpost, vpost2;
    algo::binary_tree_traverse_postorder_r(root, std::bind(v2v, _1, &vpostr));
    algo::binary_tree_traverse_postorder(root, std::bind(v2v, _1, &vpost));
    algo::binary_tree_traverse_postorder2(root, std::bind(v2v, _1, &vpost2));

    std::cout << std::endl;
    print_vector("in   order:", vinr);
    print_vector("pre  order:", vprer);
    print_vector("post order:", vpostr);

    if (vinr != vin) {
        std::cout << "Error!" << std::endl;
        print_vector("  vinr :", vinr);
        print_vector("  vin  :", vin);
    }
    if (vprer != vpre) {
        std::cout << "Error!" << std::endl;
        print_vector("  vprer :", vprer);
        print_vector("  vpre  :", vpre);
    }
    if (vpostr != vpost || vpostr != vpost2) {
        std::cout << "Error!" << std::endl;
        print_vector("  vpostr :", vpostr);
        print_vector("  vpost  :", vpost);
        print_vector("  vpost2 :", vpost2);
    }

    std::vector<BinaryTreeNode *> levels;
    algo::binary_tree_traverse_levels(root, std::bind(v2v, _1, &levels));

    std::cout << std::endl;
    print_vector("h. levels :", levels);
    std::cout << std::endl;

    algo::binary_tree_print(root);

    return 0;
}
