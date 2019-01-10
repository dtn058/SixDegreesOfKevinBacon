/*
 * ActorNode.cpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-03-17
 *
 * ActorNode class that is responsible for creating a node for every actor
 * and holding data required
 */

#include <iostream>
#include "ActorGraph.h"
#include "Movie.h"
#include "ActorEdge.h"
#include <climits>

using namespace std;

/*
 * findMovie
 * returns bool (to see if movie was found or not)
 * takes in Movie* (check to see if movie exists)
 */
bool ActorNode::findMovie(Movie* movie){
  
  // iterate through every edge
  for(ActorEdge* edge: movies){
    Movie* movie_list = edge->getMovie();
    if(*movie == *movie_list){
      return true;
    }
  }
  return false;
}

/*
 * addEdge
 * takes in ActorEdge* to make a connection between the current actor
 * and their costars
 */
void ActorNode::addEdge(ActorEdge* edge){
  this->movies.push_back(edge);
}

/*
 *  oldestMovie
 *  returns int, which is the year of the oldest movie the actor has
 */
int ActorNode::oldestMovie(){
  Movie* oldestMovie;
  int minYear = INT_MAX;
  for (ActorEdge* edge: this->movies){
    Movie* movie = edge->movie;
    if (movie->year < minYear){
      minYear = movie->year;
    }
  }
  return minYear;
}

/*
 * a destructor for ActorNode
 * deletes its edges then the node itself
 */
ActorNode::~ActorNode(){

  for(auto i : movies){
    if(i){
      delete i;
    }
  }
  this->movies.clear();  
  delete this;
}
