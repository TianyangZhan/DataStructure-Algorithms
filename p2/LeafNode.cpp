#include <iostream>
#include <math.h>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()



int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
    // 1. the leaf node has enough space
    if(count < leafSize){
        selfInsert(value);
      return NULL;
    }
    // 2. the leaf node doesn't have enough space
    else
        return lookLeft(value);
}// LeafNode::insert()

// shift the array to insert value
void LeafNode::selfInsert(int value){
    
    int i = 0;
    for(; i < count; ++i)
    {
        if(value < values[i])
        {
            for(int j = count; j > i; --j){
                values[j] = values[j-1];
            }
            values[i] = value;
            ++count;
            break;
        }
    }
    if(i == count){
        values[count] = value;
        count++;
    }
}

// look at the left sibling
LeafNode* LeafNode::lookLeft(int value){
    
    LeafNode* left = (LeafNode*)getLeftSibling();
    // left sibling exist and is not full
    if(left != NULL && left->getCount() < leafSize){
        if(value < values[0]){
            left->selfInsert(value);
            return NULL;
        }else{
            left->selfInsert(getMinimum());
            // shift itself
            for(int i = 0; i < count-1; ++i){
                values[i] = values[i+1];
            }
            count--;
            selfInsert(value);
            return NULL;
        }
    }
    // left sibling is full
    else
        return lookRight(value);
}

// look at the right sibling
LeafNode* LeafNode::lookRight(int value){
    
    LeafNode* right = (LeafNode*)getRightSibling();
    // right sibling exist and is not full
    if(right != NULL && right->getCount() < leafSize){
        if(value > values[count-1]){
            right->selfInsert(value);
            return NULL;
        }else{
            right->selfInsert(values[count-1]);
            count--;
            selfInsert(value);
            return NULL;
        }
    }
    // right sibling is full
    else
        return split(value);
}

// look at the right sibling
LeafNode* LeafNode::split(int value){
    
    //create a new node
    LeafNode* oldRight = (LeafNode*)getRightSibling();
    LeafNode* newNode = new LeafNode(leafSize,parent,this,oldRight);
    
    // set the right sibling pointer for this LeafNode
    setRightSibling(newNode);
    if(oldRight != NULL)// if the old right sibling exist, reset its left pointer
        oldRight->setLeftSibling(newNode);
    
    // set the new node values
    for(int i = (int)ceil(double(leafSize)/2); i < leafSize; ++i){
        newNode->selfInsert(values[i]);
        count--;
    }
    
    // decide which node to insert new value
    if(value > values[count-1]){
        newNode->selfInsert(value);
    }
    else{
        selfInsert(value);
        newNode->selfInsert(values[count-1]);
        count--;
    }
    
    return newNode;
}

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


