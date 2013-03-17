/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Header: CoreList.h
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
#include <map>
#include <set>
#include "misc.h"
#include "FreeTree.h"
#include "CanonicalTree.h"
#include "FrequentTreeList.h"

using namespace std;

#ifndef CoreList_H
#define CoreList_H

struct Limb //a limb of a free tree
{
	short vertexLabel; //the vertex label
	short edgeLabel; //the edge label
	short connectTo; //to which vertex it connects to

	Limb(short v = 0, short e = 0, short conTo = 0) :
		vertexLabel(v), edgeLabel(e), connectTo(conTo) { }
	Limb(const Limb& rhs)
	{
		vertexLabel = rhs.vertexLabel;
		edgeLabel = rhs.edgeLabel;
		connectTo = rhs.connectTo;
	}
	Limb& operator=(const Limb& rhs)
	{
		vertexLabel = rhs.vertexLabel;
		edgeLabel = rhs.edgeLabel;
		connectTo = rhs.connectTo;
		return *this;
	}

	bool operator<(const Limb& rhs) const;
	
	friend ostream& operator<<(ostream&, const Limb& rhs);
};

class CoreList 
{
public:
	long returnSize() const; //return number of elements in the map
	void readFrom(FrequentTreeList& ftl); //populate coreList with frequent k-trees
	void writeTo(FrequentTreeList& ftl); //write out candidate k+1-trees

	void test();

private:
	map< vector<short> ,set<Limb> > coreList;
};

#endif //CoreList_H
