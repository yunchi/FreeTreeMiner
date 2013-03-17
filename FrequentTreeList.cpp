/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   FrequentTreeList.cpp
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
#include "FrequentTreeList.h"

using namespace std;

ostream& operator <<(ostream& out, const supportNode& rhs)
{
	out << "support is: " << rhs.support << " lastTid is: " << rhs.lastTid << endl;
	return out;
}

//return number of elements in the map
long FrequentTreeList::returnSize() const
{
	return frequentList.size();
}

void FrequentTreeList::populate2(const vector<ptrFreeTree>& database)
{	
	TnodeLink t;
	FreeTree* pft;
	CanonicalTree* pct;
	pair<map<CanonicalTree,supportNode>::iterator,bool> result;
	supportNode sNode;
	char c;

	for ( long s = 1; s < database.size(); s++ )
	{
		pft = database[s];

		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			t = pft->adj[i];
			while ( t != 0 )
			{
				if ( (pft->vertexLabel[i] < pft->vertexLabel[t->v]) || 
					 (( pft->vertexLabel[i] == pft->vertexLabel[t->v])&&(i < t->v)) )
				{
					pct = new CanonicalTree(2,pft->tid);
					pct->changeVertexLabel(1,pft->vertexLabel[i]);
					pct->changeVertexLabel(2,pft->vertexLabel[t->v]);
					pct->insertEdge(Edge(1,2,t->eLabel));
					pct->normalize();
					sNode.lastTid = pft->tid;
					sNode.support = 1;
					sNode.tidList.clear();
					sNode.tidList.push_back(pft->tid);
					result = frequentList.insert(make_pair(*pct,sNode));
					if ( !result.second ) //if the tree already exists
					{
						//if the new tree has a new tid, then the frequency count
						//should be increased by 1
						//NOTE: assuming tids are coming in increasing order
						if ( ((result.first)->second).lastTid != sNode.lastTid )
						{
							((result.first)->second).lastTid = sNode.lastTid;
							((result.first)->second).support++;
							((result.first)->second).tidList.push_back(sNode.lastTid);
						}
					}
					delete pct;
				}
				t = t->next;
			}
		}
	}
	finalize(SUPPORT_THRESHOLD);
}

void FrequentTreeList::populate3(const vector<ptrFreeTree>& database, FrequentTreeList* rhs)
{
	TnodeLink t1,t2;
	FreeTree* pft;
	CanonicalTree* pct;
	CanonicalTree ctDummy(2);
	vector<short> dummy;
	pair<map<CanonicalTree,supportNode>::iterator,bool> result;
	supportNode sNode;
	char c;

	for ( long s = 1; s < database.size(); s++ )
	{
		pft = database[s];

		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			if ( pft->degree[i] < 2 ) continue; //cannot get 3-tree from here
			t1 = pft->adj[i];
			while ( t1->next != 0 )
			{
				t2 = t1->next;
				while ( t2 != 0 )
				{
					dummy.clear();
					dummy.push_back(2); //2 roots
					dummy.push_back(2); //2 vertices
					dummy.push_back(t1->eLabel);
					dummy.push_back(min(pft->vertexLabel[i], pft->vertexLabel[t1->v]));
					dummy.push_back(t1->eLabel);
					dummy.push_back(max(pft->vertexLabel[i], pft->vertexLabel[t1->v]));
					dummy.push_back(ENDOFTREETAG);
					ctDummy.setCanonicalString(dummy);
					if ( !rhs->downwardSupport(ctDummy) )
					{
						t2 = t2->next;
						continue;
					}
					dummy.clear();
					dummy.push_back(2); //2 roots
					dummy.push_back(2); //2 vertices
					dummy.push_back(t2->eLabel);
					dummy.push_back(min(pft->vertexLabel[i], pft->vertexLabel[t2->v]));
					dummy.push_back(t2->eLabel);
					dummy.push_back(max(pft->vertexLabel[i], pft->vertexLabel[t2->v]));
					dummy.push_back(ENDOFTREETAG);
					ctDummy.setCanonicalString(dummy);
					if ( !rhs->downwardSupport(ctDummy) )
					{
						t2 = t2->next;
						continue;
					}

					pct = new CanonicalTree(3,pft->tid);
					pct->changeVertexLabel(1,pft->vertexLabel[i]);
					pct->changeVertexLabel(2,pft->vertexLabel[t1->v]);
					pct->changeVertexLabel(3,pft->vertexLabel[t2->v]);
					pct->insertEdge(Edge(1,2,t1->eLabel));
					pct->insertEdge(Edge(1,3,t2->eLabel));
					pct->normalize();
					sNode.lastTid = pft->tid;
					sNode.support = 1;
					sNode.tidList.clear();
					sNode.tidList.push_back(pft->tid);
					result = frequentList.insert(make_pair(*pct,sNode));
					if ( !result.second ) //if the tree already exists
					{
						//if the new tree has a new tid, then the frequency count
						//should be increased by 1
						//NOTE: assuming tids are coming in increasing order
						if ( ((result.first)->second).lastTid != sNode.lastTid )
						{
							((result.first)->second).lastTid = sNode.lastTid;
							((result.first)->second).support++;
							((result.first)->second).tidList.push_back(sNode.lastTid);
						}
					}
					delete pct;
					t2 = t2->next;
				}
				t1 = t1->next;
			}
		}
	}
	finalize(SUPPORT_THRESHOLD);
}

void FrequentTreeList::populate4(const vector<ptrFreeTree>& database, FrequentTreeList* rhs)
{
	TnodeLink t1,t2,t3;
	FreeTree* pft;
	CanonicalTree* pct;
	CanonicalTree ctDummy(3);
	vector<short> dummy;
	pair<map<CanonicalTree,supportNode>::iterator,bool> result;
	supportNode sNode;
	char c;

	for ( long s = 1; s < database.size(); s++ )
	{
		pft = database[s];

		//the first case, 1 internal vertex, 3 leaves
		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			if ( pft->degree[i] < 3 ) continue; //cannot get 4-tree from here
			t1 = pft->adj[i];
			while ( t1 != 0 )
			{
				t2 = t1->next;
				while ( t2 != 0 )
				{
					t3 = t2->next;
					while ( t3 != 0 )
					{
						if ( POPULATE_CHECK )
						{
							dummy.clear();
							dummy.push_back(1); //1 root
							dummy.push_back(3); //3 vertices
							dummy.push_back(0); //NULL parent for the root
							dummy.push_back(pft->vertexLabel[i]); //label of the root
							dummy.push_back(ENDTAG);
							if ( (t1->eLabel < t2->eLabel) ||
								((t1->eLabel == t2->eLabel)&&
								(pft->vertexLabel[t1->v] < pft->vertexLabel[t2->v])))
							{
								dummy.push_back(t1->eLabel);
								dummy.push_back(pft->vertexLabel[t1->v]);
								dummy.push_back(t2->eLabel);
								dummy.push_back(pft->vertexLabel[t2->v]);
								dummy.push_back(ENDOFTREETAG);
							}
							else
							{
								dummy.push_back(t2->eLabel);
								dummy.push_back(pft->vertexLabel[t2->v]);
								dummy.push_back(t1->eLabel);
								dummy.push_back(pft->vertexLabel[t1->v]);
								dummy.push_back(ENDOFTREETAG);
							}
							ctDummy.setCanonicalString(dummy);
							if ( !rhs->downwardSupport(ctDummy) )
							{
								t3 = t3->next;
								continue;
							}

							dummy.clear();
							dummy.push_back(1); //1 root
							dummy.push_back(3); //3 vertices
							dummy.push_back(0); //NULL parent for the root
							dummy.push_back(pft->vertexLabel[i]); //label of the root
							dummy.push_back(ENDTAG);
							if ( (t1->eLabel < t3->eLabel) ||
								((t1->eLabel == t3->eLabel)&&
								(pft->vertexLabel[t1->v] < pft->vertexLabel[t3->v])))
							{
								dummy.push_back(t1->eLabel);
								dummy.push_back(pft->vertexLabel[t1->v]);
								dummy.push_back(t3->eLabel);
								dummy.push_back(pft->vertexLabel[t3->v]);
								dummy.push_back(ENDOFTREETAG);
							}
							else
							{
								dummy.push_back(t3->eLabel);
								dummy.push_back(pft->vertexLabel[t3->v]);
								dummy.push_back(t1->eLabel);
								dummy.push_back(pft->vertexLabel[t1->v]);
								dummy.push_back(ENDOFTREETAG);
							}
							ctDummy.setCanonicalString(dummy);
							if ( !rhs->downwardSupport(ctDummy) )
							{
								t3 = t3->next;
								continue;
							}

							dummy.clear();
							dummy.push_back(1); //1 root
							dummy.push_back(3); //3 vertices
							dummy.push_back(0); //NULL parent for the root
							dummy.push_back(pft->vertexLabel[i]); //label of the root
							dummy.push_back(ENDTAG);
							if ( (t2->eLabel < t3->eLabel) ||
								((t2->eLabel == t3->eLabel)&&
								(pft->vertexLabel[t2->v] < pft->vertexLabel[t3->v])))
							{
								dummy.push_back(t2->eLabel);
								dummy.push_back(pft->vertexLabel[t2->v]);
								dummy.push_back(t3->eLabel);
								dummy.push_back(pft->vertexLabel[t3->v]);
								dummy.push_back(ENDOFTREETAG);
							}
							else
							{
								dummy.push_back(t3->eLabel);
								dummy.push_back(pft->vertexLabel[t3->v]);
								dummy.push_back(t2->eLabel);
								dummy.push_back(pft->vertexLabel[t2->v]);
								dummy.push_back(ENDOFTREETAG);
							}
							ctDummy.setCanonicalString(dummy);
							if ( !rhs->downwardSupport(ctDummy) )
							{
								t3 = t3->next;
								continue;
							}
						} //end of if(POPULATE_CHECK)

						pct = new CanonicalTree(4,pft->tid);
						pct->changeVertexLabel(1,pft->vertexLabel[i]);
						pct->changeVertexLabel(2,pft->vertexLabel[t1->v]);
						pct->changeVertexLabel(3,pft->vertexLabel[t2->v]);
						pct->changeVertexLabel(4,pft->vertexLabel[t3->v]);
						pct->insertEdge(Edge(1,2,t1->eLabel));
						pct->insertEdge(Edge(1,3,t2->eLabel));
						pct->insertEdge(Edge(1,4,t3->eLabel));
						pct->normalize();
						sNode.lastTid = pft->tid;
						sNode.support = 1;
						sNode.tidList.clear();
						sNode.tidList.push_back(pft->tid);
						result = frequentList.insert(make_pair(*pct,sNode));
						if ( !result.second ) //if the tree already exists
						{
							//if the new tree has a new tid, then the frequency count
							//should be increased by 1
							//NOTE: assuming tids are coming in increasing order
							if ( ((result.first)->second).lastTid != sNode.lastTid )
							{
								((result.first)->second).lastTid = sNode.lastTid;
								((result.first)->second).support++;
								((result.first)->second).tidList.push_back(sNode.lastTid);
							}
						}
						delete pct;
						t3 = t3->next;
					}
					t2 = t2->next;
				}
				t1 = t1->next;
			}
		}

		//the second case, 2 internal vertices, 2 leaves
		for ( short i = 1; i <= pft->vertexNumber(); ++i )
		{
			if ( pft->degree[i] < 2 ) continue; //cannot get 4-tree from here
			t1 = pft->adj[i];
			while ( t1 != 0 )
			{
				t2 = t1->next;
				while ( t2 != 0 )
				{
					if ( i < t1->v )
					{
						t3 = pft->adj[t1->v];
						while ( t3 != 0 )
						{
							if ( (t3->v == i) || (t3->v == t2->v) || (t3->v == t1->v) )
							{
								t3 = t3->next; //backtrack to i, or not a tree(unlikely)
								continue;
							}
							if ( POPULATE_CHECK )
							{
								dummy.clear();
								dummy.push_back(1); //1 root
								dummy.push_back(3); //3 vertices
								dummy.push_back(0); //NULL parent for the root
								dummy.push_back(pft->vertexLabel[i]); //label of the root
								dummy.push_back(ENDTAG);
								if ( (t1->eLabel < t2->eLabel) ||
									((t1->eLabel == t2->eLabel)&&
									(pft->vertexLabel[t1->v] < pft->vertexLabel[t2->v])))
								{
									dummy.push_back(t1->eLabel);
									dummy.push_back(pft->vertexLabel[t1->v]);
									dummy.push_back(t2->eLabel);
									dummy.push_back(pft->vertexLabel[t2->v]);
									dummy.push_back(ENDOFTREETAG);
								}
								else
								{
									dummy.push_back(t2->eLabel);
									dummy.push_back(pft->vertexLabel[t2->v]);
									dummy.push_back(t1->eLabel);
									dummy.push_back(pft->vertexLabel[t1->v]);
									dummy.push_back(ENDOFTREETAG);
								}
								ctDummy.setCanonicalString(dummy);
								if ( !rhs->downwardSupport(ctDummy) )
								{
									t3 = t3->next;
									continue;
								}

								dummy.clear();
								dummy.push_back(1); //1 root
								dummy.push_back(3); //3 vertices
								dummy.push_back(0); //NULL parent for the root
								dummy.push_back(pft->vertexLabel[t1->v]); //label of the root
								dummy.push_back(ENDTAG);
								if ( (t1->eLabel < t3->eLabel) ||
									((t1->eLabel == t3->eLabel)&&
									(pft->vertexLabel[i] < pft->vertexLabel[t3->v])))
								{
									dummy.push_back(t1->eLabel);
									dummy.push_back(pft->vertexLabel[i]);
									dummy.push_back(t3->eLabel);
									dummy.push_back(pft->vertexLabel[t3->v]);
									dummy.push_back(ENDOFTREETAG);
								}
								else
								{
									dummy.push_back(t3->eLabel);
									dummy.push_back(pft->vertexLabel[t3->v]);
									dummy.push_back(t1->eLabel);
									dummy.push_back(pft->vertexLabel[i]);
									dummy.push_back(ENDOFTREETAG);
								}
								ctDummy.setCanonicalString(dummy);
								if ( !rhs->downwardSupport(ctDummy) )
								{
									t3 = t3->next;
									continue;
								}
							} //end of if(POPULATE_CHECK)

							pct = new CanonicalTree(4,pft->tid);
							pct->changeVertexLabel(1,pft->vertexLabel[i]);
							pct->changeVertexLabel(2,pft->vertexLabel[t1->v]);
							pct->changeVertexLabel(3,pft->vertexLabel[t2->v]);
							pct->changeVertexLabel(4,pft->vertexLabel[t3->v]);
							pct->insertEdge(Edge(1,2,t1->eLabel));
							pct->insertEdge(Edge(1,3,t2->eLabel));
							pct->insertEdge(Edge(2,4,t3->eLabel));
							pct->normalize();
							sNode.lastTid = pft->tid;
							sNode.support = 1;
							sNode.tidList.clear();
							sNode.tidList.push_back(pft->tid);
							result = frequentList.insert(make_pair(*pct,sNode));
							if ( !result.second ) //if the tree already exists
							{
								//if the new tree has a new tid, then the frequency count
								//should be increased by 1
								//NOTE: assuming tids are coming in increasing order
								if ( ((result.first)->second).lastTid != sNode.lastTid )
								{
									((result.first)->second).lastTid = sNode.lastTid;
									((result.first)->second).support++;
									((result.first)->second).tidList.push_back(sNode.lastTid);
								}
							}
							delete pct;
							t3 = t3->next;
						} //end of while ( t3 != 0 )
					} //end of if ( i < t1->v )

					if ( i < t2->v )
					{
						t3 = pft->adj[t2->v];
						while ( t3 != 0 )
						{
							if ( (t3->v == i) || (t3->v == t1->v) || (t3->v == t2->v) )
							{
								t3 = t3->next; //backtrack to i, or not a tree(unlikely)
								continue;
							}
							if ( POPULATE_CHECK )
							{
								dummy.clear();
								dummy.push_back(1); //1 root
								dummy.push_back(3); //3 vertices
								dummy.push_back(0); //NULL parent for the root
								dummy.push_back(pft->vertexLabel[i]); //label of the root
								dummy.push_back(ENDTAG);
								if ( (t1->eLabel < t2->eLabel) ||
									((t1->eLabel == t2->eLabel)&&
									(pft->vertexLabel[t1->v] < pft->vertexLabel[t2->v])))
								{
									dummy.push_back(t1->eLabel);
									dummy.push_back(pft->vertexLabel[t1->v]);
									dummy.push_back(t2->eLabel);
									dummy.push_back(pft->vertexLabel[t2->v]);
									dummy.push_back(ENDOFTREETAG);
								}
								else
								{
									dummy.push_back(t2->eLabel);
									dummy.push_back(pft->vertexLabel[t2->v]);
									dummy.push_back(t1->eLabel);
									dummy.push_back(pft->vertexLabel[t1->v]);
									dummy.push_back(ENDOFTREETAG);
								}
								ctDummy.setCanonicalString(dummy);
								if ( !rhs->downwardSupport(ctDummy) )
								{
									t3 = t3->next;
									continue;
								}

								dummy.clear();
								dummy.push_back(1); //1 root
								dummy.push_back(3); //3 vertices
								dummy.push_back(0); //NULL parent for the root
								dummy.push_back(pft->vertexLabel[t2->v]); //label of the root
								dummy.push_back(ENDTAG);
								if ( (t2->eLabel < t3->eLabel) ||
									((t2->eLabel == t3->eLabel)&&
									(pft->vertexLabel[i] < pft->vertexLabel[t3->v])))
								{
									dummy.push_back(t2->eLabel);
									dummy.push_back(pft->vertexLabel[i]);
									dummy.push_back(t3->eLabel);
									dummy.push_back(pft->vertexLabel[t3->v]);
									dummy.push_back(ENDOFTREETAG);
								}
								else
								{
									dummy.push_back(t3->eLabel);
									dummy.push_back(pft->vertexLabel[t3->v]);
									dummy.push_back(t2->eLabel);
									dummy.push_back(pft->vertexLabel[i]);
									dummy.push_back(ENDOFTREETAG);
								}
								ctDummy.setCanonicalString(dummy);
								if ( !rhs->downwardSupport(ctDummy) )
								{
									t3 = t3->next;
									continue;
								}
							} //end of if(POPULATE_CHECK)
							pct = new CanonicalTree(4,pft->tid);
							pct->changeVertexLabel(1,pft->vertexLabel[i]);
							pct->changeVertexLabel(2,pft->vertexLabel[t1->v]);
							pct->changeVertexLabel(3,pft->vertexLabel[t2->v]);
							pct->changeVertexLabel(4,pft->vertexLabel[t3->v]);
							pct->insertEdge(Edge(1,2,t1->eLabel));
							pct->insertEdge(Edge(1,3,t2->eLabel));
							pct->insertEdge(Edge(3,4,t3->eLabel));
							pct->normalize();
							sNode.lastTid = pft->tid;
							sNode.support = 1;
							sNode.tidList.clear();
							sNode.tidList.push_back(pft->tid);
							result = frequentList.insert(make_pair(*pct,sNode));
							if ( !result.second ) //if the tree already exists
							{
								//if the new tree has a new tid, then the frequency count
								//should be increased by 1
								//NOTE: assuming tids are coming in increasing order
								if ( ((result.first)->second).lastTid != sNode.lastTid )
								{
									((result.first)->second).lastTid = sNode.lastTid;
									((result.first)->second).support++;
									((result.first)->second).tidList.push_back(sNode.lastTid);
								}
							}
							delete pct;
							t3 = t3->next;
						} //end of while ( t3 != 0 )
					} //end of if ( i < t2->v )
					
					t2 = t2->next;
				} //end of while ( t2 != 0 )
				t1 = t1->next;
			} //end of while ( t1 != 0 )
		} //end of for loop
	} //end of while ( !dataFile.eof() )
	finalize(SUPPORT_THRESHOLD);
}

//check downward closure property for each candidate
void FrequentTreeList::checkDownward(FrequentTreeList& rhs)
{
	CanonicalTree* pct1;
	CanonicalTree* pct2;
	short dummy1, dummy2, dummy3;
	short i;
	bool tempFlag;

	map<CanonicalTree,supportNode>::iterator pos;
	map<CanonicalTree,supportNode>::iterator pos2;
	list<long>::iterator pos3;

	for ( pos = frequentList.begin(); pos != frequentList.end(); )
	{
		tempFlag = false;
		pct1 = new CanonicalTree(pos->first);
		i = 1;
		while ( pct1->degree[i] != 1 ) i++; //find the first leaf
		pct2 = new CanonicalTree(*pct1);
		pct2->deleteLeaf(i, dummy1, dummy2, dummy3);
		pct2->normalize();
		pos2 = rhs.frequentList.find(*pct2);
		if ( pos2 == rhs.frequentList.end() )
			tempFlag = true;
		else (pos->second).tidList = (pos2->second).tidList;
		delete pct2;

		while ( !tempFlag && i <= pct1->vertexNumber() )
		{
			if ( pct1->degree[i] == 1 ) // a leaf node
			{
				pct2 = new CanonicalTree(*pct1);
				pct2->deleteLeaf(i, dummy1, dummy2, dummy3);
				pct2->normalize();
		
				pos2 = rhs.frequentList.find(*pct2);
				if ( pos2 == rhs.frequentList.end() )
					tempFlag = true;
				else
				{
					(pos->second).tidList.push_back(INFINITY_LONG); //flag of end
					pos3 = (pos2->second).tidList.begin();
					while ( pos3 != (pos2->second).tidList.end() )
					{
						if ( (pos->second).tidList.front() == *pos3 )
						{
							(pos->second).tidList.push_back(*pos3);
							(pos->second).tidList.pop_front();
							pos3++;
						}
						else if ( (pos->second).tidList.front() < *pos3 )
						{
							while ( (pos->second).tidList.front() < *pos3 )
								(pos->second).tidList.pop_front();
						}
						else
						{
							while ( pos3 != (pos2->second).tidList.end() &&
									(pos->second).tidList.front() > *pos3 )
								pos3++;
						}
					}
					while ( (pos->second).tidList.front() != INFINITY_LONG )
						(pos->second).tidList.pop_front();
					(pos->second).tidList.pop_front(); //eat the INFINITY_LONG tag
					if ( (pos->second).tidList.size() < SUPPORT_THRESHOLD )
						tempFlag = false;
				}
				delete pct2;
			}
			i++;
		}
		delete pct1;
		if ( tempFlag )
			frequentList.erase(pos++);
		else ++pos;
	}
}

//count how many trees in F_(k-1) (rhs) are maximum
long FrequentTreeList::countMaximum(FrequentTreeList& rhs)
{
	long totalMaximum;
	CanonicalTree* pct1;
	CanonicalTree* pct2;
	short dummy1, dummy2, dummy3;
	short i;

	map<CanonicalTree,supportNode>::iterator pos;
	map<CanonicalTree,supportNode>::iterator pos2;

	for ( pos = frequentList.begin(); pos != frequentList.end(); ++pos )
	{
		pct1 = new CanonicalTree(pos->first);
		for ( i = 1; i <= pct1->vertexNumber(); i++ )
		{
			if ( pct1->degree[i] == 1 ) // a leaf node
			{
				pct2 = new CanonicalTree(*pct1);
				pct2->deleteLeaf(i, dummy1, dummy2, dummy3);
				pct2->normalize();
				pos2 = rhs.frequentList.find(*pct2);
				if ( pos2 == rhs.frequentList.end() )
				{
					cout << "error! I couldn't find subtree in frequent set!" << endl;
					exit(1);
				}
				else
					(pos2->second).support = 0;
				delete pct2;
			}
		}
		delete pct1;
	}

	totalMaximum = 0;
	for ( pos2 = rhs.frequentList.begin(); pos2 != rhs.frequentList.end(); ++pos2 )
		if ( (pos2->second).support != 0 ) totalMaximum++;

	return totalMaximum;
}


//compute support from a database with name s
void FrequentTreeList::countSupport(const vector<ptrFreeTree>& database)
{
	TupleTree* ptt;
	map<CanonicalTree,supportNode>::iterator pos;
	char c;
	long tid;
	
	for ( pos = frequentList.begin(); pos != frequentList.end(); ++pos )
		(pos->second).tidList.push_back(INFINITY_LONG);

	for ( long s = 1; s < database.size(); s++ )
	{
		ptt = new TupleTree(*database[s]);
		ptt->rootIt();
		tid = ptt->returnTid();

		for ( pos = frequentList.begin(); pos != frequentList.end(); ++pos )
		{
			while ( (pos->second).tidList.front() < tid )
			{
				(pos->second).tidList.pop_front(); //deleted unused tids
			}
			if ( (pos->second).tidList.front() == tid )
			{
				if ( ptt->support(pos->first) )
				{
					pos->second.support++;
					(pos->second).tidList.push_back(tid);
				}
				(pos->second).tidList.pop_front();
			}
		}

		delete ptt;
	}

	for ( pos = frequentList.begin(); pos != frequentList.end(); ++pos )
	{
		while ( (pos->second).tidList.front() != INFINITY_LONG )
			(pos->second).tidList.pop_front();
		(pos->second).tidList.pop_front();
	}
}

//remove elements whose support is less than a threshold
void FrequentTreeList::finalize(long threshold)
{
	map<CanonicalTree,supportNode>::iterator pos;
	for ( pos = frequentList.begin(); pos != frequentList.end(); )
	{
		if ( (pos->second).support < threshold )
			frequentList.erase(pos++);
		else ++pos;
	}
}

//to check if a canonicalTree is in the list
bool FrequentTreeList::downwardSupport(const CanonicalTree& rhs)
{
	map<CanonicalTree,supportNode>::iterator pos;
	pos = frequentList.find(rhs);
	if ( pos != frequentList.end() ) return true;
	else return false;
}

void FrequentTreeList::test()
{
	map<CanonicalTree,supportNode>::iterator pos;
	list<long>::iterator pos1;
	for ( pos = frequentList.begin(); pos != frequentList.end(); ++pos)
	{
		for ( pos1 = (pos->second).tidList.begin(); pos1 != (pos->second).tidList.end(); ++pos1 )
			cout << *pos1 << ' ';
		cout << endl;
	}
		

}

istream& operator>>(istream& in, FrequentTreeList& rhs)
{
	return in;
}

ostream& operator<<(ostream& out, const FrequentTreeList& rhs)
{
	return out;
}
