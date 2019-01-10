/*
 * pathfinder.cpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-13-17
 *
 * pathfinder.cpp which will be used for bfs / dijkstras on two actors to
 * see the connections between them.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include "ActorGraph.h"
#include <climits>

using namespace std;

ActorNode* BFSTraverse(ActorNode* start, ActorNode* stop);
ActorNode* Dijkstra(ActorNode* start, ActorNode* stop);
void print(ofstream& out, ActorNode* foundActor, std::vector<ActorEdge*> stack);
// class to help compare actor nodes
class ActorNodePtrComp{
  public:
    bool operator()(ActorNode* &lhs,ActorNode* &rhs) const{
      return lhs->index > rhs->index;
    }
};

int main(int argc, char** argv){
  // break if les than 5 args
  if(argc < 5){
    cout << "Not enough arguments" << endl;
  }
  const char * movieCast = argv[1];
  char weight = *(argv[2]);
  std::string pairToFind = argv[3];
  std::string outfile = argv[4];
  ofstream out(outfile);
  // prints the header
  out << "(actor)--[movie#@year]-->(actor)--..." << endl;
  ActorGraph actorGraph;
  if(!actorGraph.loadFromFile( movieCast, false)){
    return -1;
  }
  ifstream infile(pairToFind);
  bool have_header = false;
  if (infile.eof()) {
    cerr << "Failed to read " << pairToFind << "!\n";
    return false;
  }
  // while reading through the input file
  while(infile){
    string s;

    if(!getline(infile, s)) break;

    if(!have_header){
      have_header = true;
      continue;
    }

    istringstream ss (s); // basically parses the string, gotten by getline
    vector<string> actors;
    while(ss){
      string next;

      if(!getline(ss, next, '\t')) break; // next has first actor,then tabs
      actors.push_back(next); // will have first actor, then second actor
    }

    //set start and stop nodes
    ActorNode* start = (*((actorGraph.hash_Actor).find(actors[0]))).second;
    start->prev = NULL;
    ActorNode* stop = (*((actorGraph.hash_Actor).find(actors[1]))).second;

    //call bfstraverse or dijkstra depending on parameter
    ActorNode* foundActor;
    if(weight == 'u'){
      foundActor = BFSTraverse(start,stop);
    }
    else{
      foundActor = Dijkstra(start,stop);
    }
    //use the returned node and traverse backwards and print
    //use a stack in order to put it in correct order
    std::vector<ActorEdge*> stack;
    if (foundActor == NULL){
      out << "No Path for " << actors[0] << " and " << actors[1] << endl;
    }else{
      while(foundActor!=start){
        ActorEdge* curEdge = foundActor->prev;
        stack.push_back(curEdge);
        // find next node to go backwards
        if(foundActor == curEdge->actor1){
          foundActor = curEdge->actor2;
        }else{
          foundActor = curEdge->actor1;
        }
      }
    }

    print(out, foundActor, stack);
  }
}

/*
 * takes in the out stream, the actor found, and a stack to print it in proper
 * order
 * print method to print out the paths
 */
void print(ofstream& out, ActorNode* foundActor, std::vector<ActorEdge*> stack){
  while(!stack.empty()){
    out << "(" <<foundActor->name<< ")--[" <<(stack.back())->movie->title
      <<"#@"<<std::to_string((stack.back())->movie->year)<<"]-->";
    if(foundActor == (stack.back())->actor1){
      foundActor = (stack.back())->actor2;
    }else{
      foundActor = (stack.back())->actor1;
    }
    stack.pop_back();
  }
  out << "(" << foundActor->name << ")" << endl;
}

/*
 * BFSTraverse finds the shortest paths between the two actors given
 */
ActorNode* BFSTraverse(ActorNode* start, ActorNode* stop){
  queue<ActorNode*> toExplore;
  ActorNode* curActor = start;
  std::unordered_map<std::string, ActorNode*> actorMap;
  std::pair<std::string, ActorNode*> actorPair= std::make_pair(curActor->name, curActor);
  actorMap.insert(actorPair);
  //loop keeps running until stop is found
  while (curActor != stop){
    for (int i = 0; i < curActor->movies.size(); i++){
      ActorEdge* curEdge = curActor->movies[i]; // edge of each movie
      ActorNode* nextActor;  // next actor node on the edge
      if(curEdge->actor1 == curActor){
        nextActor = curEdge->actor2;
      }else{
        nextActor = curEdge->actor1;
      }
      //condition to stop
      if(nextActor == stop){
        nextActor->prev = curEdge;
        return nextActor;
      }
      //checks if the node has already been added to the queue or not
      if(actorMap.find(nextActor->name) == actorMap.end()){
        // decides what the next node to traverse to is
        toExplore.push(nextActor);
        nextActor->prev = curEdge;
        actorPair= std::make_pair(nextActor->name, nextActor);
        actorMap.insert(actorPair);
      }
    }
    curActor = toExplore.front();
    toExplore.pop();
  }
  return curActor;
}
/**
 * method to run dijkstras
 * takes in start and stop nodes
 * returns the final stop node, but the path back to start should be adjusted
 */
ActorNode* Dijkstra(ActorNode* start, ActorNode* stop){
  ActorNode* curActor = start;
  curActor->index = 0;
  std::priority_queue<ActorNode*, std::vector<ActorNode*>, ActorNodePtrComp> toExplore; 
  std::vector<ActorNode*> toRevert;

  toExplore.push(curActor);
  // keep exploring until the pq is empty
  while(!toExplore.empty()){
    curActor = toExplore.top();
    toExplore.pop();
    // end the method but also need to reset values so subsequent calls are not
    // affected by previous calls
    if(curActor == stop){
      for(ActorNode* actor: toRevert){
        actor->index = INT_MAX;
        actor->done = false;
      }
      start->index = INT_MAX;
      start->done = false;
      break;
    }
    ActorNode* nextActor;
    int dist = curActor->index;
    // if the node has not been accessed before then visit it
    if(!curActor->done){
      curActor->done = true;
      for(auto edges : curActor->movies){
        if(edges->actor1 == curActor){
          nextActor = edges->actor2;
        }else{
          nextActor = edges->actor1;
        }
        // check if this new weight is lower to reset the index of the node
        if((dist+edges->weight) < nextActor->index ){
          nextActor->prev = edges;
          nextActor->index = (dist+edges->weight);
          toRevert.push_back(nextActor);
          toExplore.push(nextActor);
        }
      }
    }
  }
  return curActor;
}
