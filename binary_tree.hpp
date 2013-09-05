/// ****************************************************************************
///
/// @file   : binary_tree.hpp
/// @brief  : Binary tree algorithms
///
/// @author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
///
/// ****************************************************************************
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

#include "algo.hpp"

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

/// ----------------------------------------------------------------------------
/// @brief Creates a new node of a binary tree. Allocates memory.
///
/// @param[in]  value            date value for the newly created node
/// @param[in]  date,left,right  [opt] pointers to data,left,right members
/// @return                      a newly created node
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type>
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
    TDEBUG(("+ new: %s\n") % newnode->*data);
    return newnode;
}

/// ----------------------------------------------------------------------------
/// @brief Destroys a node of a binary tree.
///
/// @param[in]  node  node to be destroyed
/// @param[in]  date  [opt] pointers to data,left,right members
/// @return           void
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type>
void
binary_tree_destroy_node(TreeNode* node,
                         DataType  TreeNode::* data  = &TreeNode::data)
{
    TDEBUG(("- destroy: %s\n") % node->*data);
    node->left = nullptr;
    node->right = nullptr;
    delete node;
}

/// ----------------------------------------------------------------------------
/// @brief Destroyes a binary tree. Extra memory O(n).
///
/// @param[in]  root             root of the binary tree
/// @param[in]  data,left,right  [opt] pointers to data,left,right members
/// @return                      void
template <typename TreeNode, typename DataType = typename TreeNode::data_type>
void
binary_tree_destroy_tree(TreeNode* root,
                         DataType TreeNode::*data = &TreeNode::data,
                         TreeNode* TreeNode::*left = &TreeNode::left,
                         TreeNode* TreeNode::*right = &TreeNode::right)
{
    // run inorder traversal and destroy the nodes
    // as we visit them
    binary_tree_traverse_inorder(root,
                                 [ &data ] (TreeNode *node) {
                                     binary_tree_destroy_node(node, data);
                                 },
                                 left, right);
}

/// ----------------------------------------------------------------------------
/// @brief Inserts a node into a binary tree at a random position
///
/// @param[in]  root        root of the binary tree
/// @param[in]  value       a value to be inserted
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 a new node with a given value
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type>
TreeNode*
binary_tree_insert_randomly(TreeNode* root, DataType value,
                            TreeNode* TreeNode::* left  = &TreeNode::left,
                            TreeNode* TreeNode::* right = &TreeNode::right)
{
    if (!root)
        return nullptr;

    TreeNode* node = root;
    TreeNode* prev = nullptr;
    TreeNode* TreeNode::* child = nullptr;

    while (node) {
        child = (rand() % 2) ? left : right;
        prev = node;
        node = node->*child;
    }

    prev->*child =
        binary_tree_new_node<TreeNode, DataType>(value);
    return prev->*child;
}

/// ----------------------------------------------------------------------------
/// @brief Functor that compares values of two binary tree nodes
///
/*
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type,
          typename Comparator = std::less<DataType> >
struct binary_tree_node_comparator
{
    int operator()(TreeNode* n1, TreeNode* n2,
                   DataType TreeNode::*data = &TreeNode::data,
                   Comparator comp = Comparator())
    {
        return comp(n1->*data, n2->*data);
    }
};
*/

/// ----------------------------------------------------------------------------
/// @brief Inserts a node into a binary search tree
///
/// @param[in]  root             root of the binary tree
/// @param[in]  value            a new value to be inserted
/// @param[in]  comp             [opt] comparator used to compare two nodes
/// @param[in]  data,left,right  [opt] pointers to data,left,right members
/// @return                      a node with a given value
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type,
          typename Comparator = std::less<DataType> >
TreeNode*
binary_tree_insert_bst(TreeNode* node, DataType value,
                       Comparator comp = Comparator(),
                       DataType  TreeNode::* data  = &TreeNode::data,
                       TreeNode* TreeNode::* left  = &TreeNode::left,
                       TreeNode* TreeNode::* right = &TreeNode::right)
{
    if (!node)
        return nullptr;

    TreeNode* prev = nullptr;
    TreeNode* TreeNode::* child = nullptr;

    while (node && (node->*data != value)) {
        if (comp(value, node->*data)) {
            child = left;
        } else {
            child = right;
        }
        prev = node;
        node = node->*child;
    }

    if (!node) {
        prev->*child =
            binary_tree_new_node<TreeNode, DataType>(value, data, left, right);
        node = prev->*child;
    } else {
        TDEBUG(("node already exists: %-03s (%p)\n") % value % node);
    }

    return node;
}

/// ----------------------------------------------------------------------------
/// @brief Searches a given value in BST
///
/// @param[in]  root             root of the binary tree
/// @param[in]  value            value to search for
/// @param[in]  comp             [opt] comparator used to compare two nodes
/// @param[in]  data,left,right  [opt] pointers to data,left,right members
/// @return                      node with the value, if exists, null otherwise
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type,
          typename Comparator = std::less<DataType> >
TreeNode*
binary_tree_search_bst(TreeNode* node, DataType value,
                       Comparator comp = Comparator(),
                       DataType  TreeNode::* data  = &TreeNode::data,
                       TreeNode* TreeNode::* left  = &TreeNode::left,
                       TreeNode* TreeNode::* right = &TreeNode::right)
{
    while (node && (value != node->*data)) {
        if (comp(value, node->*data)) {
            node = node->*left;
        } else {
            node = node->*right;
        }
    }
    return node;
}

/// ----------------------------------------------------------------------------
/// @brief Checks if a given BT is a BST
///
/// @param[in]  root             root of the binary tree
/// @param[in]  comp             [opt] comparator used to compare two nodes
/// @param[in]  data,left,right  [opt] pointers to data,left,right members
/// @return                      true if BST, false otherwise
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type,
          typename Comparator = std::less<DataType> >
bool
binary_tree_is_bst(TreeNode* root,
                   Comparator comp = Comparator(),
                   DataType  TreeNode::* data  = &TreeNode::data,
                   TreeNode* TreeNode::* left  = &TreeNode::left,
                   TreeNode* TreeNode::* right = &TreeNode::right)
{

    bool is_bst = true;
    TreeNode *prev = nullptr;

    auto is_bst_cb = [ & ] (TreeNode* node) {
        if (prev && is_bst) {
            is_bst = comp(prev->*data, node->*data);
        }
        prev = node;
    };

    binary_tree_traverse_inorder(root, is_bst_cb);

    return is_bst;
}

/// ----------------------------------------------------------------------------
/// @brief Checks if a given BT is a balanced BT
///
/// @param[in]  root             root of the binary tree
/// @param[in]  comp             [opt] comparator used to compare two nodes
/// @param[in]  data,left,right  [opt] pointers to data,left,right members
/// @return                      true if BT is balanced, false otherwise
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type,
          typename Comparator = std::less<DataType> >
bool
binary_tree_is_balanced(TreeNode* root,
                        Comparator comp = Comparator(),
                        DataType  TreeNode::* data  = &TreeNode::data,
                        TreeNode* TreeNode::* left  = &TreeNode::left,
                        TreeNode* TreeNode::* right = &TreeNode::right)
{

    std::map<TreeNode*, int> n2h;

    auto balanced_cb = [ & ] (TreeNode* node) {

        int h_left = 0, h_right = 0;
        if (node->*left) {
            h_left = n2h[node->*left];
        }
        if (node->*right) {
            h_right = n2h[node->*right];
        }

        if (h_left == -1 || h_right == -1 ||
            std::abs(h_left - h_right) > 1) {
            // left or right subtree is not balanced
            // or balance is broken at this node
            n2h[node] = -1;
        } else if (node->*left || node->*right) {
            // if any child exists, calculate height
            n2h[node] = std::max(h_left, h_right) + 1;
        } else {
            // leaf
            n2h[node] = 0;
        }

        TDEBUG(("data = %2s, hl = %2s, hr = %2s, h = %2s\n")
               % node->data % h_left % h_right % n2h[node]);
    };

    binary_tree_traverse_postorder(root, balanced_cb);

    return n2h[root] >= 0;
}

/// ----------------------------------------------------------------------------
/// @brief Inorder traversal of a binary tree.
///        One stack solution with O(n) extra memory.
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
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
            // 1. go to the right child next
            node = stack.top()->*right;      // It is important here
            // 2. visit the node             // NOT to access the node
            visit(stack.top());              // that has been visited, since
            // 3. pop from the stack         // the visitor might have
            stack.pop();                     // destroyed the node already!
        }
    }
}

/// ----------------------------------------------------------------------------
/// @brief Preorder traversal of a binary tree.
///        One stack solution with O(n) extra memory.
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
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

/// ----------------------------------------------------------------------------
/// @brief Postorder traversal of a binary tree.
///        One stack solution with O(n) extra memory.
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
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
            prev->*right == node) {       // => continue going down!

            if (node->*left)              // if left child exists,
                stack.push(node->*left);  //    => go down to the left
            else if (node->*right)        // else if right child exists,
                stack.push(node->*right); //    => go down to the right
        }
        // (2) backtracing from the left
        else if (node->*left == prev) {

            if (node->*right)             // if the right child exists,
                stack.push(node->*right); //    => go to the right next
        }
        // (3) at the bottom or backtracking from the right
        else {
            visit(node);                  // finally, visit the node
            stack.pop();
        }
        prev = node;                      // update the prev position
    }
}

/// ----------------------------------------------------------------------------
/// @brief Postorder traversal of a binary tree.
///        Another one stack solution with O(n) extra memory.
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
template <typename TreeNode, typename Visitor>
void
binary_tree_traverse_postorder1(TreeNode* node, Visitor visit,
                                TreeNode* TreeNode::* left  = &TreeNode::left,
                                TreeNode* TreeNode::* right = &TreeNode::right)
{
    std::stack<TreeNode*> stack;

    while (node || !stack.empty()) {

        if (node) {
            // (1) going down to the left
            if (node->*right)              // if right child exists,
                stack.push(node->*right);  //     => push it to the stack
            stack.push(node);              // push node to the stack
            node = node->*left;            // go down to the left

        } else {
            // (2) backtracking
            node = stack.top();
            stack.pop();

            if (!stack.empty() && node->*right == stack.top()) {
                // right child is not visited yet, pop it
                stack.pop();
                // push parent back to the stack
                stack.push(node);
                // go to the right child next
                node = node->*right;
            } else {
                visit(node);
                node = NULL;
            }
        }
    }
}

/// ----------------------------------------------------------------------------
/// @brief Postorder traversal of a binary tree.
///        Two stacks solution with O(n) extra memory. Reduction to preorder.
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
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

/// ----------------------------------------------------------------------------
/// @brief Recursive inorder traversal of a binary tree. Extra memory O(n).
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
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

/// ----------------------------------------------------------------------------
/// @brief Recursive preorder traversal of a binary tree. Extra memory O(n).
///
/// @param[in]  root         root of the binary tree
/// @param[in]  visit        visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                  void
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

/// ----------------------------------------------------------------------------
/// @brief Recursive postorder traversal of a binary tree. Extra memory O(n).
///
/// @param[in]  root         root of the binary tree
/// @param[in]  visit        visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                  void
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

/// ----------------------------------------------------------------------------
/// @brief Level-order traversal of a binary tree. Stack with O(n) extra memory.
///
/// @param[in]  root        root of the binary tree
/// @param[in]  visit       visitor callback
/// @param[in]  left,right  [opt] pointers to left,right members
/// @return                 void
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

/// ----------------------------------------------------------------------------
/// @brief Prints out a single node of a binary tree.
///
/// @param[in]  node  a node to be printed
/// @param[in]  out   [opt] out stream (default, std::cout)
/// @param[in]  data  [opt] pointer to the data member
/// @return           void
template <typename TreeNode,
          typename DataType = typename TreeNode::data_type>
void
binary_tree_print_node(TreeNode* node,
                       std::ostream &out = std::cout,
                       DataType TreeNode::* data = &TreeNode::data)
{
    out << "(" << std::setfill('0') << std::setw(2) << node->data << ")";
}

/// ----------------------------------------------------------------------------
/// @brief Prints out a diagram of a binary tree.
///
/// @param[in]  root             root of the binary tree
/// @param[in]  outs             [opt] out stream (default, std::cout)
/// @param[in]  printnode        [opt] callback to print a node
/// @param[in]  date,left,right  [opt] pointers to data,left,right members
/// @return                      void
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
    // In the tree diagram the most crucial property of a node is its distance
    // to the left edge. The diagram can be seen as a number of columns
    // where every column contains only one node at a certain horizontal
    // level. Every line consists of blocks, one block per column. A block
    // can be 1) a node block; 2) a space block, 3) an underscore block or
    // 3) left/right child-arrow block.

    // Example (every node labeled with its inorder index):
    /*
     |     ____(02)________
     |    /                \
     | (00)            ____(05)
     |     \          /        \
     |     (01)    (03)        (06)
     |                 \
     |                 (04)
    */

    if (!root)
        return;

    // First we do inroder traversal and map every node to
    // its inorder index. The inorder index is the column index in the diagram.
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
    printnode(root, ssline, data);
    nodewidth = ssline.str().length();
    ssline.str("");

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

} // namepace algo

#endif
