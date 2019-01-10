/*
 * Movie.cpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-13-17
 *
 * Movie holds all of the information pertaining to the movie, including
 * its cast, year, and title 
 */
#include "Movie.h"
#include "ActorNode.h"
#include "UFActorNode.hpp"
#include <iostream>

using namespace std;

/*
 * Operator overload for ==, makes sure that it is comparing
 * the years and titles
 */
bool Movie::operator==(Movie movie){
  if(this->title == movie.title && this->year == movie.year){
    return true;
  }else{
    return false;
  }
}

/*
 * add method takes in an ActorNode* to add that newActor to the cast
 */
void Movie::add(ActorNode* newActor){
  for(int i = 0; i < cast.size(); i++){
    ActorNode* actor = cast[i];
    if(actor->name == newActor->name){
      // return if they already exists in the cast
      return;
    }
  }
  this->cast.push_back(newActor);
}
// adds a UFActor node from the parameter passed in into the
// data structure to hold onto the nodes and make actors accessible
void Movie::addUF(UFActorNode* newActor){
  for(int i = 0; i < ufcast.size(); i++){
    UFActorNode* actor = ufcast[i];
    if(actor->name == newActor->name){
      // return if they already exists in the cast
      return;
    }
  }
  this->ufcast.push_back(newActor);
}

/*
 * find
 * returns and ActorNode* , which is the ActorNode* if we find it
 * takes in a string, which is the name of the actor we want to 
 * find in the cast
 */
ActorNode* Movie::find(std::string actorToFind){
  for(ActorNode* actor: this->cast){
    if(actor->name == actorToFind){
      return actor;
    }
  }
  return NULL;
}

/*
 * Allows us to tack on years to the name, to avoid duplicates
 */
std::string Movie::toString(){
  return this->title + "(" + std::to_string(this->year) + ")";
}

/*
 * a destructor for movies  
 */
Movie::~Movie(){
  this->cast.clear();

}
