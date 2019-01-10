/*
 * UFActorNode
 *
 * A node class for our union find implementations. Utilizes the concept
 * of sentinels and an up-tree.
 */
#ifndef UFACTORNODE_HPP
#define UFACTORNODE_HPP
#include <string>
#include <iostream>

using namespace std;
class UFActorNode{
   public:
     UFActorNode(void);
     std::string name; // name of actor
     UFActorNode* prev; // previous node going up the tree
     UFActorNode* sentinel;
     bool newNode;
     UFActorNode(std::string name, UFActorNode* prev, UFActorNode* sentinel)
       : name(name), prev(prev), sentinel(sentinel), newNode(false){} 
     // method to find hte sentinel node
     UFActorNode* findSentinel(){
       UFActorNode* curActor = this;
       //go up the set
       while(curActor->prev != NULL){
         curActor = curActor->prev;
       }
       return curActor;
     }

};

#endif
