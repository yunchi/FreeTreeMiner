/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Class: TupleTree

   Description: Used to store a tuple and compute supports
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <set>
#include <queue>
#include <fstream>
#include <string>
#include <cmath>
#include "misc.h"
#include "FreeTree.h"
#include "TupleTree.h"
#include "Bipartite.h"

using namespace std;

TupleTree::TupleTree(short v, long t) :
  FreeTree(v, t)
{
	SVU.resize(v+1);
	for ( short i = 0; i <= v; i++ )
		SVU[i].resize(v+1);
	IUW.resize(v+1);
	for ( short i = 0; i <= v; i++ )
		IUW[i].resize(v+1);
}

TupleTree::TupleTree(long t) :
  FreeTree(t)
{ 
}

TupleTree::TupleTree(const FreeTree& rhs) :
FreeTree(rhs)
{
  long t;	//tid
  short v;	//number of vertices

	t = tid;
	v = vCount;

  SVU.resize(v+1); //reserve enough space for s(v,u)
  for ( short i = 0; i <= v; i++ )
	SVU[i].resize(v+1);

  IUW.resize(v+1); //reserve enough space for I(u,w)
  for ( short i = 0; i <= v; i++ )
	IUW[i].resize(v+1);

  //find the maximal fan out
  short j = 0;
  for ( short i = 1; i <= vCount; i++ )
	  if ( degree[i] > j ) j = degree[i];
	for ( short i = 0; i <= vCount; i++ )
		for ( short k = 0; k <= vCount; k++ )
			SVU[i][k].resize(j+3); //the extra sizes are for potential candidates
										//that have more fan outs

}


TupleTree::TupleTree() : 
  FreeTree()
{
}

TupleTree::~TupleTree()
{
//	IUW.clear();
//	SVU.clear();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
function rootIt()
vertex #1 is the root, put all vertices into list of vertex, reversely
a queue is used, and a special symbol (-1) is used to indicate a round
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TupleTree::rootIt() 
{
	short i;
	TnodeLink t;
	queue<short> tempQ;
	vector<bool> tempFlag(vCount+1, false); 

	/******************************************************************
	first step: filling in levels for all vertices
	******************************************************************/
	tempQ.push(1);
	tempQ.push(-1); //-1 is used to indicate the end of a level

	while ( !tempQ.empty() )
	{
		i = tempQ.front();
		tempQ.pop();
		if ( i == -1 ) //a new level
		{
			if ( tempQ.empty() ) continue; //exit while loop
			tempQ.push(-1);
		}
		else
		{
			tempFlag[i] = true;
			vertexList.push_front(i);
			t = adj[i];
			while ( t != 0 )
			{
				if ( !tempFlag[t->v] ) //not parent
				{
					tempQ.push(t->v);
					tempFlag[t->v] = true;
				}
				t = t->next;
			}
		}
	}
}

//assuming rootIt() has been called
//assuming the candidate has more than one vertex
bool TupleTree::support(const FreeTree& rhs)
{
	short i, j, k, m, n;
	TnodeLink t, t1, t2;
	vector<short> result;
	vector<bool> visited(vCount+1,false);

	if ( vCount < rhs.vCount ) return false;

	result.resize(vCount + 1);

	////initialize s(v,u) for each pair of (v,u)
	//for ( i = 1; i <= vCount; i++ )
	//	for ( j = 1; j <= rhs.vCount; j++ )
	//	{
	//		for ( k = 0; k < SVU[i][j].size(); k++ )
	//			SVU[i][j][k] = false;
	//	}

	//initialize I(u,w) for each pair of (u,w)
	for ( i = 1; i <= rhs.vCount; i++ )
	{
		j = 0;
		t = rhs.adj[i];
		while ( t != 0 )
		{
			IUW[i][t->v] = ++j; //[u][w]: index of w as neighbor of u
			t = t->next;
		}
	}

	bPar.clearMe();

	list<short>::iterator pos;
	for ( pos = vertexList.begin(); pos != vertexList.end(); ++pos ) //for each v in V
	{
		i = *pos;
		visited[i] = true;
		if ( degree[i] == 1 && i != vertexList.back() ) //a leaf in V (not root)
		{
			for ( j = 1; j <= rhs.vCount; j++ ) //for all leaves of U
				//if ( rhs.degree[j] == 1 && rhs.vertexLabel[j] == vertexLabel[i] )
				//		SVU[i][j][1] = true;
				if ( rhs.degree[j] == 1 && rhs.vertexLabel[j] == vertexLabel[i] )
						SVU[i][j][1] = true;
				else
				{
					for ( m = 1; m <= rhs.degree[j]; m++ )
						SVU[i][j][m] = false;
				}
		}
		else //internal nodes of V
		{
			for ( j = 1; j <= rhs.vCount; j++ ) //for all nodes of U
			{
				if ( rhs.vertexLabel[j] == vertexLabel[i] &&
					 rhs.degree[j] <= degree[i] ) //degree[u] <= degree[v]-1+1
				{
					bPar.U->size = rhs.degree[j];
					if ( i == 1 ) bPar.V->size = degree[i]; //i is the root!
					else bPar.V->size = degree[i] - 1 + 1;
					k = 0; //total edge number
					t1 = adj[i];
					m = 1;
					while ( t1 != 0 )
					{
						t2 = rhs.adj[j];
						n = 1;
						while ( t2 != 0 )
						{
							if ( visited[t1->v] && //not the parent of i in V
								 t1->eLabel == t2->eLabel && //the same edge label
								 SVU[t1->v][t2->v][IUW[t2->v][j]] ) //u in SVU(v',u')
							{
								bPar.AddEdge(n,m);
								k++;
							}
							t2 = t2->next;
							n++;
						}
						t1 = t1->next;
						m++;
					}
					bPar.U->edges = k;
					bPar.Match(result);
					if ( result[0] == rhs.degree[j] ) return true;
					else 
					{
						for ( k = 1; k <= rhs.degree[j]; k++ )
							//if ( result[k] == rhs.degree[j] - 1 )
							//	SVU[i][j][k] = true;
						{
							if ( result[k] == rhs.degree[j] - 1 )
								SVU[i][j][k] = true;
							else SVU[i][j][k] = false;
						}
					}
					bPar.clearMe();
				}
				else
				{
					for ( m = 1; m <= rhs.degree[j]; m++ )
						SVU[i][j][m] = false;
				}
			}
		}
	}
	return false;
}

void TupleTree::test()
{
	list<short>::iterator pos;
	for ( pos = vertexList.begin(); pos != vertexList.end(); ++pos )
		cout << *pos << ' ' ;
	cout << endl;

	short i, j, k;

	for ( i = 1; i <= vCount; i++ )
	{
		cout << "v: " << i << endl;
		for ( j = 1; j <= vCount; j++ )
		{
			for ( k = 1; k < SVU[i][j].size(); k++ )
			{
				cout << (short) SVU[i][j][k];
			}
			cout << endl;
		}
		cout << endl;
	}

	for ( i = 1; i <= vCount; i++ )
	{
		cout << "v: " << i << endl;
		for ( j = 1; j < IUW[i].size(); j++ )
				cout << IUW[i][j];
		cout << endl;
	}

}

istream& operator>>(istream& in, TupleTree& rhs)
{
  long t;	//tid
  short v;	//number of vertices
  short p1, p2;
  short vLabel, eLabel;

  in >> t;
  in >> v;

  rhs.adj.resize(v+1);
  rhs.vertexLabel.resize(v+1);
  rhs.degree.resize(v+1);

  rhs.SVU.resize(v+1); //reserve enough space for s(v,u)
  for ( short i = 0; i <= v; i++ )
	rhs.SVU[i].resize(v+1);

  rhs.IUW.resize(v+1); //reserve enough space for I(u,w)
  for ( short i = 0; i <= v; i++ )
	rhs.IUW[i].resize(v+1);


  for (short i = 0; i <= v; i++)
  {
	rhs.adj[i] = 0;
	rhs.degree[i] = 0;
  }

  rhs.tid = t;
  rhs.vCount = v;
  rhs.vertexLabel[0] = 0;	//null
  for (short i = 1; i <= v; i++)
  {
	in >> vLabel;
	rhs.vertexLabel[i] = vLabel;
  }
	  
  for (short i = 1; i < v; i++) //the number of edges is v-1
  {
	in >> eLabel;
	in >> p1;
	in >> p2;
	rhs.insertEdge(Edge(p1,p2,eLabel));
  }

  //find the maximal fan out
  short j = 0;
  for ( short i = 1; i <= rhs.vCount; i++ )
	  if ( rhs.degree[i] > j ) j = rhs.degree[i];
	for ( short i = 0; i <= rhs.vCount; i++ )
		for ( short k = 0; k <= rhs.vCount; k++ )
			rhs.SVU[i][k].resize(j+3); //the extra sizes are for potential candidates
										//that have more fan outs
return in;
}

ostream& operator<<(ostream& out, const TupleTree& rhs)
{
  out << "Tid: " << rhs.tid << endl;
  out << "Vertices Number: " << rhs.vCount << endl;
  for (short i = 1; i <= rhs.vCount; i++)
    {
		out << rhs.vertexLabel[i] << ' ' 
		<< rhs.degree[i] << ':';
		TnodeLink n = rhs.adj[i];
		while (n != 0) 
		{
			out << "--" << n->v << '_' << n->eLabel << ' ';
			n = n->next;
		}
		out << endl;
    }
  return out;
}  

