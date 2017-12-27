// Author: Sean Davis
#ifndef evacH
#define evacH
#include "QueueAr.h"
#include "EvacRunner.h"

class RankedObj
{
public:
    int ID;
    float rank;
    RankedObj(): ID(-1), rank(-1) {}
};

class CityObj
{
public:
    
    RankedObj *roadID;
    int numRoads;
    int population;
    int evacedNum;
    int depth;      // for BFS
    bool visited;   // for DFS
    
    CityObj(): numRoads(0), population(0), evacedNum(0), depth(-1), visited(false) {}
};

class RoadObj
{
public:
    
    int destinationID;
    int capacityNum;
    int occupiedNum;
    
    RoadObj(): destinationID(-1), capacityNum(0), occupiedNum(0) {}
};

class ObjList
{
public:
    
    int count;
    int* list;
    // only used for evac cities
    RankedObj* elist;
    
    ObjList():count(0) {}
};

class Evac
{
public:
    
    int numCities;
    CityObj* cities;
    RoadObj* roads;
    Queue<int>* queue;          // for BFS
    ObjList evacCities;
    ObjList visitedCities;      // for finding flow
    ObjList usedRoads;          // for marking routes
    
    Evac(City *city, int numCity, int numRoads); // Constructor
    void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes, int &routeCount); // student sets evacRoutes and routeCount
    int PathFlow(int currentID, int incomingFlow); // depth-first search to find the total flow along a path
}; // class Evac

#endif
