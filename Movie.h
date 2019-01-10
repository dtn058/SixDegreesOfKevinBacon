#ifndef MOVIE_H
#define MOVIE_H
//#include "ActorGraph.h"
//#include "ActorNode.h"
#include <vector>
#include <string>
//#include "UnionFind.hpp"
//#include "ActorEdge.h"
class ActorNode;
class UnionFind;
class UFActorNode;

using namespace std;
class Movie {
  public:
    std::string title; // movie title
    int year; // year of movie
    std::vector<ActorNode*> cast; // cast with ActorNodes
    std::vector<UFActorNode*> ufcast;//cast with UFActorNodes

    Movie(std::string title, int year) : title(title), year(year){}
    bool operator==(Movie movie); // comparator for the Movie
    bool operator<(Movie movie);  // override the comparator so for pq

    void add(ActorNode* newActor); //adds actor to the cast
    void addUF(UFActorNode* newActor); // adds ufactor to ufcast

    ActorNode* find(std::string actorToFind); // searches for the actor

    std::string toString(); // translates movie+year into a string
    ~Movie();
};

#endif
