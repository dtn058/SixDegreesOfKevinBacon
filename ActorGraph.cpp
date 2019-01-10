/*
 * ActorGraph.cpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03/13/17
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>
#include <unordered_set>
#include "ActorGraph.h"


using namespace std;
  // class to create the comparator for the priority queue
  bool ActorGraph::MoviePtrComp::operator()(Movie* &lhs,Movie* &rhs) const{
    return lhs->year > rhs->year;
  }

ActorGraph::ActorGraph(void) {}

/**
 * create graphs with edges
 * takes in a parameter for weighted edges
 * takes in filename
 * returns a bool for success or not
 */
bool ActorGraph::loadFromFile(const char* in_filename, 
  bool use_weighted_edges) {
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
        ActorNode* actor;
        if((this->hash_Actor.find(actor_name)) == this->hash_Actor.end()){
          actor = new ActorNode(actor_name,INT_MAX,NULL,false);
        }else{
          actor = (*(this->hash_Actor.find(actor_name))).second;
        }
	std::unordered_map<std::string, ActorNode*>::iterator oldActor =
        this->hash_Actor.find(actor_name);
        std::unordered_map<std::string, Movie*>::iterator oldMovie = 
        this->hash_Movie.find(movie->toString());
        //add Actor to the bst
      	if(oldActor == hash_Actor.end()){
          std::pair<std::string, ActorNode*>actor_pair(actor->name, actor);
	        hash_Actor.insert(actor_pair);
	      }else{ 
          actor = (*oldActor).second;
        }
        // check if Movie has been read in before
        if (oldMovie == hash_Movie.end()){
          std::pair<std::string, Movie*> movie_pair(movie->toString(), movie);
          hash_Movie.insert(movie_pair);  
          movie->add(actor);
        }else{ 
          ((*oldMovie).second)->add(actor);  //add actors to the movie's cast

          // if added, then delete
          delete movie;
        }  
    }
    createEdges(); 
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    return true;
}

/*
 * Method to create graph without any edges
 * parameter is only filename
 * return true if runs properly
 */
bool ActorGraph::loadFromFileNoEdges(const char* in_filename) {
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
        ActorNode* actor;
        if((this->hash_Actor.find(actor_name)) == this->hash_Actor.end()){
          actor = new ActorNode(actor_name,INT_MAX,NULL,false);
        }else{
          actor = (*(this->hash_Actor.find(actor_name))).second;
        }
	std::unordered_map<std::string, ActorNode*>::iterator oldActor =
          this->hash_Actor.find(actor_name);
        std::unordered_map<std::string, Movie*>::iterator oldMovie = 
          this->hash_Movie.find(movie->toString());
        //add Actor to the hash map
        	if(oldActor == hash_Actor.end()){
          std::pair<std::string, ActorNode*>actor_pair(actor->name, actor);
          hash_Actor.insert(actor_pair);
        	}else{
           //delete actor;
           actor = (*oldActor).second;
          }

        // check if Movie has been read in before
        if (oldMovie == hash_Movie.end()){
          std::pair<std::string, Movie*> movie_pair(movie->toString(), movie);
          hash_Movie.insert(movie_pair);  
          movie->add(actor);
          this->pq_Movie.push(movie);
        }else{ 
          ((*oldMovie).second)->add(actor);  //add actors to the movie's cast
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
 * Method to create the edges only up to a certain year
 * takes year as a parameter and makes all edges possible to that year
 * returns true if creates edges properly
 */
bool ActorGraph::createEdgesYear(int year){
  Movie* movie = this->pq_Movie.top();
  if(this->pq_Movie.empty()){
    return false;
  }
  //go through priority queue and access movies and their actors
  while(movie && movie->year <= year && !(this->pq_Movie.empty())){
    movie = this->pq_Movie.top();
    this->pq_Movie.pop();
    //iterate through the cast in the movie
    for(int i =0; i < (movie->cast).size(); i++){ 
      ActorNode* cur_Actor = (movie->cast)[i];
      for (int k = i+1; k < (movie->cast).size(); k++){
        ActorNode* next_Actor = (movie->cast)[k];
        // create edge to account for it being an undirected edge
        ActorEdge* edge1 = new ActorEdge(cur_Actor, 
          next_Actor, movie, 1);
        next_Actor->addEdge(edge1);
        cur_Actor->addEdge(edge1);
      }
    }
    movie = this->pq_Movie.top();
  }
  return true;
}

/*
 *Helper method to help create the edges between two actors.
 *
 */
void ActorGraph::createEdges(){
 // loop through every movie
  for (auto movie: hash_Movie){ 
    // at every movie, loop through its cast
    for(int i =0; i < (movie.second->cast).size(); i++){ 
      ActorNode* cur_Actor = (movie.second->cast)[i];
      for (int k = i+1; k < (movie.second->cast).size(); k++){
        ActorNode* next_Actor = (movie.second->cast)[k];
        // create edge to account for it being an undirected edge
        ActorEdge* edge1 = new ActorEdge(cur_Actor, 
          next_Actor, movie.second, 1);
        edge1->setWeight(); 
        next_Actor->addEdge(edge1);
        cur_Actor->addEdge(edge1);
      }
    }
  }
}

// destructor for the graph
ActorGraph::~ActorGraph(){
    for(auto movie : hash_Movie){
      delete movie.second;
    }
    hash_Movie.clear();
}

