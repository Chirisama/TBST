/*
  File name: threadedBST.cpp
  Name: Chris Chen
  Date: April 13th, 2024
  This .cpp file contains all functions definition for threadedBST.h
*/

#include "threadedBST.h"
#include <algorithm>
#include <iostream>
#include <vector>

// output overload
ostream &operator<<(ostream &out, const ThreadedBST &bst) {
  vector<int> v = bst.preorderTraversal();
  for (int i = 0; i < v.size(); i++) { // NOLINT
    out << v[i] << " ";
  }
  return out;
}

// operator== overload
bool ThreadedBST::operator==(const ThreadedBST &bst) const {
  vector<int> v = inorderTraversal();
  vector<int> b = bst.inorderTraversal();
  if (v.size() != b.size())
    return false;
  for (int i = 0; i < v.size(); i++) {
    if (v[i] != b[i]) {
      return false;
    }
  }
  return true;
}
// assignment overload for copying
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
ThreadedBST &ThreadedBST::operator=(const ThreadedBST &bst) {
  if (*this == bst) {
    return *this;
  }
  *this = bst;

  return *this;
}

// constructor
// n equals the total number of nodes in the tree
ThreadedBST::ThreadedBST(int n) : totalNodes{n} {
  // vector for adding inputs
  vector<int> nums;

  for (int i = 1; i <= n; i++) {
    nums.push_back(i);
  }

  // assign headptr and start recursive call to build tree, then thread it
  headPtr = buildSubTree(nums, 0, nums.size() - 1);
  int balance = checkBalance();
  if (abs(balance) > 1) {
    vector<Node *> inorder;
    balanceInorder(headPtr, inorder);
    int size = inorder.size();

    headPtr = buildTree(0, size - 1, inorder);
  }
  threadTree(headPtr);
}

// helper function to do self-balancing, place nodes in inorder.
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
void ThreadedBST::balanceInorder(Node *root, vector<Node *> &inorder) {
  if (root == nullptr)
    return;
  balanceInorder(root->leftPtr, inorder);
  inorder.push_back(root);
  if (root->isThread) {
    root->isThread = false;
    root->rightPtr = nullptr;
  }
  balanceInorder(root->rightPtr, inorder);
}

// helper function to do self-balancing, reconstruct the tree in in order.
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
Node *ThreadedBST::buildTree(int start, int end, vector<Node *> &inorder) {

  if (start > end) {
    return nullptr;
  }
  int mid = (start + end) / 2;
  Node *root = inorder[mid];
  root->isThread = false;
  root->leftPtr = buildTree(start, mid - 1, inorder);
  root->rightPtr = buildTree(mid + 1, end, inorder);
  return root;
}

// helper function to thread a tree
void ThreadedBST::threadTree(Node *headPtr) {
  // find left most child of tree
  Node *current_node = headPtr;
  while (current_node->leftPtr != nullptr) {
    current_node = current_node->leftPtr;
  }
  if (current_node->rightPtr != nullptr) {
    current_node = current_node->rightPtr;
  }
  // traverse tree and create threads when necessary
  while (current_node->value < totalNodes) {
    if (current_node->rightPtr == nullptr) { // thread needed
      // create thread and follow it
      current_node->rightPtr = getEntry(current_node->value + 1);
      current_node->isThread = true;
      current_node = current_node->rightPtr;

      // new current_node will always have a right child, follow again
      current_node = current_node->rightPtr;
    } else if (current_node->leftPtr != nullptr) {
      // explore left child/subtree
      current_node = current_node->leftPtr;
    } else {
      // explore right child/subtree
      current_node = current_node->rightPtr;
    }
  }
}

// helper function to build subtrees recursively
// returns a Node* to make recursion possible
Node *ThreadedBST::buildSubTree(const vector<int> &nums, int lowerIndex,
                                int upperIndex) {
  Node *head = nullptr;
  while (lowerIndex <= upperIndex) {
    if (lowerIndex == 0) {
      head = new Node;
      head->value = nums[lowerIndex];
      headPtr = head;

    } else {
      head = addNode(head, nums[lowerIndex]);
    }
    lowerIndex++;
  }
  return head;
}

// helper function to add a node into the tree.
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
Node *ThreadedBST::addNode(Node *root, int value) {
  if (root == nullptr) {
    root = new Node;
    root->value = value;
  } else if (root->value >= value) {

    root->leftPtr = addNode(root->leftPtr, value);
  } else {
    root->rightPtr = addNode(root->rightPtr, value);
  }
  return root;
}

// copy constructor
ThreadedBST::ThreadedBST(const ThreadedBST &tbst) {
  vector<int> v = tbst.inorderTraversal();
  headPtr = buildSubTree(v, 0, v.size() - 1);
  int balance = checkBalance();
  if (abs(balance) > 1) {
    vector<Node *> inorder;
    balanceInorder(headPtr, inorder);
    int size = inorder.size();

    headPtr = buildTree(0, size - 1, inorder);
  }
  // NOLINTNEXTLINE, the solution clang-tidy suggests crashes.
  for (int i = 0; i < v.size(); i++) {
    if (tbst.getEntry(v[i])->isThread) {
      getEntry(v[i])->isThread = true;
      getEntry(v[i])->rightPtr = getEntry(tbst.getEntry(v[i])->rightPtr->value);
    }
  }
}

// destructor
ThreadedBST::~ThreadedBST() { clear(headPtr); }

// clear helper function to recursively delete TBST
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
void ThreadedBST::clear(Node *node) {
  if (node == nullptr) {
    return;
  }
  if (node->leftPtr != nullptr) {
    clear(node->leftPtr);
  }
  if (node->rightPtr != nullptr && !node->isThread) {
    clear(node->rightPtr);
  }
  delete node;
}

// searches Tree for a node with specified value
// returns Node if found, nullptr if not found
Node *ThreadedBST::getEntry(int n) const {
  Node *ptr = headPtr;
  while (ptr != nullptr) {
    if (n > ptr->value) {
      ptr = ptr->rightPtr;
    } else if (n < ptr->value) {
      ptr = ptr->leftPtr;
    } else {
      return ptr;
    }
  }
  return nullptr;
}

// returns true if empty, false if not
bool ThreadedBST::isEmpty() const { return (headPtr == nullptr); }

// iterator to do inorder traversal of the tree
vector<int> ThreadedBST::inorderTraversal() const {
  vector<int> v;
  inOrder(headPtr, v);
  return v;
}

// helper function for inorder traversal:
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
void ThreadedBST::inOrder(Node *root, vector<int> &v) const {
  if (!root) {
    return;
  }
  inOrder(root->leftPtr, v);
  v.push_back(root->value);
  if (!root->isThread) {
    inOrder(root->rightPtr, v);
  }
}

// iterator to do preorder traversal of the tree
vector<int> ThreadedBST::preorderTraversal() const {
  vector<int> v;
  preOrder(headPtr, v);
  return v;
}

// helper function for preorder traversal:
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
void ThreadedBST::preOrder(Node *root, vector<int> &v) const {
  if (!root)
    return;
  v.push_back(root->value);
  preOrder(root->leftPtr, v);
  if (!root->isThread) {
    preOrder(root->rightPtr, v);
  }
}

// iterator to do postorder traversal of the tree
vector<int> ThreadedBST::postorderTraversal() const {
  vector<int> v;
  postOrder(headPtr, v);
  return v;
}

// helper function for postorder traversal:
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
void ThreadedBST::postOrder(Node *root, vector<int> &v) const {
  if (!root)
    return;
  postOrder(root->leftPtr, v);
  if (!root->isThread) {
    postOrder(root->rightPtr, v);
  }
  v.push_back(root->value);
}

// removes a node
Node *ThreadedBST::remove(int value) {
  Node *node = removeHelper(headPtr, value);

  vector<Node *> inorder;
  balanceInorder(headPtr, inorder);
  int size = inorder.size();

  headPtr = buildTree(0, size - 1, inorder);
  // threadTree(headPtr);
  return node;
}

// helper function for removing a node
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
Node *ThreadedBST::removeHelper(Node *node, int value) {

  if (node == nullptr) {
    return node;
  }
  if (value > node->value) {
    node->rightPtr = removeHelper(node->rightPtr, value);
  } else if (value < node->value) {
    node->leftPtr = removeHelper(node->leftPtr, value);
  } else if (value == node->value) {
    // two children
    if (node->leftPtr != nullptr && node->rightPtr != nullptr) {
      Node *temp = seekLeftmost(node->rightPtr);
      node->value = temp->value;
      node->rightPtr = removeHelper(node->rightPtr, node->value);
    }
    // one right child
    else if (node->leftPtr == nullptr && !node->isThread) {
      Node *temp = node;
      node = node->rightPtr;
      delete temp;
    }
    // one left child or no child
    else {
      Node *temp = node;
      node = node->leftPtr;
      delete temp;
    }
  }
  return node;
}

// helper function for finding the leftmost node to succeed(for deletion).
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
Node *ThreadedBST::seekLeftmost(Node *node) {
  if (node == nullptr) {
    return nullptr;
  }
  if (node->leftPtr == nullptr) {
    return node;
  }
  return seekLeftmost(node->leftPtr);
}

// check depth of a side of node given in the parathesis.
// NOLINTNEXTLINE, intend to use recursion, ignore misc-no-recursion warning.
int ThreadedBST::checkBalance(Node *node) {
  if (node == nullptr)
    return 0;
  int max_left = checkBalance(node->leftPtr);
  int max_right = checkBalance(node->rightPtr);

  // cout << "Total difference: " << maxLeft - maxRight + 1 << endl;
  return max(max_left, max_right) + 1;
}

// check depth difference between left and right nodes.
int ThreadedBST::checkBalance() {
  int left = checkBalance(headPtr->leftPtr);
  int right = checkBalance(headPtr->rightPtr);
  return left - right;
}
