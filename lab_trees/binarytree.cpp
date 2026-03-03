/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

#include <algorithm>
#include <limits>
#include "binarytree.h"

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    std::cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
    mirror(root);
}

/**
 * Private helper for mirror().
 * @param subRoot The current node in the recursion
 */
template <typename T>
void BinaryTree<T>::mirror(Node* subRoot)
{
    if (subRoot == NULL)
        return;

    // Mirror subtrees first
    mirror(subRoot->left);
    mirror(subRoot->right);

    // Then swap left and right pointers
    std::swap(subRoot->left, subRoot->right);
}

/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered() const
{
    if (root == NULL)
        return true;

    bool isBST = true;
    // Initialize lastVisited to the lowest possible value for T
    T lastVisited = std::numeric_limits<T>::lowest();

    isOrdered(root, lastVisited, isBST);
    return isBST;
}

/**
 * Private helper for isOrdered().
 * @param subRoot The current node
 * @param lastVisited The value of the last visited node
 * @param isBST       Boolean flag indicating if the tree is still ordered
 */
template <typename T>
void BinaryTree<T>::isOrdered(const Node* subRoot, T& lastVisited, bool& isBST) const
{
    if (!isBST || subRoot == NULL)
        return;

    // First, left subtree
    isOrdered(subRoot->left, lastVisited, isBST);

    // Check current node
    if (subRoot->elem < lastVisited) {
        isBST = false;
        return;
    }
    lastVisited = subRoot->elem;

    // Then, right subtree
    isOrdered(subRoot->right, lastVisited, isBST);
}

/**
 * Creates vectors of all the possible paths from the root of the tree to any leaf
 * node and adds them to the main vector.
 * Path: all sequences from root to leaf, left to right.
 * @param paths (output) vector of paths
 */
template <typename T>
void BinaryTree<T>::printPaths(vector<vector<T> >& paths) const
{
    vector<T> currentPath;
    printPaths(root, currentPath, paths);
}

/**
 * Private helper for printPaths().
 * @param subRoot     Current node
 * @param currentPath Current path from root
 * @param paths       Main vector where paths are stored
 */
template <typename T>
void BinaryTree<T>::printPaths(const Node* subRoot,
                               vector<T>& currentPath,
                               vector<vector<T> >& paths) const
{
    if (subRoot == NULL)
        return;

    // Add current element to path
    currentPath.push_back(subRoot->elem);

    // If leaf node
    if (subRoot->left == NULL && subRoot->right == NULL) {
        paths.push_back(currentPath);
    } else {
        // Recursively continue
        printPaths(subRoot->left, currentPath, paths);
        printPaths(subRoot->right, currentPath, paths);
    }

    // On return, remove last element
    currentPath.pop_back();
}

/**
 * Each node in a tree has a distance (depth) from the root.
 * This function returns the sum of all these distances.
 * @return sum of depths of all nodes
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
    return sumDistances(root, 0);
}

/**
 * Private helper for sumDistances().
 * @param subRoot Current node
 * @param depth   Depth of current node
 * @return sum of depths in the subtree
 */
template <typename T>
int BinaryTree<T>::sumDistances(const Node* subRoot, int depth) const
{
    if (subRoot == NULL)
        return 0;

    int leftSum = sumDistances(subRoot->left, depth + 1);
    int rightSum = sumDistances(subRoot->right, depth + 1);
    return depth + leftSum + rightSum;
}

