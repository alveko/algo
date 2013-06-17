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
#include <list>
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
        std::cout << "- newnode: " << value << std::endl;
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

        std::cout << "+ insert : "
                  << prev->data << "."
                  << ((child == left) ? "left" : "right")
                  << " -> " << newnode->data
                  << std::endl;
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

        while(!queue.empty()) {
            // pop the oldest node from the queue
            TreeNode* node = queue.front();
            queue.pop();

            // visit the node
            visitor(node);

            // push any non-null children to the queue
            if (node->*left)
                queue.push(node->*left);
            if (node->*right)
                queue.push(node->*right);
        }
    }
}

#endif
