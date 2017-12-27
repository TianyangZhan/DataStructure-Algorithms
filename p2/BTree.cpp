#include <iostream>
#include "BTree.h"
#include "BTreeNode.h"
#include "LeafNode.h"
#include "InternalNode.h"
using namespace std;

BTree::BTree(int ISize, int LSize):internalSize(ISize), leafSize(LSize)
{
  root = new LeafNode(LSize, NULL, NULL, NULL);
} // BTree::BTree()


void BTree::insert(const int value)
{
    // insert value
    BTreeNode* returnNode = root->insert(value);
    
    // if split
    if(returnNode != NULL)
    {
        BTreeNode *oldRoot = root;
        // set up the new root
        InternalNode *newRoot= new InternalNode(internalSize, leafSize, NULL, NULL, NULL);
        newRoot->insert(oldRoot,returnNode);
        
        // set parent pointers
        oldRoot->setParent(newRoot);
        returnNode->setParent(newRoot);
        root = newRoot;
    }
} // BTree::insert()


void BTree::print()
{
  BTreeNode *BTreeNodePtr;
  Queue<BTreeNode*> queue(1000);

  queue.enqueue(root);
  while(!queue.isEmpty())
  {
    BTreeNodePtr = queue.dequeue();
    BTreeNodePtr->print(queue);
  } // while
} // BTree::print()
