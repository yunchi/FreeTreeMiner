/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   Class: CanonicalTree

   Description: Used to store a tree in its canonical normal form
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
#include "CanonicalTree.h"

using namespace std;

CanonicalTree::CanonicalTree(short v, long t) :
  FreeTree(v, t), level(v+1), parent(v+1), edgeLabelToParent(v+1), automorphism(v+1)
{
}

CanonicalTree::CanonicalTree(long t) :
  FreeTree(t)
{ 
}

CanonicalTree::CanonicalTree() : 
  FreeTree()
{
}

CanonicalTree::CanonicalTree(const CanonicalTree& rhs)
  : FreeTree(rhs)
{
	root1 = rhs.root1;
	root2 = rhs.root2;
	level = rhs.level;
	parent = rhs.parent;
	edgeLabelToParent = rhs.edgeLabelToParent;
	automorphism = rhs.automorphism;
	canonicalString = rhs.canonicalString;
}

CanonicalTree::CanonicalTree(const vector<short>& cString)
{
	tid = 0; //there is no tid information in this case
	short idx = 0;
	short vertexIndex;
	short parentIndex;
	
	vCount = cString[1];
	adj.resize(vCount+1);
	vertexLabel.resize(vCount+1);
	degree.resize(vCount+1);
	
	level.resize(vCount+1);
	parent.resize(vCount+1);
	edgeLabelToParent.resize(vCount+1);
	automorphism.resize(vCount+1);

	for (short i = 0; i <= vCount; i++)
	{
		adj[i] = 0;
		degree[i] = 0;
	}

	if ( cString[0] == 1 )
	{
		vertexLabel[1] = cString[3];
		vertexIndex = 2;
		parentIndex = 1;
		idx = 5; //skip the next ENDTAG
	}
	else if ( cString[0] == 2 )
	{
		vertexLabel[1] = cString[3];
		vertexLabel[2] = cString[5];
		insertEdge(Edge(1,2,cString[4]));
		vertexIndex = 3;
		parentIndex = 1;
		idx = 7; //skip the next ENDTAG
	}
	else
	{
		cerr << "wrong string!" << endl;
		exit(1);
	}

	if ( vCount == 1 || vCount == 2 ) return;

	while ( cString[idx] != ENDOFTREETAG )
	{
		if ( cString[idx] == ENDTAG )
		{
			parentIndex++;
			idx++;
			continue;
		}
		else
		{
			vertexLabel[vertexIndex] = cString[idx+1];
			insertEdge(Edge(vertexIndex,parentIndex,cString[idx]));
			vertexIndex++;
		}
		idx+=2;
	}
}


CanonicalTree& CanonicalTree::operator=(const CanonicalTree& rhs)
{
	FreeTree::operator =((FreeTree)rhs); //is this the correct way?
	root1 = rhs.root1;
	root2 = rhs.root2;
	level = rhs.level;
	parent = rhs.parent;
	edgeLabelToParent = rhs.edgeLabelToParent;
	automorphism = rhs.automorphism;
	canonicalString = rhs.canonicalString;	
	return *this;
}


CanonicalTree::~CanonicalTree()
{
}

bool CanonicalTree::deleteLeaf(short i, short& vLabel, short& eLabel, short& conTo)
{
	bool temp;
	temp = FreeTree::deleteLeaf(i, vLabel, eLabel, conTo);
	if ( temp )
	{
		level.resize(level.size() - 1);
		parent.resize(parent.size() - 1);
		edgeLabelToParent.resize(edgeLabelToParent.size() - 1);
		automorphism.resize(automorphism.size() - 1);
	}
	else
		return temp;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
function findRoot()
to find the root or a pair of roots of a free tree
a queue is used, and a special symbol (-1) is used to indicate a round
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CanonicalTree::findRoot() 
{
	short i;
	vector <short> degreeTemp(degree); //a copy of "degree"
	short totalDegree = 0; //total degrees of the tree
	for ( i = 1; i <= vCount; i++ )
		totalDegree += degreeTemp[i];

	if ( totalDegree == 0) //trivial case, single vertex 
	{
		root1 = 1;
		root2 = 0;
		return;
	}
	else if ( totalDegree == 2) //trivial case, a pair of vertices
	{
		root1 = 1;
		root2 = 2;
		return;
	}

	queue<short> tempQ;

	for ( i = 1; i <= vCount; i++ )
		if ( degreeTemp[i] == 1 )
			tempQ.push(i); //find all leaves

	tempQ.push(-1); //a round

	TnodeLink t;

	while ( true ) 
	{
		i = tempQ.front();
		tempQ.pop();
		if ( i == -1 ) //finished one round
		{
			if ( totalDegree <= 2 ) break;
			else 
			{
				tempQ.push(-1);
				continue;
			}
		}
		degreeTemp[i] = 0;
		totalDegree-=2;
		t = adj[i];
		while ( degreeTemp[t->v] == 0 ) t = t->next; //find the only ngbd with nonzero degree
		if ( (--degreeTemp[t->v]) == 1) tempQ.push(t->v);
	}

	root1 = tempQ.front();
	tempQ.pop();
	if ( totalDegree == 2 ) root2 = tempQ.front();
	else root2 = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
function normalize()
to normalize a free tree
parameter idx is the index of a node in the original tree, after calling
this function, this index is changed to the first vertex automorphism to
the original one (the new index is relative the the canonical form)
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short CanonicalTree::normalize(short idx) //default idx = 0 
{
	short i;
	TnodeLink t;
	queue<short> tempQ;
	vector<bool> tempFlag(vCount+1); //assuming inintal values are false
	vector<short> vertexByLevel(vCount+1); //partition vertices by levels
	vector<short> vertexOrder(vCount+1); //the order of each vertex, in a level
	vector<short> vertexPerLevel(vCount+1); //how many vertices in each level
	short currentLevel = 0; //current level for counting
	short currentVertex = 0; //current vertex for counting

	/******************************************************************
	step 0: finding root(s)
	******************************************************************/
	findRoot();

	/******************************************************************
	first step: filling in levels for all vertices
	******************************************************************/
	tempQ.push(root1);
	tempFlag[root1] = true;
	parent[root1] = 0;
	edgeLabelToParent[root1] = 0; //no parent edge
	if ( root2 != 0 ) 
	{
		tempQ.push(root2);
		tempFlag[root2] = true;
		parent[root2] = 0;
		t = adj[root1];
		while ( t->v != root2 ) t = t->next;
		edgeLabelToParent[root1] = t->eLabel;
		edgeLabelToParent[root2] = t->eLabel;
	}
	tempQ.push(-1); //-1 is used to indicate the end of a level

	vertexPerLevel[0] = 0; //initialize the number of vertices at level 0
	currentLevel = 1;
	vertexPerLevel[currentLevel] = 0; //initialize the number of vertices at first level

	while ( !tempQ.empty() )
	{
		i = tempQ.front();
		tempQ.pop();
		if ( i == -1 ) //a new level
		{
			if ( tempQ.empty() ) continue; //exit while loop
			currentLevel++;
			vertexPerLevel[currentLevel] = 0; 
			tempQ.push(-1);
		}
		else
		{
			level[i] = currentLevel;
			tempFlag[i] = true;
			vertexByLevel[++currentVertex] = i;
			vertexPerLevel[currentLevel]++;
			t = adj[i];
			while ( t != 0 ) //push all its children into queue
			{
				if ( !tempFlag[t->v] )
				{
					tempQ.push(t->v);
					tempFlag[t->v] = true;
					parent[t->v] = i;
					edgeLabelToParent[t->v] = t->eLabel;
				}
				t = t->next;
			}
		}
	}

	//now, currentLevel holds the level of the rooted tree

	/******************************************************************
	second step: clear the original adjacency list
	******************************************************************/
	TnodeLink holder1;
	TnodeLink holder2;
	for ( i = 1; i <= vCount; i++ )	//there is nothing to do with adj[0]
    {
		degree[i] = 0;
		holder1 = adj[i];
		while (holder1 != 0)
		{
			holder2 = holder1->next;
			delete holder1;
			holder1 = holder2;
		}
		adj[i] = 0;
    }

	/******************************************************************
	third step: sorting, bottom up
	******************************************************************/
	//accumulated total:
	for ( i = 1; i <= currentLevel; i++ ) vertexPerLevel[i]+=vertexPerLevel[i-1];

	short j,k,m,n,key;
	//sort vertices, level by level
	for ( j = currentLevel; j > 0; j-- )
	{
		//insertion sort
		for ( k = vertexPerLevel[j-1] + 2; k <= vertexPerLevel[j]; k++ )
		{
			key = vertexByLevel[k];
			m = k - 1;
			while ( (m > vertexPerLevel[j-1] ) && 
				(compareTwoVertices(key,vertexByLevel[m],vertexOrder) == 1) )
			{
				vertexByLevel[m+1] = vertexByLevel[m];
				m--;
			}
			vertexByLevel[m+1] = key;
		}

		//fill in correct order for the given level, order could be equal
		n = vertexPerLevel[j-1] + 1;
		vertexOrder[vertexByLevel[vertexPerLevel[j-1]+1]] = n;
		for ( k = vertexPerLevel[j-1] + 2; k <= vertexPerLevel[j]; k++ )
		{
			if (compareTwoVertices(vertexByLevel[k-1],vertexByLevel[k],vertexOrder) == 1)
				n++;
			vertexOrder[vertexByLevel[k]] = n;
		}

		//add the edges at the given level, other than root
		//the resulting adj list is like this: first node is for the edge to
		//parent, then the edges to children in order
		if ( j > 1 )
		{
			for ( k = vertexPerLevel[j]; k >= vertexPerLevel[j-1] + 1; k-- )
			{
				insertEdge(Edge(vertexByLevel[k],
								parent[vertexByLevel[k]],
								edgeLabelToParent[vertexByLevel[k]]));
			}
		}
		else if ( root2 != 0 ) // j=1, root level, and two roots
				insertEdge(Edge(root1,root2,edgeLabelToParent[root1]));
	}

	/******************************************************************
	forth step: compute the automorphism
	******************************************************************/
	automorphism[root1] = root1;
	if ( root2 != 0 ) //double roots
	{
		if ( vertexOrder[root1] == vertexOrder[root2] )
			automorphism[root2] = root1;
		else automorphism[root2] = root2;
	}

	//for each level, first refine sorting so that the automorphism of 
	//parent plays a role, then compute automorphism according to the order
	for ( j = 2; j <= currentLevel; j++ )
	{
		//insertion sort
		for ( k = vertexPerLevel[j-1] + 2; k <= vertexPerLevel[j]; k++ )
		{
			key = vertexByLevel[k];
			m = k - 1;
			while ( (m > vertexPerLevel[j-1] ) && 
				((vertexOrder[key] < vertexOrder[vertexByLevel[m]]) || //not likely
				 ((vertexOrder[key] == vertexOrder[vertexByLevel[m]]) &&
				 (automorphism[parent[key]] < automorphism[parent[vertexByLevel[m]]]))) )
			{
				vertexByLevel[m+1] = vertexByLevel[m];
				m--;
			}
			vertexByLevel[m+1] = key;
		}
		automorphism[vertexByLevel[vertexPerLevel[j-1]+1]] = 
			vertexByLevel[vertexPerLevel[j-1]+1];
		for ( k = vertexPerLevel[j-1] + 2; k <= vertexPerLevel[j]; k++ )
		{
			if ( ((vertexOrder[vertexByLevel[k]] == vertexOrder[vertexByLevel[k-1]]) &&
				(automorphism[parent[vertexByLevel[k]]] == automorphism[parent[vertexByLevel[k-1]]])) )
				automorphism[vertexByLevel[k]] = automorphism[vertexByLevel[k-1]];
			else automorphism[vertexByLevel[k]] = vertexByLevel[k];
		}
	
	
	}

	/******************************************************************
	fifth step: compute the canonical string
	******************************************************************/
	bool foundIndex = false;
	while ( !tempQ.empty() ) tempQ.pop(); //clear the queue
	for ( i = 0; i < vCount+1; i++ ) tempFlag[i] = false; //clear the flag array
	canonicalString.clear();

	currentVertex = vCount; //remaining number of vertices
	if ( root2 == 0 ) //single root
	{
		canonicalString.push_back(1);
		canonicalString.push_back(vCount); //number of vertics in the tree
		tempQ.push(root1);
		tempQ.push(-1);
		tempFlag[root1] = true;
	}
	else
	{
		canonicalString.push_back(2);
		canonicalString.push_back(vCount); //number of vertics in the tree
		if ( vertexOrder[root1] <= vertexOrder[root2] )
		{
			tempQ.push(root1);
			tempQ.push(root2);
		}
		else
		{
			tempQ.push(root2);
			tempQ.push(root1);
		}
		tempQ.push(-1);
		tempFlag[root1] = true;
		tempFlag[root2] = true;
	}
	
	while ( true )
	{
		if ( currentVertex == 0 ) //finished
		{
			canonicalString.push_back(ENDOFTREETAG);
			break;
		}

		i = tempQ.front();
		tempQ.pop();
		if ( i == -1 )
			canonicalString.push_back(ENDTAG);
		else 
		{
			if ( (idx != 0) && !foundIndex && (automorphism[idx] == automorphism[i]) )
			{
				foundIndex = true;
				idx = vCount - currentVertex + 1;
			}
			tempFlag[i] = true;
			canonicalString.push_back(edgeLabelToParent[i]);
			canonicalString.push_back(vertexLabel[i]);
			t = adj[i];
			while ( t != 0 )
			{
				if ( tempFlag[t->v] == false )
					tempQ.push(t->v);
				t = t->next;
			}
			tempQ.push(-1);
			currentVertex--;
		}
	}


	//for test
	//cout << "vertexByLevel: ";
	//for ( i = 1; i <= vCount; i++ ) cout << vertexByLevel[i] << ' ';
	//cout << endl;
	//cout << "vertexPerLevel: ";
	//for ( i = 0; i <= vCount; i++ ) cout << vertexPerLevel[i] << ' ';
	//cout << endl;
	//cout << "vertexOrder: ";
	//for ( i = 1; i <= vCount; i++ ) cout << vertexOrder[i] << ' ';
	//cout << endl;
	//cout << "automorphism: ";
	//for ( i = 1; i <= vCount; i++ ) cout << automorphism[i] << ' ';
	//cout << endl;
	//cout << "canonical string: ";
	//for ( i = 0; i < canonicalString.size(); i++ ) cout << canonicalString[i] << ' ';
	//cout << endl;

	return idx;


}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
function compareTwoVertices(short, short, vector<short>&)
to compare two vertices at the same level of the rooted tree
comparison based on edge (to parent) label first, then vertex label
return: 0 if v1 = v2, 1 if v1 < v2, 2 if v1 > v2
assuming the lower level has been ordered already
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short CanonicalTree::compareTwoVertices(short v1, short v2, vector<short>& vertexOrder)
{
	if ( edgeLabelToParent[v1] < edgeLabelToParent[v2] ) return 1;
	else if ( edgeLabelToParent[v1] > edgeLabelToParent[v2] ) return 2;
	else if ( vertexLabel[v1] < vertexLabel[v2] ) return 1;
	else if ( vertexLabel[v1] > vertexLabel[v2] ) return 2;
	else return(compareTwoVertices(adj[v1], adj[v2], vertexOrder));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
function compareTwoVertices(TnodeLink, TnodeLink, vector<short>&)
to compare two vertices at the same level of the rooted tree
comparison based on lower level
return: 0 if v1 = v2, 1 if v1 < v2, 2 if v1 > v2
assuming the lower level has been ordered already
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
short CanonicalTree::compareTwoVertices(TnodeLink l1, TnodeLink l2, vector<short>& vertexOrder)
{
	if ( l1 == 0 && l2 == 0 ) return 0;
	else if ( l1 == 0 ) return 2; //empty is the larger one
	else if ( l2 == 0 ) return 1;
	else if ( vertexOrder[l1->v] < vertexOrder[l2->v] ) return 1;
	else if ( vertexOrder[l1->v] > vertexOrder[l2->v] ) return 2;
	else return(compareTwoVertices(l1->next, l2->next, vertexOrder));
}

bool CanonicalTree::operator<(const CanonicalTree& rhs) const
{
	return canonicalString < rhs.canonicalString;
}

void CanonicalTree::setCanonicalString(vector<short>& rhs)
{
	canonicalString = rhs;
}

void CanonicalTree::addVertex(short vLabel)
{
	vCount++;
	adj.push_back(0);
	vertexLabel.push_back(vLabel);
	degree.push_back(0);
	level.push_back(0);
	parent.push_back(0);
	edgeLabelToParent.push_back(0);
	automorphism.push_back(0);
}


void CanonicalTree::canonicalTest() const
{
	vector<short>::const_iterator pos;
	cout << "canonical string is: ";
	for ( pos = canonicalString.begin(); pos != canonicalString.end(); ++pos )
		cout << *pos << ' ';
	cout << endl;
}

istream& operator>>(istream& in, CanonicalTree& rhs)
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

  rhs.level.resize(v+1);
  rhs.parent.resize(v+1);
  rhs.edgeLabelToParent.resize(v+1);
  rhs.automorphism.resize(v+1);

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

ostream& operator<<(ostream& out, const CanonicalTree& rhs)
{
  out << "Tid: " << rhs.tid << endl;
  out << "Vertices Number: " << rhs.vCount << endl;
  for ( short i = 1; i <= rhs.vCount; i++ )
    {
		out << rhs.vertexLabel[i] << ' ' 
		<< rhs.degree[i] << ':';
		TnodeLink n = rhs.adj[i];
		while ( n != 0 ) 
		{
			out << "--" << n->v << '_' << n->eLabel << ' ';
			n = n->next;
		}
		out << endl;
    }
  out << "automorphism: ";
  for ( short i = 1; i <= rhs.vCount; i++ ) cout << rhs.automorphism[i] << ' ';
  out << endl;
  out << "canonical string: ";
  for ( short i = 0; i < rhs.canonicalString.size(); i++ ) cout << rhs.canonicalString[i] << ' ';
  out << endl;
  return out;
}  

