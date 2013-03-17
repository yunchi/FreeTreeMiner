/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Header: CanonicalTree.h
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

using namespace std;

#ifndef CanonicalTree_H
#define CanonicalTree_H

class CanonicalTree : public FreeTree
{
public:
	CanonicalTree(short v, long t);
	CanonicalTree(long t);
	CanonicalTree(const vector<short>& cString);
	CanonicalTree();
	CanonicalTree(const CanonicalTree& rhs);
	~CanonicalTree();

	CanonicalTree& operator=(const CanonicalTree& rhs);


  short normalize(short idx = 0);
  bool operator<(const CanonicalTree& rhs) const;
  void setCanonicalString(vector<short>& rhs);
  void addVertex(short vLabel);
  bool deleteLeaf(short i, short& vLabel, short& eLabel, short& conTo);


  friend istream& operator>>(istream&, CanonicalTree&);
  friend ostream& operator<<(ostream&, const CanonicalTree&);

  friend class FrequentTreeList;
  friend class CoreList;

  void canonicalTest() const;

  vector <short> canonicalString; //the canonical string of this tree

protected:
  short root1; //the first root
  short root2; //the second root (if there exists a second root)
  vector <short> level; //the levels of vertices
  vector <short> parent; //the parents of vertices
  vector <short> edgeLabelToParent; //the edge label of edge from parent
  vector <short> automorphism; //the list of automorphisms

private:
  void findRoot();
  short compareTwoVertices(short v1, short v2, vector<short>&);
  short compareTwoVertices(TnodeLink l1, TnodeLink l2, vector<short>&);
};

#endif //CanonicalTree_H
