/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Class: FreeTree

   Description: Used to store one tree, i.e., a transaction in
   the database.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <set>
#include <fstream>
#include <string>
#include <cmath>
#include "misc.h"
#include "FreeTree.h"
using namespace std;

FreeTree::FreeTree(short v, long t) :
  vCount(v), tid(t), adj(v+1), vertexLabel(v+1), degree(v+1)
{
  for (short i = 0; i <= vCount; i++)
  {
    adj[i] = 0;
	degree[i] = 0;
  }
}

FreeTree::FreeTree(long t) :
  vCount(0), tid(t), adj(0), vertexLabel(0), degree(0)
{ 
}

FreeTree::FreeTree() : 
  vCount(0), tid(0), adj(0), vertexLabel(0), degree(0)
{
}

//deep copy constructor, don't change the order in adj list
FreeTree::FreeTree(const FreeTree& rhs)
{
	vCount = rhs.vCount;
	tid = rhs.tid;
	adj.resize(vCount + 1);
	TnodeLink t1, t2, t3;
	for ( short i = 1; i <= vCount; i++ )
	{
		t1 = rhs.adj[i];
		if ( t1 == 0 ) //unlike to happen for a tree
		{
			adj[i] = 0;	
			continue;
		}
		else
		{
			t2 = new Tnode(*t1);
			adj[i] = t2;
			while ( t1->next != 0 )
			{
				t1 = t1->next;
				t3 = new Tnode(*t1);
				t2->next = t3;
				t2 = t3;
			}
		}
	}
	vertexLabel = rhs.vertexLabel;
	degree = rhs.degree;
}//copy constructor

FreeTree& FreeTree::operator=(const FreeTree& rhs)
{
	vCount = rhs.vCount;
	tid = rhs.tid;
	adj.resize(vCount + 1);
	TnodeLink t1, t2, t3;
	for ( short i = 1; i <= vCount; i++ )
	{
		t1 = rhs.adj[i];
		if ( t1 == 0 ) //unlike to happen for a tree
		{
			adj[i] = 0;	
			continue;
		}
		else
		{
			t2 = new Tnode(*t1);
			adj[i] = t2;
			while ( t1->next != 0 )
			{
				t1 = t1->next;
				t3 = new Tnode(*t1);
				t2->next = t3;
				t2 = t3;
			}
		}
	}
	vertexLabel = rhs.vertexLabel;
	degree = rhs.degree;
	return *this;
}


FreeTree::~FreeTree()
{
  TnodeLink holder1;
  TnodeLink holder2;
  for (short i = 1; i <= vCount; i++)	//there is nothing to do with adj[0]
    {
      holder1 = adj[i];
      while (holder1 != 0)
		{
			holder2 = holder1->next;
			delete holder1;
			holder1 = holder2;
		}
    }
}

short FreeTree::vertexNumber() const { return vCount; }
long FreeTree::returnTid() const { return tid; }

void FreeTree::changeVertexLabel(short idx, short vLabel)
{
	vertexLabel[idx] = vLabel;
}


void FreeTree::insertEdge(Edge e)
{
  short x = e.vertex1, y = e.vertex2;
  short c = e.eLabel;
  TnodeLink n = new Tnode(x, c, adj[y]);
  adj[y] = n;
  degree[y]++; 
  n = new Tnode(y, c, adj[x]);
  adj[x] = n;
  degree[x]++;
}

//Be careful! will make tree disconnected
void FreeTree::deleteEdge(short i, short j)
{
	TnodeLink t1, t2;

	//traverse link list of node i, to find edge i-j
	t1 = adj[i];
	if ( t1->v == j )
	{
		adj[i] = t1->next;
		delete t1;
		degree[i]--;
	}
	else
	{
		t2 = t1;
		t1 = t1->next;
		while ( t1 != 0 )
		{
			if ( t1->v == j )
			{
				t2->next = t1->next;
				delete t1;
				degree[i]--;
				break;
			}
			else
			{
				t2 = t1;
				t1 = t1->next;
			}
		}
	}

	//traverse link list of node j, to find edge j-i
	t1 = adj[j];
	if ( t1->v == i )
	{
		adj[j] = t1->next;
		delete t1;
		degree[j]--;
	}
	else
	{
		t2 = t1;
		t1 = t1->next;
		while ( t1 != 0 )
		{
			if ( t1->v == i )
			{
				t2->next = t1->next;
				delete t1;
				degree[j]--;
				break;
			}
			else
			{
				t2 = t1;
				t1 = t1->next;
			}
		}
	}
}


//i = 0 means delete any leaf
bool FreeTree::deleteLeaf(short i, short& vLabel, short& eLabel, short& connTo)
{
	short j, k;
	TnodeLink t1, t2;

	if ( vCount <= 1 ) return false;
	if ( i != 0 )
	{
		if ( i > vCount || degree[i] != 1 ) return false;
		else j = i;
	}
	else
	{
		j = 1;
		while ( degree[j] != 1 ) ++j;
	}

	//delete the edge from leaf to parent
	vLabel = vertexLabel[j];
	eLabel = adj[j]->eLabel;
	k = adj[j]->v;
	connTo = (k > j)? k - 1 : k;
	degree[k]--;
	delete adj[j];
	t1 = adj[k];
	if ( t1->v == j ) 
	{
		adj[k] = t1->next;
		delete t1;
	}
	else
	{
		while ( t1->v != j ) 
		{
			t2 = t1;
			t1 = t1->next;
		}
		t2->next = t1->next;
		delete t1;
	}

	//delete from adj list
	for ( k = j; k < vCount; k++ )
	{
		adj[k] = adj[k+1];
		degree[k] = degree[k+1];
		vertexLabel[k] = vertexLabel[k+1];
	}

	//decrement vCount
	vCount--;

	for ( k = 1; k <= vCount; k++ )
	{
		t1 = adj[k];
		while ( t1 != 0 )
		{
			if ( t1->v > j ) t1->v = t1->v - 1;
			t1 = t1->next;
		}
	}

	return true;
}

istream& operator>>(istream& in, FreeTree& rhs)
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
return in;
}

ostream& operator<<(ostream& out, const FreeTree& rhs)
{
  out << rhs.tid << endl;
  out << rhs.vCount << endl;
  for (short i = 1; i <= rhs.vCount; i++)
	out << rhs.vertexLabel[i] << endl;

  TnodeLink n;
  for (short i = 1; i <= rhs.vCount; i++)
  {
	n = rhs.adj[i];
	while (n != 0) 
	{
		if ( i < n->v )
		{
			out << n->eLabel << ' ' << i << ' ' << n->v << endl;
		}
		n = n->next;
    }
  }
  return out;
}  

