/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Header: FreeTree.h
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <fstream>
#include <string>
#include <cmath>
#include "misc.h"

using namespace std;

#ifndef FreeTree_H
#define FreeTree_H

struct Tnode
{
  short v;		//the other vertex this link connects to
  short eLabel;	//edge label
  Tnode* next;	//head of the link list for adjacent vertices
  Tnode(short x, short e, Tnode* t) 
  { v = x; eLabel = e; next = t; }

  // shallow copy constructor
  Tnode(const Tnode& rhs) { v = rhs.v; eLabel = rhs.eLabel; next = 0; } 
  Tnode& operator=(const Tnode& rhs) 
  { v = rhs.v; eLabel = rhs.eLabel; next = 0; return *this; }
};

typedef Tnode* TnodeLink;

class FreeTree
{
 public:
  FreeTree(short v, long t);
  FreeTree(long t);
  FreeTree();
  FreeTree(const FreeTree& rhs); //copy constructor
  ~FreeTree();

  FreeTree& operator=(const FreeTree& rhs);

  short vertexNumber() const;
  long returnTid() const;
  void changeVertexLabel(short idx, short vLabel);
  void insertEdge(Edge e);
  void deleteEdge(short i, short j);
  bool deleteLeaf(short i, short& vLabel, short& eLabel, short& conTo);

  friend istream& operator>>(istream&, FreeTree&);
  friend ostream& operator<<(ostream&, const FreeTree&);

  friend class TupleTree;
  friend class FrequentTreeList;
  friend class CoreList;

public:
  long tid;  //transaction ID

protected:
  short vCount; //number of vertices
  vector <TnodeLink> adj; //adjacency list (heads)
  vector <short> vertexLabel; //labels of vertices
  vector <short> degree; //degrees of vertices
};

#endif //FreeTree_H
