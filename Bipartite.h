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

#ifndef Bipartite_H
#define Bipartite_H

//#define MAX_N 65600
#define MAX_N 1000
#define MAX_VERTICES	MAX_N

typedef struct enode {
  struct enode *next;
  short h;
} BEdge;

typedef struct {
  BEdge *firstEdge;
  BEdge *nextEdge;
  short  mate;		/* mate == 0 means no mate */
  bool critical;   //if this vertex is critical
} U_vertex;

typedef struct {
  U_vertex A[MAX_N];
  short size;
  short edges;
} U_partition;

typedef struct {
  short  mate;		 /* mate == 0 means no mate */
} V_vertex;

typedef struct {
  V_vertex A[MAX_N];
  short size;
} V_partition;


typedef struct {
  short u;       /* vertex */
  short i_parent; /* index in queue of parent vertex in tree of u */
  short v;      /* current mate of u */
} myQueue;

class Bipartite {
public:
	U_partition* U;
	V_partition* V;

public:
	Bipartite();
	~Bipartite();
	void clearMe();
	void deepClearMe();
	void AddEdge(short u, short v);
	void Inits();
	void UpdatePath(short i1, short i2);
	void FindPaths();
	void GreedyMatch();
	short MeasureMatching();
	void Match( vector<short> & );
	void findCritical();
	void PrintStatus();

private:
	int iUnmatched;
	bool visited[MAX_N];
	myQueue queue[MAX_N];

};

#endif //Bipartite_H
