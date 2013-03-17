/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Header: FrequentTreeList.h
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
#include <utility>
#include "misc.h"
#include "FreeTree.h"
#include "CanonicalTree.h"
#include "TupleTree.h"

using namespace std;

#ifndef FrequentTreeList_H
#define FrequentTreeList_H

struct supportNode 
{
	long support;
	long lastTid;
	list<long> tidList;
};

class FrequentTreeList 
{
public:
	long returnSize() const; //return number of elements in the map
	void populate2(const vector<ptrFreeTree>& database);
	void populate3(const vector<ptrFreeTree>& database, FrequentTreeList* rhs);
	void populate4(const vector<ptrFreeTree>& database, FrequentTreeList* rhs);
	void checkDownward(FrequentTreeList& rhs);
	void countSupport(const vector<ptrFreeTree>& database); //compute support from a database with name s
	void finalize(long threshold); //remove elements whose support is less than a threshold
	bool downwardSupport(const CanonicalTree& rhs);
	long countMaximum(FrequentTreeList& rhs);

	void test();

	friend istream& operator>>(istream&, FrequentTreeList&);
	friend ostream& operator<<(ostream&, const FrequentTreeList&);

	friend class CoreList;

private:
	map<CanonicalTree,supportNode> frequentList;
};

#endif //FrequentTreeList_H
