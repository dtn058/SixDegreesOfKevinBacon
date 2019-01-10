#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include "FriendGraph.hpp"
#include "FriendNode.hpp"

using namespace std;
void findMutualFriends(FriendGraph& friendGraph, std::string& friend1, std::string& friend2 , ofstream& out);

int main(int argc, char** argv){
  if(argc < 5){
    cout << "Not enough arguments" << endl;
  }

  const char * data = argv[1];
  std::string friend1 = argv[2];
  std::string friend2 = argv[3];
  std::string outfile = argv[4];
  ofstream out(outfile);

  // prints the header
  out << "Searching for Mutual Friends" << endl;

  FriendGraph friendGraph;
  if(!friendGraph.loadFromFile( data )){
    return -1;
  }
  findMutualFriends(friendGraph, friend1, friend2, out);
  return 0;
}

/**
 * method to find mutual friends
 * Parameters: the friend graph, names of two people of interest, outstream
 * returns void
 */
void findMutualFriends(FriendGraph& friendGraph, std::string& friend1, 
    std::string& friend2, ofstream& out){
  FriendNode* personNode = (*(friendGraph.hash_friend.find(friend1))).second;
  FriendNode* friendNode = (*(friendGraph.hash_friend.find(friend2))).second;
  bool found = false;
  // go through each person in first persons friends list
  for(auto check1 : personNode->friendsList){
   // check 1 friend node away to see if there is a mutual friend 
   if(check1.second->friendsList.find(friend2) != 
        check1.second->friendsList.end()){
      // print if found
      out << check1.second->name << endl;
      found = true;
    }
  }
  // if not found then print no mutual friends
  if(!found){
    out << "No Mutual Friends" << endl;
  }
}
