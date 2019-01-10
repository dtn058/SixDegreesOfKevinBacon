/*
 * ActorGraph.h
 * Author: Dat Nguyen, Alexander Nguyen
 * Date:   03-03-17
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <set>
#include <unordered_map>
#include "Movie.h"
#include "ActorNode.h"
#include "ActorEdge.h"
#include <queue>

// Maybe include some data structures here

using namespace std;

class ActorGraph {
protected:
  
    // Maybe add class data structure(s) here
    // class to make the comparator for the priority queue
    class MoviePtrComp{
      public:
        bool operator()(Movie* &lhs, Movie* &rhs) const;
    };

public:
    ActorGraph(void);
    // all actors from the file
    std::unordered_map<std::string, ActorNode*> hash_Actor;
    // all the movies from the file
    std::unordered_map<std::string, Movie*> hash_Movie;
    //used to store movies in order of year 
    std::priority_queue<Movie*, std::vector<Movie*>, MoviePtrComp> pq_Movie;
    

    // Maybe add some more methods here
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 
     * 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    // same as load file from before except does not create any edges
    bool loadFromFileNoEdges(const char* in_filename);
    // creates edges year by year
    bool createEdgesYear(int year);
    ~ActorGraph();

private:
  // helper for loadfromfile to create all the edges between nodes at once
  void createEdges();
  
};


#endif // ACTORGRAPH_H
