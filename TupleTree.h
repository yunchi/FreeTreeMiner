/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Header: TupleTree.h
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
#include "FreeTree.h"
#include "Bipartite.h"

using namespace std;

#ifndef TupleTree_H
#define TupleTree_H

class TupleTree : public FreeTree
{
public:
	TupleTree(short v, long t);
	TupleTree(long t);
	TupleTree(const FreeTree& rhs);
	TupleTree();
	~TupleTree();

  void rootIt();
  bool support(const FreeTree&);

  friend istream& operator>>(istream&, TupleTree&);
  friend ostream& operator<<(ostream&, const TupleTree&);

 void test();

 protected:
  list <short> vertexList; //the list of vertices sorted by level
  vector<vector<vector<bool> > > SVU;
  vector <vector<short> > IUW;
  Bipartite bPar;
};

#endif //TupleTree_H
