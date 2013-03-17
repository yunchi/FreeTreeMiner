// BiPartite2.cpp : Defines the entry point for the console application.
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <queue>
#include <iterator>
#include <cmath>
#include <ctime>
#include <utility>
#include "BiPartite2.h"

using namespace std;

BiPartite2::BiPartite2(short numU, short numV) :
U_partition(numU+1), V_partition(numV+1), 
visitedU(numU+1,false), visitedV(numV+1, false)
{
}

BiPartite2::~BiPartite2()
{
	BEdge* e1;
	BEdge* e2;
	for ( short i = 0; i < U_partition.size(); i++ )
	{	
		e1 = U_partition[i].firstEdge;
		while ( e1 != 0 )
		{
			e2 = e1;
			e1 = e1->next;
			delete e2;
		}
	}
}

void BiPartite2::addEdge(short u, short v)
{
	BEdge* e1;
	e1 = new BEdge(v, U_partition[u].firstEdge);
	U_partition[u].firstEdge = e1;
}

void BiPartite2::initialize()
{
	short u;
	iUnmatched = MAX_N;
	for ( u = 1; u < U_partition.size(); u++ )
	{
		U_partition[u].nextEdge = U_partition[u].firstEdge;
		if ( U_partition[u].mate == 0 )
			queue[--iUnmatched].u = u;
	}
}

void BiPartite2::updatePath(short i1, short i2)
{
  short u, v;

  while (i2 >= 0)
    {
      u = queue[i2].u;
      v = queue[i1].v;
      V_partition[v].mate = u;
      U_partition[u].mate = v;
      i1 = i2;
      i2 = queue[i2].i_parent;
    }
}

void BiPartite2::findPaths()
{
  /* finds augmenting paths by breadth-first search. */

  short tail;
  short head;
  short u, i;
  BEdge *e, *e2;

  /* external loop gets unmatched u vertices from far end of array queue */
  while (iUnmatched < MAX_N)
    {
      head = tail = 0;
      queue[head].u = queue[iUnmatched++].u;
      queue[head].i_parent = -1; /* root of BFS tree */

	  /* queue processing */
      while (tail >= head)
	{
	  u = queue[head++].u;
	  e = U_partition[u].firstEdge;
	  /* adjacency list scanning */
	  while (e != 0)
	    {
	      if ( !visitedU[V_partition[e->h].mate] )
		{
		  /* extend tree */
		  queue[++tail].u = u = V_partition[e->h].mate;
		  queue[tail].i_parent = head-1;
		  queue[tail].v = e->h;
		  visitedU[u] = true;
		  /* look-ahead heuristic */
		  e2 = U_partition[u].nextEdge;
		  while (e2 != 0 && (V_partition[e2->h].mate != 0) )
		    {
		      e2 = e2->next;
		    }
		  U_partition[u].nextEdge = e2;
		  if (e2 != 0)
		    {
		      /* u in the queue but not the edge to v */
		      V_partition[e2->h].mate = u;
		      U_partition[u].mate = e2->h;
		      /* now for the rest of the path */
		      updatePath(tail, head-1);
		      /* temporary cut is emptied */
		      for (i = 0; i <= tail; i++) 
			{
			  visitedU[queue[i].u] = false;
			}
		      tail = head-1; /* to get off from queue loop */
		      break; /* to get off from adj list scan loop */
		    }
		}
	      e = e->next;
	    }
	}
    }
}

void BiPartite2::greedyMatch()
{
	short u;
	BEdge* e;

	for ( u = 1; u < U_partition.size(); u++ )
	{
		e = U_partition[u].firstEdge;
		while ( e != 0 )
		{
			if ( V_partition[e->h].mate == 0 )
			{
				V_partition[e->h].mate = u;
				U_partition[u].mate = e->h;
				break;
			}
			e = e->next;
		}
	}
}

void BiPartite2::match(vector<short> & result)
{
	greedyMatch();
	initialize();
	findPaths();
	findCritical();
	result[0] = measureMatching();
	for ( short i = 1; i < U_partition.size(); i++ )
		result[i] = (U_partition[i].critical)? result[0] - 1 : result[0];
}

short BiPartite2::measureMatching()
{
	short size = 0;
	for ( short u = 1; u < U_partition.size(); u++ )
		if ( U_partition[u].mate != 0 ) size++;
	return size;
}

void BiPartite2::findCritical()
{
	short tail = 0;
	short head = 0;
	BEdge* e1;
	BEdge* e2;
	short u;
	for ( u = 1; u < U_partition.size(); u++ )
	{
		visitedU[u] = false;
		if ( U_partition[u].mate == 0 )
		{
			queue[tail++].u = u; //put unmated vertices into queue 
			U_partition[u].critical = false; //unmated vertices are not critical
		}
		else U_partition[u].critical = true; //potential critical vertex in U (X)
	}

	visitedU[0] = true; //unmatched vertices in V are pointing to this one
	while ( tail > head )
	{
		u = queue[head++].u;
		e1 = U_partition[u].firstEdge;
		while ( e1 != 0 )
		{
			if ( (e1->h != U_partition[u].mate) && !visitedV[e1->h] )
			{
				visitedV[e1->h] = true;
				if ( !visitedU[V_partition[e1->h].mate] )
				{
					visitedU[V_partition[e1->h].mate] = true;
					U_partition[V_partition[e1->h].mate].critical = false;
					queue[tail++].u = V_partition[e1->h].mate;
				}
			}
			e1 = e1->next;
		}
	}
}

void BiPartite2::test()
{
	for ( short u = 1; u < U_partition.size(); u++ )
	{
		cout << "u: " << u << " mate: " << U_partition[u].mate << endl;
	}
}

//int main(int argc, char* argv[])
//{
//  short matchingSize;
//  BiPartite2* bPar2;
//  vector<short> result;
//
//  char c;
//  short u, v, size;
//  short uu, vv;
//
//  ifstream dataFile("bbtest1_3.txt");
//  if (!dataFile)
//  {
//	  cerr << "cannot open input file!" << endl;
//	  exit(1);
//  }
//
//  dataFile >> uu;
//  dataFile >> vv;
//  dataFile >> size;
//
//  result.resize(uu+1);
//
//  bPar2 = new BiPartite2(uu,vv);
//
//  for ( int i = 0; i < size; i++ )
//  {
//	dataFile >> c;
//	dataFile >> u;
//	dataFile >> v;
//	bPar2->addEdge(u,v);
//  }
//
//  dataFile.close();
//  
//  bPar2->match(result);
//  bPar2->findCritical();
//
//  matchingSize = bPar2->measureMatching();
//
//  printf("n=%d, m=%d, size=%d (%.1f%%)\n",
//	 uu, vv, matchingSize, 
//	 (float) 100*matchingSize/vv);
//	return 0;
//}
