#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <cmath>
#include <ctime>
#include <utility>
#include <queue>

#ifndef BiPartite2_H
#define BiPartite2_H

using namespace std;

const short MAX_N = 1000;

struct BEdge
{
	short h;
	BEdge* next;
	BEdge(short x)
	{
		h = x;
		next = 0;
	}
	BEdge(short x, BEdge* pointTo)
	{
		h = x;
		next = pointTo;
	}
};

struct U_vertex
{
	BEdge* firstEdge;
	BEdge* nextEdge;
	short mate;
	bool critical;
	U_vertex()
	{
		firstEdge = 0;
		nextEdge = 0;
		mate = 0;
		critical = false;
	}
};

struct V_vertex
{
	short mate;
	V_vertex()
	{
		mate = 0;
	}
};

struct myQueue
{
	short u; //vertex
	short i_parent; //index in queue of parent vertex in tree of u
	short v; //current mate of u
};

class BiPartite2
{
private:
	vector <U_vertex> U_partition;
	vector <V_vertex> V_partition;
	vector <bool> visitedU;
	vector <bool> visitedV;
	myQueue queue[MAX_N];
	short iUnmatched;

public:
	BiPartite2(short numU, short numV);
	~BiPartite2();
	void addEdge(short u, short v);
	void initialize();
	void updatePath(short i1, short i2);
	void findPaths();
	void greedyMatch();
	void match(vector<short> &);
	short measureMatching();
	void findCritical();

	void test();
};

#endif //BiPartite2_H