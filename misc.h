using namespace std;

#ifndef MISC_H
#define MISC_H

const bool POPULATE_CHECK = false;

const short ENDOFTREETAG = 3002;
const short ENDTAG = 3001;
const short INFINITY_SHORT = 3003; //a number greater than any short
const long INFINITY_LONG = 2000000000; //a number greater than any long

extern long SUPPORT_THRESHOLD;

//as a result, the maximal number of vertices in a tree is 3000

class FreeTree;
typedef FreeTree* ptrFreeTree;


struct Edge
{
  short vertex1; //one end of the edge
  short vertex2; //the other end of the edge
  short eLabel; //the label of the edge
  Edge(short v = 0, short w = 0, short eLabel = 0) : 
    vertex1(v), vertex2(w), eLabel(eLabel) { }
  Edge(const Edge& rhs) { vertex1 = rhs.vertex1; vertex2 = rhs.vertex2; eLabel = rhs.eLabel; }
  Edge& operator=(const Edge& rhs) 
  { vertex1 = rhs.vertex1; vertex2 = rhs.vertex2; eLabel = rhs.eLabel; return *this; }
};

#endif //MISC_H
