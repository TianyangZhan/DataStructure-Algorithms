#include <iostream>
#include <fstream>
#include "CPUTimer.h"
#include "dsexceptions.h"
#include "LinkedList.h"
#include "StackAr.h"
#include "CursorList.h"
#include "QueueAr.h"
#include "StackLi.h"
#include "vector.h"
#include "SkipList.h"

vector<CursorNode <int> > cursorSpace(2500000);
using namespace std;

void RunList(char* filename);
void RunCursorList(char* filename);
void RunStackAr(char* filename);
void RunStackLi(char* filename);
void RunQueueAr(char* filename);
void RunSkipList(char* filename);

int main()
{
  char filename[100]; //the file name
  int choice;
  CPUTimer ct; //initial cpu timer object

  //get users' choices of file
  cout << "Filename >> ";
  cin >> filename;

  do
  {
    cout << "\nADT Menu\n";
    cout << "0. Quit\n";
    cout << "1. LinkedList\n";
    cout << "2. CursorList\n";
    cout << "3. StackAr\n";
    cout << "4. StackLi\n";
    cout << "5. QueueAr\n";
    cout << "6. SkipList\n";
    cout << "Your choice >> ";
    cin >> choice;

    ct.reset();
    switch(choice)
    {
      case 1: RunList(filename); break;
      case 2: RunCursorList(filename); break;
      case 3: RunStackAr(filename); break;
      case 4: RunStackLi(filename); break;
      case 5: RunQueueAr(filename); break;
      case 6: RunSkipList(filename); break;
    }
    cout << "CPU time: " << ct.cur_CPUTime() << endl;
  } while(choice > 0); //get choice and run

  return 0;
}

void RunList(char* filename)
{
  //declare variables
  char command;
  int values;
  List<int> list;

  //open files
  ifstream inf(filename);
  inf.ignore(1000, '\n'); //read the first line
  //read and run commands
  while(inf >> command >> values)
  {
    if(command == 'i')
      list.insert(values, list.zeroth());
    else if(command == 'd')
      list.remove(values);
  }

  return;
}

void RunCursorList(char* filename)
{
  //declare variables
  char command;
  ;
  int values;
  CursorList<int> clist(cursorSpace);

  //open files
  ifstream inf(filename);
  inf.ignore(1000, '\n'); //read the first line
  //read and run commands
  while(inf >> command >> values)
  {
    if(command == 'i')
      clist.insert(values, clist.zeroth());
    else if(command == 'd')
      clist.remove(values);
  }

  return;
}

void RunStackAr(char* filename)
{
  //declare variables
  char command;
  ;
  int values;
  StackAr<int> SAarray(2500000);

  //open files
  ifstream inf(filename);
  inf.ignore(1000, '\n'); //read the first line
  //read and run commands
  while(inf >> command >> values)
  {
    if(command == 'i')
      SAarray.push(values);
    else if(command == 'd')
      SAarray.pop();
  }

  return;
}

void RunStackLi(char* filename)
{
  //declare variables
  char command;
  ;
  int values;
  StackLi<int> SLlist;

  //open files
  ifstream inf(filename);
  inf.ignore(1000, '\n'); //read the first line
  //read and run commands
  while(inf >> command >> values)
  {
    if(command == 'i')
      SLlist.push(values);
    else if(command == 'd')
      SLlist.pop();
  }

  return;
}

void RunQueueAr(char* filename)
{
  //declare variables
  char command;
  ;
  int values;
  Queue<int> Qarray(2500000);

  //open files
  ifstream inf(filename);
  inf.ignore(1000, '\n'); //read the first line
  //read and run commands
  while(inf >> command >> values)
  {
    if(command == 'i')
      Qarray.enqueue(values);
    else if(command == 'd')
      Qarray.dequeue();
  }

  return;
}

void RunSkipList(char* filename)
{
  //declare variables
  char command;
  ;
  int values;
  SkipList<int> SKlist(-1, 2500000);

  //open files
  ifstream inf(filename);
  inf.ignore(1000, '\n'); //read the first line
  //read and run commands
  while(inf >> command >> values)
  {
    if(command == 'i')
      SKlist.insert(values);
    else if(command == 'd')
      SKlist.deleteNode(values);
  }

  return;
}
