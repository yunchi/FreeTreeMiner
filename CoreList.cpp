/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   CoreList.cpp
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
#include "CoreList.h"

using namespace std;

bool Limb::operator<(const Limb& rhs) const
{
	if ( connectTo < rhs.connectTo ) return true;
	else if ( connectTo > rhs.connectTo ) return false;
	else if ( edgeLabel < rhs.edgeLabel ) return true;
	else if ( edgeLabel > rhs.edgeLabel ) return false;
	else if ( vertexLabel < rhs.vertexLabel ) return true;
	else return false;
}

ostream& operator<<(ostream& out, const Limb& rhs)
{
	out << "Vertex: " << rhs.vertexLabel << ' ';
	out << "Edge: " << rhs.edgeLabel << ' ';
	out << "ConnectTo: " << rhs.connectTo << ' ';
	return out;
}


//return number of elements in the map
long CoreList::returnSize() const
{
	return coreList.size();
}

//populate coreList with frequent k-trees
//assumming the number of leaves is >= 2
void CoreList::readFrom(FrequentTreeList& ftl)
{
	CanonicalTree* pct1;
	CanonicalTree* pct2;
	Limb tempLimb;
	short i, j;
	short v1 = 0;
	short v2 = 0;
	short v3;
	set<Limb> setLimb;
	map<CanonicalTree,supportNode>::iterator pos;
	pair<map< vector<short> ,set<Limb> >::iterator, bool> result;

	for ( pos = ftl.frequentList.begin(); pos != ftl.frequentList.end(); ++pos)
	{
		pct1 = new CanonicalTree();
		*pct1 = pos->first;
		v1 = 0;
		v2 = 0;

		//find out the "top2" labels
		for ( i = 1; i <= pct1->vCount; i++ )
		{
			if ( pct1->degree[i] == 1 ) //leaf node
			{
				if ( pct1->vertexLabel[i] > v2 )
				{
					v2 = pct1->vertexLabel[i];
					if ( v2 > v1 )
					{
						v3 = v2;
						v2 = v1;
						v1 = v3;
					}
				}
			}
		}

		for ( i = 1; i <= pct1->vCount; i++ )
		{
			if ( pct1->degree[i] == 1 && //leaf node
				 ((pct1->vertexLabel[i] == v1) || (pct1->vertexLabel[i] == v2)) )
			{
				pct2 = new CanonicalTree(*pct1);
				pct2->deleteLeaf(i, tempLimb.vertexLabel, tempLimb.edgeLabel, tempLimb.connectTo);
				tempLimb.connectTo = pct2->normalize(tempLimb.connectTo);
				setLimb.clear();
				setLimb.insert(tempLimb);
				result = coreList.insert(make_pair(pct2->canonicalString,setLimb));
				if ( !result.second ) //if the core already exists
				{
					(result.first)->second.insert(tempLimb);
				}
				delete pct2;
			}
		}
		delete pct1;
	}
}

//write out candidate k+1-trees
void CoreList::writeTo(FrequentTreeList& ftl)
{
	CanonicalTree* pct1;
	CanonicalTree* pct2;
	map< vector<short> ,set<Limb> >::iterator pos;
	set<Limb>::iterator pos1;
	set<Limb>::iterator pos2;
	supportNode sNode;
	pair<map<CanonicalTree,supportNode>::iterator,bool> result;
	Limb tempLimb1;
	Limb tempLimb2;
	short i, j, k;

	sNode.lastTid = 0;
	sNode.support = 0;

	for ( pos = coreList.begin(); pos != coreList.end(); ++pos )
	{
		pct1 = new CanonicalTree(pos->first);
		pct1->normalize();	//assuming the automorphism does not change
							//i.e., after normalization, limbs are still valid
		for ( pos1 = (pos->second).begin(); pos1 != (pos->second).end(); ++pos1 )
		{
			//first case, self-join
			tempLimb1 = *pos1;
			for ( i = 1; i <= pct1->vCount; i++ )
			{
				if ( pct1->automorphism[i] == pct1->automorphism[tempLimb1.connectTo] )
				{
					for ( j = i; j <= pct1->vCount; j++ )
					{
						if ( pct1->automorphism[j] == pct1->automorphism[tempLimb1.connectTo] )
						{
							pct2 = new CanonicalTree(*pct1);
							pct2->addVertex(tempLimb1.vertexLabel);
							pct2->insertEdge(Edge(i,pct2->vCount,tempLimb1.edgeLabel));
							pct2->addVertex(tempLimb1.vertexLabel);
							pct2->insertEdge(Edge(j,pct2->vCount,tempLimb1.edgeLabel));
							pct2->normalize();
							ftl.frequentList.insert(make_pair(*pct2,sNode));
							delete pct2;
						}
					}
				}
			}

			//second case, join with others
			tempLimb1 = *pos1;
			for ( pos2 = pos1; pos2 != (pos->second).end(); ++pos2 )
			{
				tempLimb2 = *pos2;
				for ( i = 1; i <= pct1->vCount; i++ )
				if ( pct1->automorphism[i] == pct1->automorphism[tempLimb1.connectTo] )
				{
					for ( j = 1; j <= pct1->vCount; j++ )
					{
						if ( pct1->automorphism[j] == pct1->automorphism[tempLimb2.connectTo] )
						{
							pct2 = new CanonicalTree(*pct1);
							pct2->addVertex(tempLimb1.vertexLabel);
							pct2->insertEdge(Edge(i,pct2->vCount,tempLimb1.edgeLabel));
							pct2->addVertex(tempLimb2.vertexLabel);
							pct2->insertEdge(Edge(j,pct2->vCount,tempLimb2.edgeLabel));
							pct2->normalize();
							ftl.frequentList.insert(make_pair(*pct2,sNode));
							delete pct2;
						}
					}
				}
			}
		}

		delete pct1;
	}
}

void CoreList::test()
{
	map< vector<short> ,set<Limb> >::iterator pos;
	set<Limb>::iterator pos1;
	for ( pos = coreList.begin(); pos != coreList.end(); ++pos )
	{
		for ( short i = 0; i < (pos->first).size(); i++ )
		{
			cout << (pos->first)[i] << ' ';
		}
		cout << endl;
		for ( pos1 = (pos->second).begin(); pos1 != (pos->second).end(); ++pos1 )
		{
			cout << *pos1 << endl;;
		}
		cout << endl;
	}
}
