/*
 * FriendGraph.hpp
 * Author: Dat Nguyen, Alexander Nguyen
 * Date: 03/13/17
 *
 * header file for FriendGraph, which implements the Disjoint Set ADT to properly find a connection 
 * between two friends.
 */

#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP
#include <iostream>
#include <unordered_map>
#include "Movie.h"
#include <queue>
#include <sstream>
#include <fstream>
#include "FriendNode.hpp"

using namespace std;

//class FriendNode;
class FriendGraph {
  public:
    FriendGraph(void);
    std::unordered_map<std::string, FriendNode*> hash_friend;
    
    bool loadFromFile(const char* in_filename);
    // union method to add
    bool createEdges(int year);
    ~FriendGraph();
};
FriendGraph::FriendGraph (){}

/**
 * Load the movies from the file and put them int accessible space
 *
 */
bool FriendGraph::loadFromFile(const char* in_filename){
  // Initialize the file stream
  ifstream infile(in_filename);
  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile) {
    string s;
    // get the next line
    if (!getline( infile, s )) break;

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;
      
      // get the next string before hitting a tab character and 
      // put it in 'next'
      if (!getline( ss, next, ' ' )) break;

        record.push_back( next );
    }
    
    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }

    std::string person_name(record[0]);
    std::string friend_name(record[1]);

    FriendNode* newPerson;
    FriendNode* newFriend;
    //establish person and friend nodes, insert into hash map/graph
    if(this->hash_friend.find(person_name) == this->hash_friend.end()){
      newPerson = new FriendNode(person_name);
      std::pair<std::string, FriendNode*> pair(newPerson->name, newPerson); 
      this->hash_friend.insert(pair);
    }else{
      newPerson = (*(this->hash_friend.find(person_name))).second;
    }
    if(this->hash_friend.find(friend_name) == this->hash_friend.end()){
      newFriend = new FriendNode(friend_name);
      std::pair<std::string, FriendNode*> pair(newFriend->name, newFriend);
      this->hash_friend.insert(pair); 
    }else{
      newFriend = (*(this->hash_friend.find(friend_name))).second;
    }
    // create edges
    newPerson->friendsList.insert(std::pair<std::string, FriendNode*>
      (newFriend->name, newFriend));
  }
  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();
  return true;
}
FriendGraph::~FriendGraph(){
  for(auto node : this->hash_friend){
    delete node.second;
  }
}
#endif
