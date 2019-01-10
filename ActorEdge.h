/*
 * ActorEdge.h
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-13-17
 */

#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include <iostream>
#include "ActorGraph.h"
#include "Movie.h"
#include "ActorNode.h"

using namespace std;
/*
 *  Header for edge class for our edges connecting the actors
 */

class Movie;
class ActorNode;
class ActorEdge{

  public:
    ActorNode* actor1; // pointer to next node?
    ActorNode* actor2; // pointer to current node
    Movie* movie; // movie that these two actors played in together
    int weight; //weight of the edge
    ActorEdge( ActorNode* curr, ActorNode* to, Movie* movie, int weight) : 
      actor1(curr),actor2(to), movie(movie), weight(weight){}
    //override the operator
    bool operator<(const ActorEdge& other); 
    //getter method
    int getWeight();
    //setter method
    void setWeight();
    //getter for movie
    Movie* getMovie();
    //setter for movie
    void setNextMovie(Movie* arg);

    ~ActorEdge();

}; 

#endif
