/*
 * ActorEdge.cpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-13-17
 */


#include <iostream>
#include "ActorGraph.h"
#include "Movie.h"
#include "ActorNode.h"

using namespace std;


/*
 * Edge class for our edges connecting the actors
 */

/*
 * helper method to get the weight
 */
int ActorEdge::getWeight(){
  return weight;
}

/*
 * helper method to set weights
 */
void ActorEdge::setWeight(){
  this->weight = 1+(2015-(this->movie)->year);
}

/*
 * helper to get a movie from ActorEdge
 */
Movie* ActorEdge::getMovie(){
  return movie;
}

/*
 * helper to set a movie
 */
void ActorEdge::setNextMovie(Movie* arg){
  this->movie = arg;
}

/*
 * overloaded less than operator to make sure it's comparing weights
 *
 */
bool ActorEdge::operator<(const ActorEdge& other){
  return this->weight > other.weight;
}

ActorEdge::~ActorEdge(){
}
