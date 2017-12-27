// Author Sean Davis
#ifndef defragmenterH
  #define defragmenterH

#include "mynew.h"
#include "DefragRunner.h"
#include "QuadraticProbing.h"
#include "QuadraticProbing.h"
#include "BinaryHeap.h"

class Defragmenter
{
public:
  Defragmenter(DiskDrive *diskDrive);
    void findIndex(DiskDrive*& diskDrive,DiskBlock*& fragment, unsigned& blockID, unsigned currentID, unsigned& driveIndex,
                    QuadraticHashTable<DiskBlock*> *& RAMstore, QuadraticHashTable<unsigned> *& indexTable,
                    BinaryHeap <unsigned> *& heap, int heapSize);
    void writeIndex(DiskDrive*& diskDrive,DiskBlock*& fragment, unsigned& blockID,
                    unsigned& driveIndex, QuadraticHashTable<DiskBlock*> *& RAMstore, QuadraticHashTable<unsigned> *& indexTable,
                    BinaryHeap <unsigned> *& heap);
}; // class Defragmenter

#endif
