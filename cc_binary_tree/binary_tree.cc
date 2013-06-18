#include <vector>
#include <iterator>
#include <algorithm> // std::max()
#include <iostream>  // std::cin, std::cout
#include <stdlib.h>  // rand()
#include <time.h>    // time()

#include <sstream>

//#include <boost/program_options.hpp>
//#include <boost/format.hpp>

#include "algo/binary_tree.hpp"

using BinaryTreeNode = algo::binary_tree_node<int>;

void visitor(BinaryTreeNode *node)
{
    if (node)
        std::cout << node->data << " ";
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    int n = 7;
    int cnt = 1;

    BinaryTreeNode *root;
    root = algo::binary_tree_new_node<BinaryTreeNode>(cnt++);

    for (int i = cnt; i <= n; i++) {
        algo::binary_tree_insert_randomly(root,
                                          algo::binary_tree_new_node
                                          <BinaryTreeNode>(cnt++));
    }

    std::cout << "inorder   : ";
    algo::binary_tree_traverse_inorder(root, visitor);
    std::cout << std::endl;

    std::cout << "preorder  : ";
    algo::binary_tree_traverse_preorder(root, visitor);
    std::cout << std::endl;

    std::cout << "postorder : ";
    algo::binary_tree_traverse_postorder(root, visitor);
    std::cout << std::endl;

    std::cout << "h. levels : ";
    algo::binary_tree_traverse_levels(root, visitor);
    std::cout << std::endl;

    std::cout << "print     : " << std::endl;
    algo::binary_tree_print(root);

    return 0;
}
