/*
  File name: main.cpp
  Name: Chris Chen
  Date: April 13th, 2024
  This is the main file of the program.
*/

#include "threadedBST.h"
#include <iostream>

using std::cout;

void test1() { // size 1
  int n = 1;
  ThreadedBST orig_tree(n);         // creating a tree
  ThreadedBST new_tree = orig_tree; // copying original tree
  // remove all evens from newTree
  for (int i = 2; i < n + 1; i += 2) {
    new_tree.remove(i);
  }
  // output
  cout << "Original Tree: " << orig_tree << endl;
  cout << "New Tree: " << new_tree << endl << endl;
}

void test2() { // size 4
  int n = 4;
  ThreadedBST orig_tree(n);         // creating a tree
  ThreadedBST new_tree = orig_tree; // copying original tree
  // remove all evens from newTree
  for (int i = 2; i < n + 1; i += 2) {
    new_tree.remove(i);
  }
  // output
  cout << "Original Tree: " << orig_tree << endl;
  cout << "New Tree: " << new_tree << endl << endl;
}

int main() {

  test1();
  test2();

  return 0;
}
