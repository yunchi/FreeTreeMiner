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

#include "Bipartite.h"

using namespace std;

Bipartite::Bipartite()
{
	U = new U_partition;
	V = new V_partition;
	int i;
	for (i = 0; i < MAX_N; i++)
    {
		U->A[i].firstEdge = (BEdge *) 0;
		U->A[i].nextEdge = (BEdge *) 0;
		U->A[i].mate = 0;
		U->A[i].critical = false;
    }
	U->size = 0;
	U->edges = 0;

	for (i = 0; i < MAX_N; i++)
    {
		V->A[i].mate = 0;
    }
	V->size = 0;  
}

Bipartite::~Bipartite()
{
	int i;
	BEdge* e1;
	BEdge* e2;

	for (i = 0; i <= U->size; i++)
    {
		e1 = U->A[i].firstEdge;		
		while ( e1 != (BEdge *) 0)
		{
			e2 = e1;
			e1 = e1->next;
			delete e2;
		}
		U->A[i].firstEdge = (BEdge *) 0;
		U->A[i].nextEdge = (BEdge *) 0;
	}

	delete U;
	delete V;
}

void Bipartite::clearMe()
{
	int i;
	BEdge* e1;
	BEdge* e2;

	for (i = 0; i <= U->size; i++)
    {
		e1 = U->A[i].firstEdge;		
		while ( e1 != (BEdge *) 0)
		{
			e2 = e1;
			e1 = e1->next;
			delete e2;
		}
		U->A[i].firstEdge = (BEdge *) 0;
		U->A[i].nextEdge = (BEdge *) 0;
		U->A[i].mate = 0;
		U->A[i].critical = false;
	}

	U->size = 0;
	U->edges = 0;

	for (i = 0; i <= V->size + 1; i++)
    {
		V->A[i].mate = 0;
    }
	V->size = 0;  
}

void Bipartite::deepClearMe()
{
	int i;
	BEdge* e1;
	BEdge* e2;

	for (i = 0; i < MAX_N; i++)
    {
		e1 = U->A[i].firstEdge;		
		while ( e1 != (BEdge *) 0)
		{
			e2 = e1;
			e1 = e1->next;
			delete e2;
		}
		U->A[i].firstEdge = (BEdge *) 0;
		U->A[i].nextEdge = (BEdge *) 0;
		U->A[i].mate = 0;
		U->A[i].critical = false;
	}

	U->size = 0;
	U->edges = 0;

	for (i = 0; i < MAX_N; i++)
    {
		V->A[i].mate = 0;
    }
	V->size = 0;  
}


void Bipartite::AddEdge(short u, short v)
{
  BEdge *e1;

  e1 = new BEdge;

  e1->next = U->A[u].firstEdge;
  U->A[u].firstEdge = e1;
  e1->h = v;
}

void Bipartite::Inits()
{
  short u;
  iUnmatched = MAX_N;
  for (u = 1; u <= U->size; u++)
    {
      U->A[u].nextEdge = U->A[u].firstEdge;
      visited[u] = false;
      /* we put all unmatched vertices from U at the other end of the queue */
      if (!U->A[u].mate)
	{
	  queue[--iUnmatched].u = u;
	}
    }
}

void Bipartite::UpdatePath(short i1, short i2)
{
  short u, v;

  while (i2 >= 0)
    {
      u = queue[i2].u;
      v = queue[i1].v;
      V->A[v].mate = u;
      U->A[u].mate = v;
      i1 = i2;
      i2 = queue[i2].i_parent;
    }
}

void Bipartite::FindPaths()
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
	  e = U->A[u].firstEdge;
	  /* adjacency list scanning */
	  while (e != (BEdge *) 0)
	    {
	      if (!visited[V->A[e->h].mate])
		{
		  /* extend tree */
		  queue[++tail].u = u = V->A[e->h].mate;
		  queue[tail].i_parent = head-1;
		  queue[tail].v = e->h;
		  visited[u] = true;
		  /* look-ahead heuristic */
		  e2 = U->A[u].nextEdge;
		  while (e2 != (BEdge *) 0 && V->A[e2->h].mate)
		    {
		      e2 = e2->next;
		    }
		  U->A[u].nextEdge = e2;
		  if (e2 != (BEdge *) 0)
		    {
		      /* u in the queue but not the edge to v */
		      V->A[e2->h].mate = u;
		      U->A[u].mate = e2->h;
		      /* now for the rest of the path */
		      UpdatePath(tail, head-1);
		      /* temporary cut is emptied */
		      for (i = 0; i <= tail; i++) 
			{
			  visited[queue[i].u] = false;
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

void Bipartite::GreedyMatch()
{
  short u;
  BEdge *e;
  short count = 0;

  for (u = 1; u <= U->size; u++)
    {
      e = U->A[u].firstEdge;
      while (e != (BEdge *) 0)
	{
	  if (!V->A[e->h].mate)
	    {
	      V->A[e->h].mate = u;
	      U->A[u].mate = e->h;
	      break;
	    }
	  e = e->next;
	}
    }
}

short Bipartite::MeasureMatching()
{
  short size = 0, u;
  for (u = 1; u <= U->size; u++) if (U->A[u].mate) size++;
  return size;
}

void Bipartite::Match( vector<short> & result )
{
  GreedyMatch();
  Inits();
  FindPaths();
  findCritical();
  result[0] = MeasureMatching();
  for ( short i = 1; i <= U->size; i++ )
	  result[i] = (U->A[i].critical)? result[0] - 1 : result[0];
}

void Bipartite::findCritical()
{
	short tail = 0;
	short head = 0;
	BEdge* e1;
	BEdge* e2;
	short u;
	for ( u = 1; u <= U->size; u++ )
	{
		visited[u] = false;
		if ( !U->A[u].mate )
		{
			queue[tail++].u = u; //put unmated vertices into queue 
			U->A[u].critical = false; //unmated vertices are not critical
		}
		else U->A[u].critical = true; //potential critical vertex in U (X)
	}
	for ( u = U->size + 1; u <= U->size + V->size; u++ ) visited[u] = false; 
	visited[0] = true; //unmatched vertices in V are pointing to this one
	while ( tail > head )
	{
		u = queue[head++].u;
		e1 = U->A[u].firstEdge;
		while ( e1 != (BEdge *) 0 )
		{
			if ( (e1->h != U->A[u].mate) && !visited[e1->h + U->size] )
			{
				visited[e1->h + U->size] = true;
				if ( !visited[V->A[e1->h].mate] )
				{
					visited[V->A[e1->h].mate] = true;
					U->A[V->A[e1->h].mate].critical = false;
					queue[tail++].u = V->A[e1->h].mate;
				}
			}
			e1 = e1->next;
		}
	}
}

void Bipartite::PrintStatus()
{
  //short u, v;

  //printf("printing status...\n");

  //for (v = 1; v <= V->size; v++)
  //  {
  //    printf("v%d %d\n", v, V->A[v].mate);
  //  }
  //for (u = 1; u <= U->size; u++)
  //{
	 // cout << "vertex" << u << ": " << U->A[u].critical << endl;
  //}
}

//int main(int argc, char* argv[])
//{
//  short matchingSize;
//  Bipartite bPar1;
//  
//  char c;
//  short u, v;
//
//  ifstream dataFile("bbtest1_10.txt");
//  if (!dataFile)
//  {
//	  cerr << "cannot open input file!" << endl;
//	  exit(1);
//  }
//
//  dataFile >> bPar1.U->size;
//  dataFile >> bPar1.V->size;
//  dataFile >> bPar1.U->edges;
//
//  for ( int i = 0; i < bPar1.U->edges; i++ )
//  {
//	dataFile >> c;
//	dataFile >> u;
//	dataFile >> v;
//	bPar1.AddEdge(u,v);
//  }
//
//  dataFile.close();
//  
//  bPar1.Match();
//  bPar1.findCritical();
//  //bPar1.PrintStatus();
//
//  matchingSize = bPar1.MeasureMatching();
//
//  printf("n=%d, m=%d, size=%d (%.1f%%)\n",
//	 bPar1.U->size, bPar1.U->edges, matchingSize, 
//	 (float) 100*matchingSize/bPar1.U->size);
//
//	return 0;
//}
