#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;
void preorderString(EncodingTreeNode* root, Queue<Bit>& returnString);
void inorderString(EncodingTreeNode* root, Queue<char>& returnString);
/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation. 011 100 11
 */

void decodeText_(EncodingTreeNode* tree, Queue<Bit> messageBits, string& returnString)
{
    if (messageBits.isEmpty())
    {
        return;
    }
    EncodingTreeNode* temp_root = tree;
    while (!temp_root->isLeaf())
    {
        if (messageBits.dequeue() == 0)
        {
            temp_root = temp_root->zero;
        }
        else
        {
            temp_root = temp_root->one;
        }
    }
    returnString += temp_root->ch;
    decodeText_(tree, messageBits, returnString);
}

string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    /* TODO: Implement this function. */
    string returnString = {};
    decodeText_(tree, messageBits, returnString);
    return returnString;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void preorderReconstruction_(Queue<Bit>& treeShape, EncodingTreeNode* &root)
{
    if (treeShape.isEmpty())
    {
        return;
    }

    if (treeShape.dequeue() == 0)
    {
        root = new EncodingTreeNode('A');
        return;
    }
    else
    {
        root = new EncodingTreeNode(nullptr, nullptr);
    }
    preorderReconstruction_(treeShape, root->zero);
    preorderReconstruction_(treeShape, root->one);
}

EncodingTreeNode* preorderReconstruction(Queue<Bit>& treeShape)
{
    EncodingTreeNode* root = nullptr;
    preorderReconstruction_(treeShape, root);
    return root;
}

void inorderReconstruction(Queue<char>& treeLeaves, EncodingTreeNode* root)
{
    if (root == nullptr)
    {
        return;
    }
    inorderReconstruction(treeLeaves, root->zero);
    if (root->isLeaf())
    {
        root->ch = treeLeaves.dequeue();
    }
    inorderReconstruction(treeLeaves, root->one);
}

EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    EncodingTreeNode* root = preorderReconstruction(treeShape);
    inorderReconstruction(treeLeaves, root);
    return root;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Implement this function. */
    Queue<Bit>  treeShape = data.treeShape;
    Queue<char> treeLeaves = data.treeLeaves;
    Queue<Bit>  messageBits = data.messageBits;
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);
    string result = decodeText(tree, messageBits);
    deallocateTree(tree);
    return result;
}

string upperString(string text)
{
    string newText = {};
    for (char c : text)
    {
        if (isalpha(c))
        {
            newText += toupper(c);
        }
        else
            newText += c;
    }
    return newText;
}

std::map<char, int> computateFrequency(string Text)
{
    std::map<char, int> dict;
    for (char c : Text)
    {
        std::map<char, int>::iterator it = dict.find(c);
        if (it != dict.end())
        {
            it->second ++;
        }
        else
        {
            dict[c] = 1;
        }
    }
    return dict;
}

class cmp{
public:
    bool operator()(std::pair<EncodingTreeNode*, int> & a, std::pair<EncodingTreeNode*, int>& b){
        return a.second >= b.second;
    }
};
EncodingTreeNode* generateTree(std::map<char, int> dict)
{
    std::priority_queue<std::pair<EncodingTreeNode*, int>, vector<std::pair<EncodingTreeNode*, int>>, cmp> q;
    for (std::map<char, int>::iterator it = dict.begin(); it != dict.end(); it++)
    {
        EncodingTreeNode* node = new EncodingTreeNode(it->first);
        std::pair<EncodingTreeNode*, int> p(node, it->second);
        q.push(p);
    }

    while(q.size() >= 2)
    {
        std::pair<EncodingTreeNode*, int> l = q.top();
        q.pop();
        std::pair<EncodingTreeNode*, int> r = q.top();
        q.pop();
        EncodingTreeNode* node = new EncodingTreeNode(l.first, r.first);
        std::pair<EncodingTreeNode*, int> newP(node, l.second + r.second);
        q.push(newP);
    }
    return q.top().first;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Implement this function. */

    /*
     * 1) 统计词频
     * 2）建树
     **/

    // text = upperString(text);
    std::map<char, int> dict = computateFrequency(text);
    return generateTree(dict);
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */

void buildDictionary_(EncodingTreeNode* tree, Queue<Bit> temp, std::map<char, Queue<Bit>>& dict)
{
    if (tree->isLeaf())
    {
        // dict[toupper(tree->getChar())] = temp;
        dict[tree->getChar()] = temp;
        return;
    }

    Queue<Bit> temp_0 = temp;
    temp_0.enqueue(0);
    Queue<Bit> temp_1 = temp;
    temp_1.enqueue(1);

    buildDictionary_(tree->zero, temp_0, dict);
    buildDictionary_(tree->one, temp_1, dict);
}

std::map<char, Queue<Bit>> buildDictionary(EncodingTreeNode* tree)
{
    std::map<char, Queue<Bit>> result;
    Queue<Bit> temp;
    buildDictionary_(tree, temp, result);
    return result;
}

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    std::map<char, Queue<Bit>> dictionary = buildDictionary(tree);
    Queue<Bit> res;
    for (char c : text)
    {
        // c = toupper(c);
        Queue<Bit> t = dictionary[c];
        while(!t.isEmpty())
        {
            res.enqueue(t.dequeue());
        }
    }
    return res;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    preorderString(tree, treeShape);
    inorderString(tree, treeLeaves);
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Implement this function. */
    EncodingTreeNode* tree = buildHuffmanTree(messageText);
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    Queue<Bit> messagebits = encodeText(tree, messageText);
    flattenTree(tree, treeShape, treeLeaves);
    EncodedData res;
    res.treeLeaves = treeLeaves;
    res.treeShape = treeShape;
    res.messageBits = messagebits;

    deallocateTree(tree);
    return res;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */

    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    EncodingTreeNode* sublchild = new EncodingTreeNode(R, S);
    EncodingTreeNode* rchild = new EncodingTreeNode(sublchild, E);
    EncodingTreeNode* root = new EncodingTreeNode(T, rchild);
    return root;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    if (t == nullptr)
    {
        return;
    }
    deallocateTree(t->zero);
    deallocateTree(t->one);
    delete[] t;
}

void preorderString(EncodingTreeNode* root, Queue<Bit>& returnString)
{
    /*
     * tree shape
     **/

    if (root == nullptr)
    {
        return;
    }
    if (root->isLeaf())
    {
        returnString.enqueue(0);
    }
    else
    {
        returnString.enqueue(1);
    }
    preorderString(root->zero, returnString);
    preorderString(root->one, returnString);
}

void inorderString(EncodingTreeNode* root, Queue<char>& returnString)
{
    /*
    * tree leaves
    **/
    if (root == nullptr)
    {
        return;
    }
    if (root->zero == nullptr)
    {
        returnString.enqueue(root->ch);
    }
    inorderString(root->zero, returnString);
    inorderString(root->one, returnString);
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* TODO: Implement this utility function needed for testing. */
    /*
    * 先序和中序遍历，中序和后序遍历都能唯一确定一颗子树
    * 用先序和中序遍历来判断是否相等
    **/

    // string preorderString_a = {};
    // string preorderString_b = {};
    // string inorderString_a = {};
    // string inorderString_b = {};
    // preorderString(a, preorderString_a);
    // preorderString(b, preorderString_b);
    // inorderString(a, inorderString_a);
    // inorderString(b, inorderString_b);
    // if (preorderString_a == preorderString_b && inorderString_a == inorderString_b)
    // {
    //     return true;
    // }

    // return false;

    // 同步遍历两棵树
    if (a == nullptr && b == nullptr)
    {
        return true;
    }
    if (a == nullptr || b == nullptr)
    {
        return false;
    }
    if (a->isLeaf() && b->isLeaf())
    {
        if (a->getChar() == b->getChar())
        {
            return true;
        }
        else
            return false;
    }
    if (a->isLeaf() || b->isLeaf())
    {
        return false;
    }

    if (a->zero != nullptr && b->zero != nullptr)
    {
        return areEqual(a->zero, b->zero);
    }
    if (a->one != nullptr && b->one != nullptr)
    {
        return areEqual(a->one, b->one);
    }
    return false;

}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */

PROVIDED_TEST("Equal Test")
{
    // Singleton tree vs empty tree
    EncodingTreeNode* singletonTree = new EncodingTreeNode('A');
    EncodingTreeNode* emptyTree = nullptr;
    EXPECT_EQUAL(areEqual(singletonTree, emptyTree), false);
    deallocateTree(singletonTree);
    deallocateTree(emptyTree);

    // Singleton tree vs Singleton tree
    EncodingTreeNode* singletonTree_a = new EncodingTreeNode('A');
    EncodingTreeNode* singletonTree_b = new EncodingTreeNode('A');
    EncodingTreeNode* singletonTree_c = new EncodingTreeNode('B');
    EXPECT_EQUAL(areEqual(singletonTree_a, singletonTree_b), true);
    EXPECT_EQUAL(areEqual(singletonTree_a, singletonTree_c), false);
    deallocateTree(singletonTree_a);
    deallocateTree(singletonTree_b);
    deallocateTree(singletonTree_c);

    // example tree vs singleton tree
    EncodingTreeNode* singletonTree_d = new EncodingTreeNode('C');
    EncodingTreeNode* tree_c = createExampleTree();
    EXPECT_EQUAL(areEqual(singletonTree_d, tree_c), false);
    deallocateTree(singletonTree_d);
    deallocateTree(tree_c);

    // example tree vs example tree
    EncodingTreeNode* tree_a = createExampleTree();
    EncodingTreeNode* tree_b = createExampleTree();
    EXPECT_EQUAL(areEqual(tree_a, tree_b), true);
    deallocateTree(tree_a);
    deallocateTree(tree_b);

    // example subtree cs example subtree
    EncodingTreeNode* tree_d = createExampleTree();
    EXPECT_EQUAL(areEqual(tree_d->zero, tree_d->one), false);
    deallocateTree(tree_d);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
