#include <vector>
#include <iterator>
#include <algorithm>   // std::max()
#include <iostream>    // std::cin, std::cout
#include <functional>  // std::bind
#include <stdlib.h>    // rand()
#include <time.h>      // time()

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

void run_traversals(BinaryTreeNode *root)
{
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

    std::vector<BinaryTreeNode*> vpostr, vpost, vpost2, vpost1;
    algo::binary_tree_traverse_postorder_r(root, std::bind(v2v, _1, &vpostr));
    algo::binary_tree_traverse_postorder(root, std::bind(v2v, _1, &vpost));
    algo::binary_tree_traverse_postorder1(root, std::bind(v2v, _1, &vpost1));
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
    if (vpostr != vpost || vpostr != vpost1 || vpost1 != vpost2) {
        std::cout << "Error!" << std::endl;
        print_vector("  vpostr :", vpostr);
        print_vector("  vpost  :", vpost);
        print_vector("  vpost1 :", vpost1);
        print_vector("  vpost2 :", vpost2);
    }

    std::vector<BinaryTreeNode *> levels;
    algo::binary_tree_traverse_levels(root, std::bind(v2v, _1, &levels));

    std::cout << std::endl;
    print_vector("h. levels :", levels);
    std::cout << std::endl;

    bool is_bst = algo::binary_tree_is_bst(root);
    std::cout << std::endl;
    std::cout << "Is BST      : " << is_bst << std::endl;

    bool is_balanced = algo::binary_tree_is_balanced(root);
    std::cout << "Is balanced : " << is_balanced << std::endl;
    std::cout << std::endl;
}


int main(int argc, char *argv[])
{
    srand(time(NULL));

    BinaryTreeNode *root;
    int n = 10;
    int cnt = 1;

    // random BT

    root = algo::binary_tree_new_node<BinaryTreeNode>(cnt++);

    for (int i = cnt; i <= n; i++) {
        algo::binary_tree_insert_randomly(root, cnt++);
    }

    run_traversals(root);
    algo::binary_tree_print(root);
    algo::binary_tree_destroy_tree(root);

    // random BST

    root = algo::binary_tree_new_node<BinaryTreeNode>(rand() % n + 1);

    for (int i = 0; i < n; i++) {
        algo::binary_tree_insert_bst(root, rand() % n + 1);
    }

    run_traversals(root);
    algo::binary_tree_print(root);
    algo::binary_tree_destroy_tree(root);

    return 0;
}
