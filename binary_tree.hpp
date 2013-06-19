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
#include <stack>
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
        data_type data;
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
    binary_tree_traverse_inorder(TreeNode* node, Visitor visit,
                                 TreeNode* TreeNode::* left  = &TreeNode::left,
                                 TreeNode* TreeNode::* right = &TreeNode::right)
    {
        std::stack<TreeNode*> stack;
        while (node || !stack.empty()) {
            if (node) {
                // *** go left as deep as possible
                // push nodes on the way to the stack
                stack.push(node);
                node = node->*left;
            } else {
                // *** we are at the bottom (or backtracking up)
                // 1. pop from the stack
                node = stack.top();
                stack.pop();
                // 2. visit the node
                visit(node);
                // 3. go to the right child
                node = node->*right;
            }
        }
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_preorder(TreeNode* node, Visitor visit,
                                  TreeNode* TreeNode::* left  = &TreeNode::left,
                                  TreeNode* TreeNode::* right = &TreeNode::right)
    {
        std::stack<TreeNode*> stack;
        while (node || !stack.empty()) {
            if (node) {
                // *** go left as deep as possible
                // 1. visit the node
                visit(node);
                // 2. if right child exists, push it to the stack
                if (node->*right)
                    stack.push(node->*right);
                // 3. go to the next left child
                node = node->*left;
            } else {
                // *** we are at the bottom (or backtracking up)
                // pop from the stack (stored right childs)
                node = stack.top();
                stack.pop();
            }
        }
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_postorder(TreeNode* node, Visitor visit,
                                   TreeNode* TreeNode::* left  = &TreeNode::left,
                                   TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!node) {
            return;
        }

        std::stack<TreeNode*> stack;
        stack.push(node);
        TreeNode* prev = nullptr;

        while (!stack.empty()) {

            // every iteration starts with
            // the node at the top of the stack
            node = stack.top();

            // (1) basic case, going down
            if (prev == nullptr ||            // if just started (no prev)
                prev->*left == node ||        // or if went down left or right
                prev->*right == node) {       // on the previous iteration
                                              // => continue to go down!
                if (node->*left)              // if the left child exists,
                    stack.push(node->*left);  //    => go to the left next
                else if (node->*right)        // else! if the right child exists,
                    stack.push(node->*right); //    => go to the right next

            // (2) backtracing from the left
            } else if (node->*left == prev) { // just backtracked from the left
                                              // => shall go to the right now
                if(node->*right)              // if the right child exists,
                    stack.push(node->*right); //    => go to the right next

            // (3) otherwise, ready to visit
            } else {                          // implicitly: (node == prev) or
                                              // just backtracked from the right
                visit(node);
                stack.pop();
            }
            prev = node;                      // update the prev position
        }
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_postorder2(TreeNode* node, Visitor visit,
                                    TreeNode* TreeNode::* left  = &TreeNode::left,
                                    TreeNode* TreeNode::* right = &TreeNode::right)
    {
        std::stack<TreeNode*> stack;

        // The 2 stacks solution for the iterative postorder traversal.
        //
        // First, we do the inversed postorder traversal (using 1st stack)
        // and push the nodes into the 2nd stack.
        // Second, we visit nodes as we pop them from the 2nd stack, thus
        // obtaining them in the direct postorder.

        // In order to get the inversed postorder, we run the preorder, but
        // with the reversed child pointers and put nodes into the stack
        // as we visit them.

        binary_tree_traverse_preorder(node,
                                      [ &stack ] (TreeNode* node) {
                                          stack.push(node);
                                      },
                                      right, // instead of left!
                                      left); // instead of right! :)

        while (!stack.empty()) {
            visit(stack.top());
            stack.pop();
        }
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_inorder_r(TreeNode* root, Visitor visit,
                                   TreeNode* TreeNode::* left  = &TreeNode::left,
                                   TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root) {
            return;
        }
        binary_tree_traverse_inorder_r(root->*left, visit);
        visit(root);
        binary_tree_traverse_inorder_r(root->*right, visit);
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_preorder_r(TreeNode* root, Visitor visit,
                                    TreeNode* TreeNode::* left  = &TreeNode::left,
                                    TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root) {
            return;
        }
        visit(root);
        binary_tree_traverse_preorder_r(root->*left, visit);
        binary_tree_traverse_preorder_r(root->*right, visit);
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_postorder_r(TreeNode* root, Visitor visit,
                                     TreeNode* TreeNode::* left  = &TreeNode::left,
                                     TreeNode* TreeNode::* right = &TreeNode::right)
    {
        if (!root) {
            return;
        }
        binary_tree_traverse_postorder_r(root->*left, visit);
        binary_tree_traverse_postorder_r(root->*right, visit);
        visit(root);
    }

    template <typename TreeNode, typename Visitor>
    void
    binary_tree_traverse_levels(TreeNode* root, Visitor visit,
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
            visit(node);

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

    template <typename TreeNode,
              typename DataType = typename TreeNode::data_type>
    void
    binary_tree_print_node(TreeNode* node,
                           std::ostream &out = std::cout,
                           DataType TreeNode::* data = &TreeNode::data)
    {
        out << "(" << std::setfill('0') << std::setw(2) << node->data << ")";
    }

    template <typename TreeNode,
              typename DataType = typename TreeNode::data_type,
              typename PrintNode = decltype(&binary_tree_print_node<TreeNode,
                                                                    DataType>)>
    void
    binary_tree_print(TreeNode* root,
                      std::ostream &outs = std::cout,
                      PrintNode printnode = &binary_tree_print_node<TreeNode,
                                                                    DataType>,
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

        int nodewidth = 4;
        TreeNode* tmp = binary_tree_new_node<TreeNode>();
        printnode(tmp, ssline, data);
        nodewidth = ssline.str().length();
        ssline.str("");
        delete tmp;

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
                printnode(node, ssline, data);
                //printnode(node,0);
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
