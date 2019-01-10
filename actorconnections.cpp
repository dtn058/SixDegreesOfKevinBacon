/*
 * actorconnections.cpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-13-17
 *
 * actorconnections.cpp which will be used for bfs / unionfind on two actors to
 * see the earliest year that they connect
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>
#include <climits>
#include "ActorGraph.h"
#include "Movie.h"
#include "ActorNode.h"
#include "UnionFind.hpp"
#include "UFActorNode.hpp"

using namespace std;
ActorNode* BFSTraverse(ActorNode* start, ActorNode* stop);
void BFSGraph(ActorGraph& actorGraph, vector<string>& actors, ofstream& out);
void UFGraph(UnionFind& unionGraph, vector<string>& actors, ofstream& out);

int main(int argc, char** argv){
  if(argc < 5){
    cout << "Not enough arguments" << endl;
  }

  const char * movieCast = argv[1];
  std::string pairToFind = argv[2];
  std::string outfile = argv[3];
  std::string flag = argv[4]; // will be bfs or ufind
  ofstream out(outfile);

  // prints the header
  out << "Actor1\tActor2\tYear" << endl;

  ifstream infile(pairToFind);
  bool have_header = false;

  if (infile.eof()) {
    cerr << "Failed to read " << pairToFind << "!\n";
    return false;
  }

  vector<string> ufActors;
  UnionFind unionGraph;
  while(infile){
    ActorGraph actorGraph;
    if (flag == "bfs"){
      if(!actorGraph.loadFromFileNoEdges( movieCast)){
        return -1;
      }
    }    
    
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
      ufActors.push_back(next); // push all of the actors into ufActors;
    }
    if(flag == "bfs"){
      BFSGraph(actorGraph, actors, out);
    }  
  }
  
  // after the while loop, check to see if ufind was supposed to be run
  if(flag == "ufind"){
    //load in all the movies and the actors
    if(!unionGraph.loadFromFile(movieCast)){
      return -1;
    }
    UFGraph(unionGraph, ufActors, out);
  } 
  return 0;
}
/**
 * Create the Graph for Union Find
 * takes in the graph object, string of actor pairs, and the outstream
 * returns void
 */
void UFGraph(UnionFind& unionGraph, vector<string>& actors, ofstream& out){
  int foundPath[actors.size()/2];// to store the first years a path found
  for(int i = 0; i < actors.size()/2; i++){
    foundPath[i] = 0;
  }
  int year = 0;
  while(year <= 2015){
    unionGraph.createSets(year);
    //iterate through all the actors and make comparisons
    for(int i = 0; i < actors.size()-1; i+=2){
      UFActorNode* actor1 = (*((unionGraph.hash_Actor).find(actors[i]))).second;
      UFActorNode* actor2 = (*((unionGraph.hash_Actor).find(actors[i+1]))).second;
      if(unionGraph.pathCompress(actor1, actor2) && foundPath[i/2] == 0){
        foundPath[i/2] = year;
      }
    }
    year++;
  }
  // prints out the actors found or not found
  for(int i = 0; i < actors.size()-1; i+=2){
    if(foundPath[i/2] == 0){
      out << actors[i] << "\t" << actors[i+1] << "\t" << "9999" << endl;
    }else{
      out << actors[i] << "\t" << actors[i+1] << "\t" << foundPath[i/2] << endl;
    }
  }
}
/**
 * method to run the BFSgraph
 *
 */
void BFSGraph(ActorGraph& actorGraph, vector<string>& actors, ofstream& out){
    // see if these actors exist
    ActorNode* actor1 = (*((actorGraph.hash_Actor).find(actors[0]))).second;
    actor1->prev = NULL;
    ActorNode* actor2 = (*((actorGraph.hash_Actor).find(actors[1]))).second;
    ActorNode* foundActor = NULL;
    int year = 0;
    bool isFound = true;
    while(!foundActor){
      // form edges for that year
      //cout << actor1->name << endl;
      bool cont = actorGraph.createEdgesYear(year);
      if(!cont){
        // if done searching and no path is found
        out << actors[0] << "\t" << actors[1] << "\t" << "9999" << endl;
        isFound = false;
        break;
      }else{
        // if not done searching and need to search still
        foundActor = BFSTraverse(actor1, actor2);
        year++;
      }
    }
    // if path is found, print it out
    if(isFound){
      if(year == 1){
        year = foundActor->oldestMovie()+1;
      }
    out << actors[0] << "\t" << actors[1] << "\t" << year-1 << endl;
    }
}
/*
 * BFSTraverse finds the shortest paths between the two actors given
 */
ActorNode* BFSTraverse(ActorNode* start, ActorNode* stop){
  //cout << start->name << endl;
  queue<ActorNode*> toExplore;
  ActorNode* curActor = start;
  std::unordered_map<std::string, ActorNode*> actorMap;
  std::pair<std::string, ActorNode*> actorPair= std::make_pair(curActor->name, curActor);
  actorMap.insert(actorPair);
  if(start->movies.empty()|| stop->movies.empty()){
    return NULL;
  }
  toExplore.push(start);
  while (!toExplore.empty()){
    //cout << toExplore.size() << endl;
    curActor = toExplore.front();
    toExplore.pop();
    // check if actor is found
    if(curActor == stop){
      return curActor;
    }
    for (int i = 0; i < curActor->movies.size(); i++){
      ActorEdge* curEdge = curActor->movies[i]; // edge of each movie
      ActorNode* nextActor;  // next actor node on the edge
      //determine the next actor since undirected edge is ambiguous
      if(curEdge->actor1 == curActor){
        nextActor = curEdge->actor2;
      }else{
        nextActor = curEdge->actor1;
      }
      //checks if the node has already been added to the queue or not
      if(actorMap.find(nextActor->name) == actorMap.end()){
        // decides what the next node to traverse to is
        //cout << "add" << endl;
        toExplore.push(nextActor);
        nextActor->prev = curEdge;
        actorPair= std::make_pair(nextActor->name, nextActor);
        actorMap.insert(actorPair);
      }
    }
  }
  return NULL;
}

