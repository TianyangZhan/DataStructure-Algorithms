#include <cstdlib>
#include <cmath>
#include "evac.h"
#include "QueueAr.h"
#include "EvacRunner.h"
using namespace std;

int min(int a, int b){
   if(a < b)
       return a;
    else
        return b;
}

// qsort function (-1 because of decending order)
int Ranking(const void *ptr, const void *ptr2){ return -1 * ( ((RankedObj*)ptr)->rank - ((RankedObj*)ptr2)->rank ); }


Evac::Evac(City *city, int numCity, int numRoads){
    
    // ********* create an adjacency list ***********
    numCities = numCity;
    cities = new CityObj[numCities];
    roads = new RoadObj[2 * numRoads];
    visitedCities.list = new int[numCities];
    usedRoads.list = new int[numRoads * numCities];

    // cities are vertices
    for(int i = 0; i < numCity; ++i)
    {
        int roadNum = city[i].roadCount;
        cities[city[i].ID].population = city[i].population;
        cities[city[i].ID].evacedNum = 0;
        cities[city[i].ID].numRoads = roadNum;
        cities[city[i].ID].roadID = new RankedObj[roadNum];

        // roads are edges
        for(int j = 0; j < roadNum; ++j){
            cities[ city[i].ID ].roadID[j].ID = city[i].roads[j].ID;
            roads[ city[i].roads[j].ID ].destinationID = city[i].roads[j].destinationCityID;
            // weight is PeoplePerHour on the road
            roads[ city[i].roads[j].ID ].capacityNum = city[i].roads[j].peoplePerHour;
        } 
    }
} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes, int &routeCount){
    
    int timeSpent = 1;   // the first round is done in time = 1
    routeCount = 0;  // initial route = 0
    queue = new Queue<int> (numCities);   // create a Queue for evac cities to do bfs to find depth
    evacCities.count = numEvacs;
    evacCities.elist = new RankedObj[evacCities.count];
    
    // push all evac cities into queue
    for(int i = 0; i < evacCities.count; ++i){
        int ID = evacIDs[i];
        cities[ID].depth = 1;
        queue->enqueue(ID);
        evacCities.elist[i].ID = ID;
    }

    // ************ BFS **************
    // to cut down uneeded roads
    while( !(queue->isEmpty()) ){
        int ID = queue->dequeue();
        // Find roads to other cities
        for(int j = 0; j < cities[ID].numRoads;j++){
            int destinationID = roads[cities[ID].roadID[j].ID].destinationID;
            // if the destination city hasn't been visited, push it into queue
            if(cities[destinationID].depth == -1){
                cities[destinationID].depth = cities[ID].depth + 1;
                queue->enqueue(destinationID);
            }
            
            int roadCap = min(cities[roads[ cities[ID].roadID[j].ID].destinationID].population, roads[ cities[ID].roadID[j].ID].capacityNum);
            int destDepth = cities[roads[ cities[ID].roadID[j].ID].destinationID].depth;
            int numDestRoads = cities[roads[ cities[ID].roadID[j].ID].destinationID].numRoads;
            cities[ID].roadID[j].rank = (float)(numDestRoads * pow(2,roadCap / 500) * pow(50,destDepth));
            /*// faster version on my computer but not on csif  <-- ??
            int roadCap = min(cities[roads[ cities[ID].roadID[j].ID].destinationID].population, roads[ cities[ID].roadID[j].ID].capacityNum);
            int depth = cities[roads[ cities[ID].roadID[j].ID].destinationID].depth;
            int numDestRoads = cities[roads[ cities[ID].roadID[j].ID].destinationID].numRoads;
            cities[ID].roadID[j].rank = (float)(pow(numDestRoads,2) * pow(3,roadCap/1500) * pow(depth,5*2));
            */
            // delete unnecessary roads
            if(cities[destinationID].depth != -1 && cities[destinationID].depth < cities[ID].depth)
            {
                cities[ID].numRoads--;
                cities[ID].roadID[j].ID = cities[ID].roadID[cities[ID].numRoads].ID;
                cities[ID].roadID[cities[ID].numRoads].ID = cities[ID].roadID[j].ID;
                j--; // don't increment i because of the switch
            }
        } // for loop
    } // while( !(queue->isEmpty()) )
    
    do{
        // ********** Sort the cities that needed to be evacuated *********
        for(int i = 0; i < evacCities.count; i++){
            int ID = evacCities.elist[i].ID;
            // if there are people left
            if(cities[ID].evacedNum < cities[ID].population){
                int totalCap = 0;
                // calculate and sum up each road's capacity
                for(int j = 0; j < cities[ID].numRoads; j++){
                    totalCap += roads[cities[ID].roadID[j].ID].capacityNum;
                }
                evacCities.elist[i].rank = cities[ evacCities.elist[i].ID ].numRoads * (cities[ID].population - cities[ID].evacedNum) / (1 * totalCap); // rank the cities based on the ratio of its leftover population and average road capacity (!!)
            }else{
                evacCities.elist[i] = evacCities.elist[evacCities.count - 1];
                (evacCities.count)--;
                i--;    // don't increment i because of the switch
            }
        }
        
        
        // else (Still Need To Evacuate)
        // sort the evac cities
        qsort(evacCities.elist, evacCities.count, sizeof(RankedObj), Ranking);
        // sort the roads
        for(int j = 0; j < numCities; ++j){
            qsort(cities[j].roadID, cities[j].numRoads, sizeof(RankedObj), Ranking);
        }
        
        // ********* Evacuate Cities ***********
        for(int i = 0; i < evacCities.count; i++){
            
            while(visitedCities.count > 0){
                cities[visitedCities.list[--(visitedCities.count)]].visited = false;
            }
            
            // **************** DFS *****************
            int cityID = evacCities.elist[i].ID;
            int leftOver = cities[cityID].population - cities[cityID].evacedNum;
            cities[cityID].evacedNum += PathFlow(cityID,leftOver);
            
        } // for evacCities
    
        // This hour is donw
        // mark all evacRoutes
        for(int i = 0; i < usedRoads.count; i++){
            
            evacRoutes[routeCount].roadID = usedRoads.list[i];
            evacRoutes[routeCount].numPeople = roads[usedRoads.list[i]].occupiedNum;
            evacRoutes[routeCount].time = timeSpent;
            // refresh the roads
            roads[usedRoads.list[i]].occupiedNum = 0;
            
            routeCount++;
        }
        usedRoads.count = 0;
        timeSpent++;
    }while(evacCities.count > 0); // do_while loop
} // evacuate

// DFS to find the total flow along a path
// recursive call Pathflow()
int Evac::PathFlow(int currentID, int incomingFlow){
    
    int totalFlow = 0;
    
    // BaseCase
    // check if the city has been visited
    if(!cities[currentID].visited){
        cities[currentID].visited = true;
        visitedCities.list[(visitedCities.count)++] = currentID;
    }else{
        return 0;
    }
    
    // if the city is not marked as evacCity (the city need to receive evacuees)
    if(cities[currentID].depth != 1){
        
        int cityCap = cities[currentID].population - cities[currentID].evacedNum;
        if(incomingFlow > cityCap){ // if there is not enough room
            totalFlow += cityCap;
            cities[currentID].evacedNum += cityCap;
        }else{
            totalFlow = incomingFlow;
            cities[currentID].evacedNum += totalFlow;
            return totalFlow;
        }
    }

    // RecursiveCase
    // if there not enough room, need to move people out of current city
    for(int i = 0; i < cities[currentID].numRoads && totalFlow - incomingFlow < 0; ++i){
        
        int roadID = cities[currentID].roadID[i].ID;
        if(cities[ roads[roadID].destinationID ].depth >= cities[currentID].depth){
            int roadCap = roads[roadID].capacityNum - roads[roadID].occupiedNum;
            int leftOver = incomingFlow - totalFlow;
            
            int maxFlow = PathFlow(roads[roadID].destinationID, min(roadCap, leftOver));
        
            if(maxFlow > 0){
                totalFlow += maxFlow;
                roads[roadID].occupiedNum += maxFlow;
                usedRoads.list[usedRoads.count] = roadID;
                (usedRoads.count)++;
            }
        }
    }
    return totalFlow;
}
