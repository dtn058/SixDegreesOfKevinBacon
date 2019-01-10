/*
 * ActorNode.h
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-03-17
 *
 * header file for our ActorNode class, which contains information for
 * each actor
 */

#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <iostream>
#include "ActorGraph.h"
#include "Movie.h"
#include "ActorEdge.h"
#include <vector>

using namespace std;

/*
 * Class used to make nodes for our Actor graph
 *
 */
class Movie;

class ActorNode{

  public:
    std::string name; // get name of the actor
    int index; // index of the current actor
    ActorEdge* prev; // previous actor
    std::vector<ActorEdge*> movies; // list of movies starred in, 
    bool done;

    ActorNode(std::string name, int index, ActorEdge* prev, bool done) 
      : name(name), index(index), prev(prev), done(false){ }

    /*
     * findMovie
     * returns bool (to see if movie was found or not)
     * takes in Movie* (check to see if movie exists)
     */
    bool findMovie(Movie* movie);

    /*
     * addEdge
     * takes in ActorEdge* to make a connection between the current actor
     * and their costars
     */
    void addEdge(ActorEdge* edge);

    /*
     * oldestMovie
     * returns int (the year of the actor's oldest movie)
     */
    int oldestMovie();

    ~ActorNode();
};

#endif
