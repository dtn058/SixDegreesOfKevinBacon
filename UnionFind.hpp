/*
 * UnionFind.hpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date: 03/13/17
 *
 * header file for UnionFind, which implements the Disjoint Set ADT to properly find a connection 
 * between two actors.
 */

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP
#include <iostream>
#include <unordered_map>
#include "Movie.h"
#include <queue>
#include <sstream>
#include <fstream>
#include "UFActorNode.hpp"

using namespace std;

//class ActorNode;
class UnionFind {
  protected: 
    //class for the pq to compare movies
    class MoviePtrComp{
      public:
        bool operator()(Movie* &lhs, Movie* &rhs) const;
    };

  public:
    UnionFind(void);
    std::unordered_map<std::string, UFActorNode*> hash_Actor;// store all actor
    std::unordered_map<std::string, Movie*> hash_Movie;//store all movies
    std::priority_queue<Movie*, std::vector<Movie*>, MoviePtrComp> pq_Movie;
   // store movies in order
    //load all the movies and actors from the file, and hte pairs
    bool loadFromFile(const char* in_filename);
    //search for the actors and then also compress the path
    bool pathCompress(UFActorNode* actor1, UFActorNode* actor2);
    // union method to add
    bool createSets(int year);

    ~UnionFind();
};

UnionFind::UnionFind(void){}
bool UnionFind::MoviePtrComp::operator()(Movie* &lhs,Movie* &rhs) const{
  return lhs->year > rhs->year;
}

bool UnionFind::loadFromFile(const char* in_filename){
  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile) {
    string s;
    // get the next line
    if (!getline( infile, s )) break;
    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character and 
      // put it in 'next'
      if (!getline( ss, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);

    // we have an actor/movie relationship, now what?
    // make the movie if it doesn't exist, if it does then set it to movie
    Movie* movie;
    if((this->hash_Movie.find(movie_title)) == this->hash_Movie.end()){
      movie = new Movie(movie_title, movie_year);
    }else{
      movie = (*(this->hash_Movie.find(movie_title+"("+
              std::to_string(movie_year)+")"))).second;
    }
    // make the actor if it doesn't exist, if it does set it to actor.
    UFActorNode* actor;
    if((this->hash_Actor.find(actor_name)) == this->hash_Actor.end()){
      actor = new UFActorNode(actor_name,NULL, NULL);
    }else{
      actor = (*(this->hash_Actor.find(actor_name))).second;
    }
    std::unordered_map<std::string, UFActorNode*>::iterator oldActor =
      this->hash_Actor.find(actor_name);
    std::unordered_map<std::string, Movie*>::iterator oldMovie = 
      this->hash_Movie.find(movie->toString());
    //add Actor to the hash map
    if(oldActor == hash_Actor.end()){
      std::pair<std::string, UFActorNode*>actor_pair(actor->name, actor);
      hash_Actor.insert(actor_pair);
    }else{ 
      actor = (*oldActor).second;
    }

    // check if Movie has been read in before
    if (oldMovie == hash_Movie.end()){
      std::pair<std::string, Movie*> movie_pair(movie->toString(), movie);
      hash_Movie.insert(movie_pair);  
      movie->addUF(actor);
      this->pq_Movie.push(movie);
    }else{ 
      ((*oldMovie).second)->addUF(actor);  //add actors to the movie's cast
      // if added, then delete
      delete movie;
    }  
  }
  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();
  return true;
}
/**
 * create the sets up to a certain year
 * will merge sets if deemed necessary
 * parameter is the final year to read up to
 * returns true if successfully creates sets
 */
bool UnionFind::createSets(int year){
  Movie* movie = this->pq_Movie.top();
  // check if there are still movies to create connections
  if (this->pq_Movie.empty()){
    return false;
  }
  //get the next movies from the pq
  while (movie && movie->year <= year && !(this->pq_Movie.empty())){
    movie = this->pq_Movie.top();
    this->pq_Movie.pop();
    // iterate through the movie's casts and create connections
    for(int outer = 0; outer < (movie->ufcast).size(); outer++){
      UFActorNode* curActor = (movie->ufcast)[outer];
      curActor->newNode = true;
      for (int inner = outer+1; inner < (movie->ufcast).size(); inner++){
        UFActorNode* nextActor = (movie->ufcast)[inner];
        //check if a sentinel node
        if(nextActor->prev==NULL && !nextActor->newNode){
          nextActor->prev = curActor;
          nextActor->newNode = true;
        }else{ 
          // if not sentinel, then find sentinel and merge the sets
          UFActorNode* nextSent = nextActor->findSentinel();
          UFActorNode* curSent = curActor->findSentinel();
          if(nextSent != curSent){  // if sentinels are different then merge
            nextSent->prev = curSent;
          }
        }
      }
    }
    movie = this->pq_Movie.top();
  }
  return true;
}
/**
 * Takes in the two actors to find as the parameters
 * finds the actors and then compresses the path up to the sentinel
 * returns true if the actors are found
 */
bool UnionFind::pathCompress(UFActorNode* actor1, UFActorNode* actor2){
  std::vector<UFActorNode*> actor1Path;
  std::vector<UFActorNode*> actor2Path;
  //find the sentinels
  while(actor1->prev != NULL){
    actor1Path.push_back(actor1);
    actor1 = actor1->prev;
  }
  while(actor2->prev != NULL){
    actor2Path.push_back(actor2);
    actor2 = actor2->prev;
  }
  // compress the paths
  for (UFActorNode* actor : actor1Path){
    actor->prev = actor1;
  }
  for (UFActorNode* actor : actor2Path){
    actor->prev = actor2;
  }
  // check if they have the same sentinels
  if(actor1 == actor2){
    return true;
  }else{
    return false;
  }
}

UnionFind::~UnionFind(){
  for(auto actor : hash_Actor){
    delete actor.second;
  }

  for(auto movie : hash_Movie){
    delete movie.second;
  }
}  
#endif
