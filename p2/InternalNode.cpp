#include <iostream>
#include <math.h>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{
    BTreeNode* returnNode = NULL;
    int i; // will be where value belongs
    
    for(i = count - 1; i > 0 && keys[i] > value; i--);
    returnNode = children[i]->insert(value);
    
    for(i = 0; i < count; ++i){
        keys[i] = children[i]->getMinimum();
    }
    
    // child has enough space.
    if(returnNode == NULL){
        return NULL;
    }
    
    // need to add in a leaf node
    // 1. the internal node has enough space
    if(count < internalSize){
        selfInsert(returnNode);
        return NULL;
    }
    // 2. the internal node doesn't have enough space
    else
        return lookLeft(returnNode);
}// InternalNode::insert()

// shift the array to insert value
void InternalNode::selfInsert(BTreeNode* returnNode){
    
    int key = returnNode->getMinimum();
    int i = 0;
    for(; i < count; ++i)
    {
        if(key < keys[i])
        {
            for(int j = count; j > i; --j){
                keys[j] = keys[j-1];
                children[j] = children[j-1];
            }
            keys[i] = key;
            children[i] = returnNode;
            children[i]->setParent(this);
            ++count;
            break;
        }
    }
    if(i == count){
        keys[count] = key;
        children[count] = returnNode;
        children[count]->setParent(this);
        count++;
    }
}

// look at the left sibling
InternalNode* InternalNode::lookLeft(BTreeNode* returnNode){

    InternalNode* left = (InternalNode*)getLeftSibling();
    // left sibling exist and is not full
    if(left != NULL && left->getCount() < internalSize){
        if(returnNode->getMinimum() < children[0]->getMinimum()){
            left->selfInsert(returnNode);
            return NULL;
        }else{
            left->selfInsert(children[0]);
            // shift itself to make up for the losing minimum
            for(int i = 0; i < count-1; ++i){
                keys[i] = keys[i+1];
                children[i] = children[i+1];
            }
            count--;
            selfInsert(returnNode);
            return NULL;
        }
    }
    // left sibling is full
    else
        return lookRight(returnNode);
}

// look at the right sibling
InternalNode* InternalNode::lookRight(BTreeNode* returnNode){
    
    InternalNode* right = (InternalNode*)getRightSibling();
    // left sibling exist and is not full
    if(right != NULL && right->getCount() < internalSize){
        if(returnNode->getMinimum() > children[count-1]->getMinimum()){
            right->selfInsert(returnNode);
            return NULL;
        }else{
            right->selfInsert(children[count-1]);
            count--;
            selfInsert(returnNode);
            return NULL;
        }
    }
    // right sibling is full
    else
        return split(returnNode);
}

// look at the right sibling
InternalNode* InternalNode::split(BTreeNode* returnNode){
    
    //create a new node
    InternalNode* oldRight = (InternalNode*)getRightSibling();
    InternalNode* newNode = new InternalNode(internalSize,leafSize,parent,this,oldRight);
    
    // set the right sibling pointer for this InternalNode
    setRightSibling(newNode);
    if(oldRight != NULL)// if the old right sibling exist, reset its left pointer
        oldRight->setLeftSibling(newNode);
    
    // set the new node values
    for(int i = (int)ceil(double(internalSize)/2); i < internalSize; ++i){
        newNode->selfInsert(children[i]);
        count--;
    }
    
    // decide which node to insert new value
    if(returnNode->getMinimum() > children[count-1]->getMinimum()){
        newNode->selfInsert(returnNode);
    }
    else{
        selfInsert(returnNode);
        newNode->selfInsert(children[count-1]);
        count--;
    }
    
    return newNode;
}

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
    count = 2;
    
    children[0]=oldRoot;
    keys[0] = oldRoot->getMinimum();
    
    children[1]=node2;
    keys[1] = node2->getMinimum();
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
    // student don't want to use this
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


