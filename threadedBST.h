/*
  File name: threadedBST.h
  Name: Chris Chen
  Date: April 13th, 2024
  This header file contains signatures for all functions and variables.
*/

#pragma once
#ifndef THREADEDBST_H
#define THREADEDBST_H

#include <iostream>
#include <vector>

using namespace std;

class Node {
  friend class ThreadedBST;

private:
  int value;
  Node *leftPtr = nullptr;
  Node *rightPtr = nullptr;
  bool isThread = false; // points at thread or child
};

class ThreadedBST {

  // output overload
  friend ostream &operator<<(ostream &out, const ThreadedBST &bst);

public:
  // assignment overload for copying
  ThreadedBST &operator=(const ThreadedBST &bst);

  // operator== overload
  bool operator==(const ThreadedBST &bst) const;

  // constructor
  // n equals the total number of nodes in the tree
  explicit ThreadedBST(int n);

  // copy constructor
  ThreadedBST(const ThreadedBST &tbst);

  // Add a new node and rebalance
  Node *addNode(Node *root, int value);

  // rebalancing the tree;
  void rebalance();

  // checking if this tree is balanced:
  int checkBalance(Node *node);

  // check depth difference between left and right nodes.
  int checkBalance();

  // helper function to do self-balancing, place nodes in inorder.
  void balanceInorder(Node *root, vector<Node *> &inorder);

  // helper function to do self-balancing, reconstruct the tree in in order.
  Node *buildTree(int start, int end, vector<Node *> &inorder);

  // destructor
  ~ThreadedBST();

  // helped function to find parent
  Node *getParent(int n) const;

  // removes specific element
  // returns true if successful, false if not
  Node *remove(int value);

  // iterator to do inorder traversal of the tree
  vector<int> inorderTraversal() const;

  // iterator to do preorder traversal of the tree
  vector<int> preorderTraversal() const;

  // iterator to do postorder traversal of the tree
  vector<int> postorderTraversal() const;

private:

  // Helper function for in-order traversal:
  void inOrder(Node *root, vector<int> &v) const;

  // Helper function for pre-order traversal:
  void preOrder(Node *root, vector<int> &v) const;

  // Helper function for post-order traversal:
  void postOrder(Node *root, vector<int> &v) const;

  // customized helper function for removing a specific node.
  Node *removeHelper(Node *node, int value);

  // customized helper function to seek for leftmost child for a selected node.
  Node *seekLeftmost(Node *node);

  // helper function to thread a tree
  void threadTree(Node *headPtr);

  // clear helper function to recursively delete TBST
  void clear(Node *node);

  // returns true if empty, false if not
  bool isEmpty() const;

  // searchs Tree for a node with specified value
  // returns Node if found, nullptr if not found
  Node *getEntry(int n) const;

  // helper function to build subtrees recursively
  // returns a Node* to make recursion possible
  Node *buildSubTree(const vector<int> &nums, int lower, int upper);

  // root pointer (top of tree)
  Node *headPtr = nullptr;

  // total number of nodes in the tree
  int totalNodes;
};

#endif