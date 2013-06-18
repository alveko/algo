/*******************************************************************************
 * File   : binary_tree.hpp
 * Brief  : Algorithms to deal with binary trees
 *
 * Author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
 *
 *******************************************************************************
 */
#ifndef ALGO_BINARY_TREE_HPP
#define ALGO_BINARY_TREE_HPP

#include <map>
//#include <list>
#include <queue>
#include <stdlib.h>  // rand()

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace algo
{

    template<typename T>
    struct binary_tree_node
    {
        using data_type = T;
        T data;
        struct binary_tree_node *left;
        struct binary_tree_node *right;
    };

    template <typename TreeNode, typename DataType = typename TreeNode::data_type>
    TreeNode*
    binary_tree_new_node(DataType value = DataType(),
                         DataType  TreeNode::* data  = &TreeNode::data,
                         TreeNode* TreeNode::* left  = &TreeNode::left,
                         TreeNode* TreeNode::* right = &TreeNode::right)
    {
        TreeNode *newnode = new TreeNode();
        newnode->*data  = value;
        newnode->*left  = nullptr;
        newnode->*right = nullptr;
        return newnode;
    }

    template <typename TreeNode>
    TreeNode*
    binary_tree_insert_randomly(TreeNode* root, TreeNode* newnode,
                                TreeNode* TreeNode::* left  = &TreeNode::left,
                                TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root)
            return newnode;

        TreeNode* node = root;
        TreeNode* prev = nullptr;
        TreeNode* TreeNode::* child = nullptr;

        while (node) {
            child = (rand() % 2) ? left : right;
            prev = node;
            node = node->*child;
        }

        prev->*child = newnode;
        return prev;
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_inorder(TreeNode* root, Visitor visitor,
                                 TreeNode* TreeNode::* left  = &TreeNode::left,
                                 TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root) {
            return;
        }
        binary_tree_traverse_inorder(root->*left, visitor);
        visitor(root);
        binary_tree_traverse_inorder(root->*right, visitor);
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_preorder(TreeNode* root, Visitor visitor,
                                  TreeNode* TreeNode::* left  = &TreeNode::left,
                                  TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root) {
            return;
        }
        visitor(root);
        binary_tree_traverse_preorder(root->*left, visitor);
        binary_tree_traverse_preorder(root->*right, visitor);
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_postorder(TreeNode* root, Visitor visitor,
                                   TreeNode* TreeNode::* left  = &TreeNode::left,
                                   TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root) {
            return;
        }
        binary_tree_traverse_postorder(root->*left, visitor);
        binary_tree_traverse_postorder(root->*right, visitor);
        visitor(root);
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_levels(TreeNode* root, Visitor visitor,
                                TreeNode* TreeNode::* left  = &TreeNode::left,
                                TreeNode* TreeNode::* right = &TreeNode::right)
    {
        // initially the queue contains only the root
        std::queue<TreeNode*> queue;
        queue.push(root);
        queue.push(nullptr); // nullptr = new level marker

        while(!queue.empty()) {
            // pop the oldest node from the queue
            TreeNode* node = queue.front();
            queue.pop();

            // visit a node (node = nullptr means new level!)
            visitor(node);

            if (node) {
                // push any non-null children to the queue
                if (node->*left) {
                    queue.push(node->*left);
                }
                if (node->*right) {
                    queue.push(node->*right);
                }
            } else if (!queue.empty()) {
                // new level begins
                queue.push(nullptr);
            }
        }
    }

    template <typename TreeNode, typename DataType = typename TreeNode::data_type>
    void
    binary_tree_print_node(TreeNode* node,
                           std::ostream &outs = std::cout, int nodewidth = 4,
                           DataType TreeNode::* data = &TreeNode::data)
    {
        outs << "("
             << std::setfill('0') << std::setw(nodewidth-2)
             << node->*data << ")";
    }

    template <typename TreeNode, typename DataType = typename TreeNode::data_type>
    void
    binary_tree_print(TreeNode* root,
                      std::ostream &outs = std::cout, int nodewidth = 4,
                      DataType  TreeNode::* data  = &TreeNode::data,
                      TreeNode* TreeNode::* left  = &TreeNode::left,
                      TreeNode* TreeNode::* right = &TreeNode::right)
    {
        // In the tree diagram the most crucial property is the width of a node
        // when it is printed. The diagram can be seen as a number of columns
        // where every column contains only one node at a certain horizontal
        // level. Every line consists of blocks, one block per column. A block
        // can be a space block, an underscore block or left/right child block.

        // Example (every node labeled with its inorder index):
        /*
         *    ____(02)________
         *   /                \
         *(00)            ____(05)
         *    \          /        \
         *    (01)    (03)        (06)
         *                \
         *                (04)
        */

        // First we do inroder traversal and map every node to
        // its inorder index. The inorder index is the column index in diagram.
        int icol = 0;
        std::map<TreeNode*, int> col;
        auto count_columns = [ & ](TreeNode *node) {
            col[node] = icol++;
        };
        binary_tree_traverse_inorder(root, count_columns);

        //
        std::stringstream ssline;   // current line (the line with nodes)
        std::stringstream ssnext;   // next line (the line with "/" and "\")
        std::stringstream ssspace, ssunder; // space and underscore blocks
        std::stringstream ssleft, ssright;  // blocks with "/" and "\"
        ssspace << " " << std::setw(nodewidth-2) << " " << " ";
        ssunder.fill('_');
        ssunder << std::setw(nodewidth) << "";
        ssleft  << std::setw(nodewidth) << std::right << "/";
        ssright << std::setw(nodewidth) << std::left  << "\\";

        int off = 0;
        auto print = [ & ](TreeNode *node) {
            if (node) {
                // process a node
                auto outline = std::ostream_iterator<std::string>(ssline);
                auto outnext = std::ostream_iterator<std::string>(ssnext);
                int cc = col[node];
                int sp = cc - off;
                int ls = (node->*left) ? (cc - col[node->*left]) : 0;
                int rs = (node->*right) ? (col[node->*right] - cc) : 0;
                std::fill_n(outline, sp - ls, ssspace.str());
                std::fill_n(outnext, sp - ls, ssspace.str());
                if (ls) {
                    std::fill_n(outline, 1, ssspace.str());
                    std::fill_n(outnext, 1, ssleft.str());
                    std::fill_n(outline, ls - 1, ssunder.str());
                    std::fill_n(outnext, ls - 1, ssspace.str());
                }
                binary_tree_print_node(node, ssline, nodewidth, data);
                ssnext << ssspace.str();
                if (rs) {
                    std::fill_n(outline, rs - 1, ssunder.str());
                    std::fill_n(outnext, rs - 1, ssspace.str());
                    std::fill_n(outline, 1, ssspace.str());
                    std::fill_n(outnext, 1, ssright.str());
                }
                off = col[node] + rs + 1;
            } else {
                // nullptr node => new level, print and reset the lines
                outs << ssline.str() << std::endl;
                outs << ssnext.str() << std::endl;
                ssline.str("");
                ssnext.str("");
                off = 0;
            }
        };

        // at the end of the day, it's just a horizontal level traversal
        binary_tree_traverse_levels(root, print);
    }
}

#endif
