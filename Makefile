# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder actorconnections extension



# include what ever source code *.h files pathfinder relies on (these are merely the ones that were used in the solution)

pathfinder: ActorGraph.o ActorNode.o Movie.o ActorEdge.o

extension: FriendGraph.hpp FriendNode.hpp

actorconnections: ActorGraph.o ActorNode.o Movie.o ActorEdge.o UnionFind.hpp UFActorNode.hpp

# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers

ActorGraph.o:UnionFind.hpp Movie.h ActorNode.h ActorEdge.h ActorGraph.h

clean:
	rm -f pathfinder actorconnections extension *.o core*

