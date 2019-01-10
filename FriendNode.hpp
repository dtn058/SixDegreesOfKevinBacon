#ifndef FRIENDNODE_HPP
#define FRIENDNODE_HPP
#include <iostream>
#include <unordered_map>

using namespace std;

class FriendNode{
  public:
    // name of the friend
    std::string name;
    // map to store the friends list
    std::unordered_map<std::string, FriendNode*> friendsList;

    FriendNode(std::string name) : name(name){}
};
#endif
