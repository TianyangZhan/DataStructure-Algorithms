#include <iostream>
#include "defragmenter.h"
#include "DefragRunner.h"
#include "mynew.h"
#include "QuadraticProbing.h"
#include "BinaryHeap.h"

Defragmenter::Defragmenter(DiskDrive *diskDrive)
{
    // storage in RAM for DiskBlocks
    int storeSize = 11500;
    QuadraticHashTable<DiskBlock*> * RAMstore = new QuadraticHashTable<DiskBlock*>(NULL, storeSize);
    // storage in RAM for moved IDs of DiskBlocks
    int tableSize = 22500;
    QuadraticHashTable<unsigned> * indexTable = new QuadraticHashTable<unsigned>(-1, tableSize);
    // storage in RAM for free IDs of DiskBlocks
    int heapSize = 205000;
    BinaryHeap <unsigned> * heap = new BinaryHeap <unsigned> (heapSize);
    
    int count = 0;
    int numFiles = diskDrive->getNumFiles();
    // store unused BolckID in diskdrive in binary heap (max heap)
    for (unsigned i = diskDrive->getCapacity() - 1; i >= 2; i--){
        if (!(diskDrive->FAT[i])){
            if(count < heapSize){
                heap->insert(i);
                ++count;
            }
        }
    }
    
    unsigned driveIndex = 2;
    for(int i = 0; i < numFiles; ++i){
        
        // set the first disblock
        unsigned blockID = diskDrive->directory[i].getFirstBlockID();
        diskDrive->directory[i].setFirstBlockID(driveIndex);
      
        // go through the whole file
        // EndOfFile <==> blockID == 1)
        while(blockID != 1){
            unsigned currentID = 0;
            DiskBlock* fragment = NULL;
            findIndex(diskDrive, fragment, blockID, currentID, driveIndex, RAMstore, indexTable, heap, heapSize);
            writeIndex(diskDrive, fragment, blockID, driveIndex, RAMstore, indexTable, heap);
            ++driveIndex;
        }// while
    }// for
    
    //cout << " RAMstore: " << RAMstore->maxSize << " IndexHash: " << indexTable->maxSize <<endl;
} // Defragmenter()



void Defragmenter::findIndex(DiskDrive*& diskDrive,DiskBlock*& fragment, unsigned& blockID, unsigned currentID,
                             unsigned& driveIndex,QuadraticHashTable<DiskBlock*> *& RAMstore,
                             QuadraticHashTable<unsigned> *& indexTable,BinaryHeap <unsigned> *& heap, int heapSize){
    
    if(blockID >= driveIndex){
        // read directly from disk
        fragment = diskDrive->readDiskBlock(blockID);
        if(blockID > driveIndex){
            diskDrive->FAT[blockID] = false;
            if (heap->size() < heapSize)
                heap->insert(blockID);
        }
    }else{
        // (blockID < driveIndex), the fragment has been moved
        // read from RAMstore
        fragment = RAMstore->find(blockID);
        if(fragment){
            RAMstore->remove(blockID);
        }else{
        // not in RAMstore, read from free space in diskDrive (index stored in indexTable)
            currentID = indexTable->find(blockID);   // currentID == oldBlockID
            indexTable->remove(blockID);
            
            // if oldBlockID has been put in heap again
            // currentID == oldBlockID
            while (indexTable->find(currentID) != (unsigned)-1){
                indexTable->remove(blockID);
                blockID = currentID;
                currentID = indexTable->find(blockID);
            }
            
            fragment = diskDrive->readDiskBlock(currentID);
            
            if(currentID != driveIndex){
                diskDrive->FAT[currentID] = false;
                if(heap->size() < heapSize)
                    heap->insert(currentID);
            }
        } // inner else
    } // outer else
}




void Defragmenter::writeIndex(DiskDrive*& diskDrive,DiskBlock*& fragment, unsigned& blockID, unsigned& driveIndex,
                              QuadraticHashTable<DiskBlock*> *& RAMstore, QuadraticHashTable<unsigned> *& indexTable,
                              BinaryHeap <unsigned> *& heap){
    
    if(blockID != driveIndex){
    // need to write in diskDrive
        if(diskDrive->FAT[driveIndex]) {
        // if driveIndex is used, move it
            DiskBlock * temp = diskDrive->readDiskBlock(driveIndex);
            
            // put it in RAMstore
            if(RAMstore->getCurrentSize() <= RAMstore->getArraySize() - 50){
                RAMstore->insert(driveIndex, temp);
            }else{
                // put it in diskDrive
                while(diskDrive->FAT[heap->findMax()]){     // find an empty space
                    heap->deleteMax();
                }
                unsigned currentID = heap->findMax();
                heap->deleteMax();
                
                diskDrive->writeDiskBlock(temp, currentID);
                diskDrive->FAT[currentID] = true;
                indexTable->insert(driveIndex, currentID);
                delete temp;
            }
        }
        
        diskDrive->writeDiskBlock(fragment, driveIndex);
        diskDrive->FAT[driveIndex] = true;
        
        blockID = fragment->getNext();
        if(blockID != 1){
        // fragment is not the end of the file
            fragment->setNext(driveIndex + 1);
        }
    }else{
        // (blockID == index)   already good, do nothing
        blockID = fragment->getNext();
    }
    
    delete fragment;
    
}

