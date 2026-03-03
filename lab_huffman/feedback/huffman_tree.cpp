/**
 * @file huffman_tree.cpp
 * Implementation of a Huffman Tree class.
 *
 * @author Chase Geigle - Created
 * @author Nathan Walters - Modified
 * @date Summer 2012
 * @date Fall 2017
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>

#include "huffman_tree.h"

using namespace std;

HuffmanTree::HuffmanTree(vector<Frequency> frequencies)
{
    std::stable_sort(frequencies.begin(), frequencies.end());
    buildTree(frequencies);
    vector<bool> path;
    buildMap(root_, path);
}

HuffmanTree::HuffmanTree(const HuffmanTree& other)
{
    copy(other);
}

HuffmanTree::HuffmanTree(BinaryFileReader& bfile)
{
    root_ = readTree(bfile);
    vector<bool> path;
    buildMap(root_, path);
}

HuffmanTree::~HuffmanTree()
{
    clear(root_);
}

const HuffmanTree& HuffmanTree::operator=(const HuffmanTree& rhs)
{
    if (this != &rhs) {
        clear(root_);
        copy(rhs);
    }
    return *this;
}

void HuffmanTree::clear(TreeNode* current)
{
    if (current == NULL)
        return;
    clear(current->left);
    clear(current->right);
    delete current;
}

void HuffmanTree::copy(const HuffmanTree& rhs)
{
    root_ = copy(rhs.root_);
}

HuffmanTree::TreeNode* HuffmanTree::copy(const TreeNode* current)
{
    if (current == NULL)
        return NULL;
    TreeNode* node = new TreeNode(current->freq);
    node->left = copy(current->left);
    node->right = copy(current->right);
    return node;
}

/**
 * Helper function: finds the smallest element on the two queues
 * and removes it. In the event that there is a tie, it should
 * remove the front of the **singleQueue**.
 *
 * @param singleQueue The first queue to inspect.
 * @param mergeQueue The second queue to inspect.
 * @return A pointer to the smallest TreeNode that used to be at
 *    the front of one of the queues.
 */
HuffmanTree::TreeNode*
HuffmanTree::removeSmallest(queue<TreeNode*>& singleQueue,
                            queue<TreeNode*>& mergeQueue)
{

    if (singleQueue.empty() && mergeQueue.empty()) {
        return NULL;
    }


    if (mergeQueue.empty()) {
        TreeNode* smallest = singleQueue.front();
        singleQueue.pop();
        return smallest;
    }

    if (singleQueue.empty()) {
        TreeNode* smallest = mergeQueue.front();
        mergeQueue.pop();
        return smallest;
    }


    TreeNode* singleFront = singleQueue.front();
    TreeNode* mergeFront  = mergeQueue.front();

    if (singleFront->freq.getFrequency() <= mergeFront->freq.getFrequency()) {

        TreeNode* smallest = singleFront;
        singleQueue.pop();
        return smallest;
    } else {
        TreeNode* smallest = mergeFront;
        mergeQueue.pop();
        return smallest;
    }
}

/**
 * Helper function used by the constructor to build a HuffmanTree
 * for a collection of Frequency data. Each Frequency object
 * represents a character and how often it appears in the data to
 * be encoded.
 *
 * @param frequencies The set of Frequency objects to build the
 *    tree with.
 */
void HuffmanTree::buildTree(const vector<Frequency>& frequencies)
{
    queue<TreeNode*> singleQueue; // Queue containing the leaf nodes
    queue<TreeNode*> mergeQueue;  // Queue containing the inner nodes


    for (size_t i = 0; i < frequencies.size(); i++) {
        TreeNode* leaf = new TreeNode(frequencies[i]);
        singleQueue.push(leaf);
    }


    while ((singleQueue.size() + mergeQueue.size()) > 1) {

        TreeNode* leftSmall  = removeSmallest(singleQueue, mergeQueue);

        TreeNode* rightSmall = removeSmallest(singleQueue, mergeQueue);


        int combinedFreq = leftSmall->freq.getFrequency() + rightSmall->freq.getFrequency();
        TreeNode* parent = new TreeNode(combinedFreq);
        parent->left  = leftSmall;
        parent->right = rightSmall;


        mergeQueue.push(parent);
    }


    if (!singleQueue.empty()) {
        root_ = singleQueue.front();
        singleQueue.pop();
    } else if (!mergeQueue.empty()) {
        root_ = mergeQueue.front();
        mergeQueue.pop();
    } else {

        root_ = NULL;
    }
}

/**
 * Decodes a given file into its string contents.
 *
 * @param bfile BinaryFileReader to read bits from.
 * @return The decoded contents of the file.
 */
string HuffmanTree::decodeFile(BinaryFileReader& bfile)
{
    stringstream ss;
    decode(ss, bfile);
    return ss.str();
}

/**
 * Helper function that decodes a file by traversing the tree based on
 * the bits read.
 *
 * @param ss The stringstream being used to build the decoded
 *    output string.
 * @param bfile The binary file we are decoding.
 */
void HuffmanTree::decode(stringstream& ss, BinaryFileReader& bfile)
{
    TreeNode* current = root_;
    while (bfile.hasBits()) {
        bool bit = bfile.getNextBit();
        if (bit == false) { 
            if (current->left != NULL) {
                current = current->left;
            }
        } else {           
            if (current->right != NULL) {
                current = current->right;
            }
        }

        
        if ( (current->left == NULL) && (current->right == NULL) ) {
            ss << current->freq.getCharacter();
            current = root_;
        }
    }
}

/**
 * Writes a compressed version of the tree to the file.
 *
 * @param bfile The binary file to be written to.
 */
void HuffmanTree::writeTree(BinaryFileWriter& bfile)
{
    writeTree(root_, bfile);
}

/**
 * Helper function to write the tree out to a binary file in a
 * compressed way.
 *
 * @param current The root of the subtree we are currently writing.
 * @param bfile The file we are writing to.
 */
void HuffmanTree::writeTree(TreeNode* current, BinaryFileWriter& bfile)
{
    if (current == NULL) {
        return;
    }


    if (current->left == NULL && current->right == NULL) {
        bfile.writeBit(true);
        bfile.writeByte(current->freq.getCharacter());
    } else {

        bfile.writeBit(false);
        writeTree(current->left, bfile);
        writeTree(current->right, bfile);
    }
}

/**
 * Helper function used by the constructor to build a HuffmanTree
 * from a compressed version of the tree written in a binary file.
 *
 * @param bfile The binary file we are reading.
 * @return A pointer to the root of the subtree built.
 */
HuffmanTree::TreeNode* HuffmanTree::readTree(BinaryFileReader& bfile)
{

    if (!bfile.hasBits()) {
        return NULL;
    }

    bool bit = bfile.getNextBit();
    if (bit == true) {

        char c = bfile.getNextByte();
        TreeNode* leaf = new TreeNode( Frequency(c, 0) );
        return leaf;
    } else {

        TreeNode* leftSub  = readTree(bfile);
        TreeNode* rightSub = readTree(bfile);
        TreeNode* parent = new TreeNode(0);
        parent->left  = leftSub;
        parent->right = rightSub;
        return parent;
    }
}

void HuffmanTree::buildMap(TreeNode* current, vector<bool>& path)
{
    // Base case: leaf node.
    if (current->left == NULL && current->right == NULL) {
        bitsMap_[current->freq.getCharacter()] = path;
        return;
    }

    // Move left
    path.push_back(false);
    buildMap(current->left, path);
    path.pop_back();

    // Move right
    path.push_back(true);
    buildMap(current->right, path);
    path.pop_back();
}

void HuffmanTree::printInOrder() const
{
    printInOrder(root_);
    cout << endl;
}

void HuffmanTree::printInOrder(const TreeNode* current) const
{
    if (current == NULL)
        return;
    printInOrder(current->left);
    cout << current->freq.getCharacter() << ":" << current->freq.getFrequency()
         << " ";
    printInOrder(current->right);
}

void HuffmanTree::writeToFile(const string& data, BinaryFileWriter& bfile)
{
    for (auto it = data.begin(); it != data.end(); ++it)
        writeToFile(*it, bfile);
}

void HuffmanTree::writeToFile(char c, BinaryFileWriter& bfile)
{
    vector<bool> bits = getBitsForChar(c);
    for (auto it = bits.begin(); it != bits.end(); ++it)
        bfile.writeBit(*it);
}

vector<bool> HuffmanTree::getBitsForChar(char c)
{
    return bitsMap_[c];
}

// class for generic printing

template <typename TreeNode>
class HuffmanTreeNodeDescriptor
    : public GenericNodeDescriptor<HuffmanTreeNodeDescriptor<TreeNode>>
{
  public:
    HuffmanTreeNodeDescriptor(const TreeNode* root) : subRoot_(root)
    { /* nothing */
    }

    string key() const
    {
        std::stringstream ss;
        char ch = subRoot_->freq.getCharacter();
        int freq = subRoot_->freq.getFrequency();

        // print the sum of the two child frequencies
        if (ch == '\0')
            ss << freq;
        // print the leaf containing a character and its count
        else {
            if (ch == '\n')
                ss << "\\n";
            else
                ss << ch;
            ss << ":" << freq;
        }
        return ss.str();
    }

    bool isNull() const
    {
        return subRoot_ == NULL;
    }
    HuffmanTreeNodeDescriptor left() const
    {
        return HuffmanTreeNodeDescriptor(subRoot_->left);
    }
    HuffmanTreeNodeDescriptor right() const
    {
        return HuffmanTreeNodeDescriptor(subRoot_->right);
    }

  private:
    const TreeNode* subRoot_;
};

int HuffmanTree::height(const TreeNode* subRoot) const
{
    if (subRoot == NULL)
        return -1;
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

void HuffmanTree::print(std::ostream& out) const
{
    int h = height(root_);
    if (h > _max_print_height) {
        out << "Tree is too big to print. Try with a small file (e.g. "
               "data/small.txt)"
            << endl;
        return;
    }

    printTree(HuffmanTreeNodeDescriptor<TreeNode>(root_), out);
}

